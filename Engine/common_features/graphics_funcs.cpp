/*
 * Platformer Game Engine by Wohlstand, a free platform for game making
 * Copyright (c) 2014 Vitaly Novichkov <admin@wohlnet.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QPixmap>
#include <QImage>
#include <QRgb>
#include <QFileInfo>
#include <QMessageBox>
#include <QtOpenGL/QGLWidget>


#include "graphics_funcs.h"
#include "../../_Libs/EasyBMP/EasyBMP.h"

#include <QtDebug>

QImage GraphicsHelps::setAlphaMask(QImage image, QImage mask)
{
    if(mask.isNull())
        return image;

    if(image.isNull())
        return image;

    QImage target = image;
    QImage newmask = mask;

    if(target.size()!= newmask.size())
    {
        newmask = newmask.copy(0,0, target.width(), target.height());
    }

    newmask.invertPixels();

    target.setAlphaChannel(newmask);

    return target;
}

QImage GraphicsHelps::fromBMP(QString &file)
{
    QImage errImg;

    BMP tarBMP;
    if(!tarBMP.ReadFromFile( file.toLocal8Bit().data() )){
        return errImg; //Check if empty with errImg.isNull();
    }

    QImage bmpImg(tarBMP.TellWidth(),tarBMP.TellHeight(),QImage::Format_RGB666);

    for(int x = 0; x < tarBMP.TellWidth(); x++){
        for(int y = 0; y < tarBMP.TellHeight(); y++){
            RGBApixel pixAt = tarBMP.GetPixel(x,y);
            bmpImg.setPixel(x,y,qRgb(pixAt.Red, pixAt.Green, pixAt.Blue));
        }
    }

    return bmpImg;
}

//QPixmap GraphicsHelps::loadPixmap(QString file)
//{
//    return QPixmap::fromImage(loadQImage(file));
//}

QImage GraphicsHelps::loadQImage(QString file)
{
    QImage image = QImage( file );
    if(image.isNull())
        image = fromBMP( file);
    return image;
}

PGE_Texture GraphicsHelps::loadTexture(PGE_Texture &target, QString path, QString maskPath)
{
    QImage sourceImage;
    // Load the OpenGL texture
    sourceImage = loadQImage(path); // Gives us the information to make the texture

    if(sourceImage.isNull())
    {
        SDL_Quit();
        //if(ErrorCheck::hardMode)
        //{
            QMessageBox::critical(NULL, "Texture error",
                QString("Error loading of image file: \n%1\nReason: %2.")
                .arg(path).arg(QFileInfo(path).exists()?"wrong image format":"file not exist"), QMessageBox::Ok);
            exit(1);
        //}
        return target;
    }

    //Apply Alpha mask
    if(!maskPath.isEmpty() && QFileInfo(maskPath).exists())
    {
        QImage maskImage = loadQImage(maskPath);
        sourceImage = setAlphaMask(sourceImage, maskImage);
    }

    sourceImage.convertToFormat(QImage::Format_ARGB32);
    QRgb upperColor = sourceImage.pixel(0,0);
    target.ColorUpper.r = float(qRed(upperColor))/255.0f;
    target.ColorUpper.g = float(qGreen(upperColor))/255.0f;
    target.ColorUpper.b = float(qBlue(upperColor))/255.0f;

    QRgb lowerColor = sourceImage.pixel(0, sourceImage.height()-1);
    target.ColorLower.r = float(qRed(lowerColor))/255.0f;
    target.ColorLower.g = float(qGreen(lowerColor))/255.0f;
    target.ColorLower.b = float(qBlue(lowerColor))/255.0f;

    //qDebug() << path << sourceImage.size();

    sourceImage = QGLWidget::convertToGLFormat(sourceImage).mirrored(false, true);

    target.nOfColors = 4;
    target.format = GL_RGBA;

    glEnable(GL_TEXTURE_2D);
    // Have OpenGL generate a texture object handle for us
    glGenTextures( 1, &(target.texture) );

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, target.texture );

    // Edit the texture object's image data using the information SDL_Surface gives us
    target.w = sourceImage.width();
    target.h = sourceImage.height();
    // Set the texture's stretching properties

    // Bind the texture object
    glBindTexture( GL_TEXTURE_2D, target.texture );

    glTexImage2D(GL_TEXTURE_2D, 0, target.nOfColors, sourceImage.width(), sourceImage.height(),
         0, target.format, GL_UNSIGNED_BYTE, sourceImage.bits() );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glDisable(GL_TEXTURE_2D);

    return target;
}


QPixmap GraphicsHelps::squareImage(QPixmap image, QSize targetSize=QSize(0,0) )
{
    QPixmap target = QPixmap(targetSize);
    target.fill(Qt::transparent);
    QPixmap source;

    if( ( targetSize.width() < image.width() ) || ( targetSize.height() < image.height() ))
        source = image.scaled(targetSize, Qt::KeepAspectRatio);
    else
        source = image;

    QPainter p(&target);

    int targetX = qRound( ( ( qreal(target.width()) - qreal(source.width()) ) / 2 ) );
    int targetY = qRound( ( ( qreal(target.height()) - qreal(source.height()) ) / 2 ) );

    p.drawPixmap( targetX, targetY,source.width(),source.height(), source );

    p.end();
    return target;
}

SDL_Surface* GraphicsHelps::QImage_toSDLSurface(const QImage &sourceImage)
{
    // Ensure that the source image is in the correct pixel format
    QImage image = sourceImage;
    if (image.format() != QImage::Format_ARGB32)
        image = image.convertToFormat(QImage::Format_ARGB32);

    // QImage stores each pixel in ARGB format
    // Mask appropriately for the endianness
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    Uint32 amask = 0x000000ff;
    Uint32 rmask = 0x0000ff00;
    Uint32 gmask = 0x00ff0000;
    Uint32 bmask = 0xff000000;
#else
    Uint32 amask = 0xff000000;
    Uint32 rmask = 0x00ff0000;
    Uint32 gmask = 0x0000ff00;
    Uint32 bmask = 0x000000ff;
#endif

    return SDL_CreateRGBSurfaceFrom((void*)image.constBits(),
        image.width(), image.height(), image.depth(), image.bytesPerLine(),
        rmask, gmask, bmask, amask);
}
