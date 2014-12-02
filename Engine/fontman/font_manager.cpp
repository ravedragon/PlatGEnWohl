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

#include "font_manager.h"

#include "../common_features/app_path.h"
#include "../common_features/graphics_funcs.h"

#include <QtDebug>
#include <QFile>

#include <QImage>
#include <QPainter>
#include <QFontMetrics>
#include <QFontDatabase>
#include <QGLWidget>

#include <SDL2/SDL.h>
#undef main

bool FontManager::isInit=false;
//TTF_Font * FontManager::defaultFont=NULL;

GLuint FontManager::textTexture=0;

int     FontManager::fontID;
QFont FontManager::defaultFont;

void FontManager::init()
{
    //    int ok = TTF_Init();
    //    if(ok==-1)
    //    {
    //        qDebug() << "Can't load Font manager: " << TTF_GetError() << "\n";
    //        return;
    //    }

    //defaultFont = buildFont(ApplicationPath + "/fonts/PressStart2P.ttf", 14);
    //if(defaultFont==NULL)
    //    return;

    fontID = QFontDatabase::addApplicationFont(":/PressStart2P.ttf");

    QString family("Press Start 2P");
    if(!QFontDatabase::applicationFontFamilies(fontID).isEmpty())
        family = QFontDatabase::applicationFontFamilies(fontID).at(0);

    defaultFont.setFamily(family);//font.setWeight(14);
    defaultFont.setPointSize(12);

    //defaultFont = buildFont_RW(":/PressStart2P.ttf", 14);

    isInit = true;
}

void FontManager::quit()
{}

//TTF_Font *FontManager::buildFont(QString _fontPath, GLint size)
//{
//    TTF_Font *temp_font = TTF_OpenFont(_fontPath.toLocal8Bit(), size);
//    if(!temp_font)
//    {
//        qDebug() << "TTF_OpenFont: " << TTF_GetError() << "\n";
//        return NULL;
//    }

//    TTF_SetFontStyle( temp_font, TTF_STYLE_NORMAL );

//    return temp_font;
//}

//TTF_Font *FontManager::buildFont_RW(QString _fontPath, GLint size)
//{
//    QFile font(_fontPath);
//    if(!font.open(QFile::ReadOnly))
//        {
//            qDebug() << "Can't open file" << _fontPath;
//            return NULL;
//        }
//    QDataStream in(&font);
//    int bufferSize = font.size();
//    char *temp = new char[bufferSize];
//    qDebug() << "read data";
//    in.readRawData(temp, bufferSize);

//    qDebug() << "read font";

//    TTF_Font *temp_font = TTF_OpenFontRW(SDL_RWFromMem(temp, (int)bufferSize), 0, size);
//    if(!temp_font)
//    {
//        qDebug() << "TTF_OpenFont: " << TTF_GetError() << "\n";
//        return NULL;
//    }

//    //temp[bufferSize-1] = '\n';
//    //delete[] temp;

//    TTF_SetFontStyle( temp_font, TTF_STYLE_NORMAL );
//    return temp_font;
//}

void FontManager::SDL_string_texture_create(QFont &font, QRgb color, QString &text, GLuint *texture)
{
    if(!isInit) return;

    QFontMetrics meter(font);
    QImage text_image = QImage(meter.width(text), meter.height()*text.split('\n').size(), QImage::Format_ARGB32);
    text_image.fill(Qt::transparent);

    QPainter x(&text_image);
    x.setFont(font);
    x.setBrush(QBrush(color));
    x.setPen(QPen(color));
    x.drawText(text_image.rect(), text);
    x.end();

    text_image = QGLWidget::convertToGLFormat(text_image);//.mirrored(false, true);

    glGenTextures(1, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0,  4, text_image.width(), text_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text_image.bits() );

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void FontManager::SDL_string_texture_create(QFont &font, QRect limitRect, int fontFlags, QRgb color, QString &text, GLuint *texture)
{
    if(!isInit) return;

    //QFontMetrics meter(font);
    QImage text_image = QImage(limitRect.size(), QImage::Format_ARGB32);
    text_image.fill(Qt::transparent);

    QPainter x(&text_image);
    x.setFont(font);
    x.setBrush(QBrush(color));
    x.setPen(QPen(color));
    x.drawText(text_image.rect(), fontFlags, text);
    x.end();

    text_image = QGLWidget::convertToGLFormat(text_image);//.mirrored(false, true);

    glGenTextures(1, texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glBindTexture(GL_TEXTURE_2D, *texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0,  4, text_image.width(), text_image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, text_image.bits() );

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
}

void FontManager::SDL_string_render2D( GLuint x, GLuint y, GLuint *texture )
{
    if(!isInit) return;

    GLint w;
    GLint h;

    glEnable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, *texture);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH, &w);
    glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

    glColor4f( 1.f, 1.f, 1.f, 1.f);
    glBegin(GL_QUADS);
        glTexCoord2f(0.0,1.0);glVertex3f(x, y, 0.0);
        glTexCoord2f(0.0,0.0);glVertex3f(x, y+h, 0.0) ;
        glTexCoord2f(1.0,0.0);glVertex3f(x+w, y+h, 0.0);
        glTexCoord2f(1.0,1.0);glVertex3f(x+w, y, 0.0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

void FontManager::printText(QString text, int x, int y)
{
    if(!isInit) return;
    SDL_string_texture_create(defaultFont, qRgba(255,255,255,255), text, &textTexture);
    SDL_string_render2D(x, y, &textTexture );
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &textTexture );
}

GLuint FontManager::TextToTexture(QString text, QRect rectangle, int alignFlags)
{
    if(!isInit) return 0;

    GLuint fontTexture;
    SDL_string_texture_create(defaultFont,rectangle, alignFlags, qRgba(255,255,255,255), text, &fontTexture);
    return fontTexture;
    //SDL_string_render2D(x, y, &textTexture );
    //glDisable(GL_TEXTURE_2D);
    //glDeleteTextures(1, &textTexture );
}

void FontManager::printText(QString text, int x, int y, int pointSize, QRgb color)
{
    if(!isInit) return;

    QString(family);

    if(!QFontDatabase::applicationFontFamilies(fontID).isEmpty())
            family = QFontDatabase::applicationFontFamilies(fontID).at(0);

    QFont font(family);//font.setWeight(14);
    font.setPointSize(pointSize);

    SDL_string_texture_create(font, color, text, &textTexture);
    SDL_string_render2D(x, y, &textTexture );
    glDisable(GL_TEXTURE_2D);
    glDeleteTextures(1, &textTexture );
}

