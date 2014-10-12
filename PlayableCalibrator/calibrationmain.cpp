/*
 * SMBX64 Playble Character Sprite Calibrator, a free tool for playable srite design
 * This is a part of the Platformer Game Engine by Wohlstand, a free platform for game making
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
//#include <QtDebug>
#include <QSettings>

#include "calibrationmain.h"
#include "ui_calibrationmain.h"
#include "animator/SpriteScene.h"
#include "about/about.h"
#include "frame_matrix/matrix.h"
#include "animator/animate.h"
#include "animator/aniFrames.h"
#include "image_calibration/image_calibrator.h"

#include "main/globals.h"
#include "main/graphics.h"

CalibrationMain::CalibrationMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CalibrationMain)
{
    //qDebug() << "Loaded";

    buffer.W=100;
    buffer.H=100;
    buffer.offsetX=0;
    buffer.offsetY=0;
    buffer.used=true;
    wasCanceled=false;

    //qDebug() << "Struct set";

    currentFile = "";//QApplication::applicationDirPath()+"/peach-2.gif";

    //qDebug() << "set filename";

    ui->setupUi(this);

    //qDebug() << "set ui";

        QString inifile = QApplication::applicationDirPath() + "/" + "pge_calibrator.ini";
        QSettings settings(inifile, QSettings::IniFormat);
        settings.beginGroup("Main");
            LastOpenDir = settings.value("lastpath", ".").toString();
            currentFile = settings.value("lastfile", currentFile).toString();
        settings.endGroup();

    //qDebug() << "set settings";

    QVector<frameOpts > framesY;
    frameOpts spriteData;

    //qDebug() << "set struct and vectors";

    // Write default values
    for(int i=0; i<10; i++)
    {
        framesY.clear();
        for(int j=0; j<10; j++)
        {
            spriteData=buffer;
            framesY.push_back(spriteData);
        }
        framesX.push_back(framesY);
    }

    //qDebug() << "create scene";

    Scene = new SpriteScene;

    //qDebug() << "set scene";
    ui->PreviewGraph->setScene(Scene);

    if(currentFile.isEmpty())
    {
        if(!on_OpenSprite_clicked())
        {
            wasCanceled=true;
            return;
        }

    }

    //qDebug() << "load config";

    loadConfig(currentFile);


    //qDebug() << "set options 1 " + QString::number(CurFRMx) + "-"  + QString::number(CurFRMy);

    ui->Height->setValue(framesX[CurFRMx][CurFRMy].H);

    //qDebug() << "set options 2";
    ui->Width->setValue(framesX[CurFRMx][CurFRMy].W);

    //qDebug() << "set options 3";
    ui->OffsetX->setValue(framesX[CurFRMx][CurFRMy].offsetX);

   // qDebug() << "set options 4";
    ui->OffsetY->setValue(framesX[CurFRMx][CurFRMy].offsetY);

    //qDebug() << "set options 5";
    ui->EnableFrame->setChecked(framesX[CurFRMx][CurFRMy].used);

    //qDebug() << "seted";

    //Animation timer
  /*
  QTimer *timer = new QTimer(this);
    connect(
                timer, SIGNAL(timeout()),
                this,
                SLOT( nextFrame() ) );
    timer->start(128);
    */

}

void CalibrationMain::closeEvent(QCloseEvent *event)
{
    QString config = QApplication::applicationDirPath() + "/" + "pge_calibrator.ini";
    QSettings opts(config, QSettings::IniFormat);
    opts.beginGroup("Main");
        opts.setValue("lastpath", LastOpenDir);
        opts.setValue("lastfile", currentFile);
    opts.endGroup();

    event->accept();
}




CalibrationMain::~CalibrationMain()
{
    delete ui;
}

SpriteScene::SpriteScene(QObject *parent) : QGraphicsScene(parent)
{
    QGraphicsItem * dragon;
    QGraphicsItem * Frame;
    QGraphicsItem * Square;
    QList<QString > tmp;
    QFileInfo ourFile(currentFile);
    QString imgFileM;
    QPixmap sprite;
    croc = new QGraphicsPixmapItem;
    sizer = new QGraphicsRectItem;

    mCurrentFrameX = 0;
    mCurrentFrameY = 0;

    x=0; y=0, dir=1;
    mPos = QPoint(0,0);

    tmp = ourFile.fileName().split(".", QString::SkipEmptyParts);
    if(tmp.size()==2)
        imgFileM = tmp[0] + "m." + tmp[1];
    else
        imgFileM = "";

    QImage maskImg;
    if(QFile::exists(ourFile.absoluteDir().path() + "/" + imgFileM))
        maskImg = Graphics::loadQImage( ourFile.absoluteDir().path() + "/" + imgFileM );
    else
        maskImg = QImage();

    sprite = QPixmap::fromImage(
                    Graphics::setAlphaMask(
                        Graphics::loadQImage( currentFile )
                        , maskImg )
                    );


    mSpriteImage = sprite;

    draw();

    Frame = addRect(0,0,100,100, QPen(Qt::gray, 1), Qt::transparent);
    Frame->setZValue(-10);

    croc->setPixmap(QPixmap(currentImage));
    dragon = addPixmap(QPixmap(currentImage));
    croc->setParentItem(dragon);
    dragon->setZValue(0);

    Square = addRect(0,0,0,0, QPen(Qt::NoPen), Qt::transparent);
    sizer->setRect(0,0,100,100);
    sizer->setPen(QPen(Qt::green));
    sizer->setParentItem(Square);
    sizer->setZValue(3);

    //qDebug() << "Scene created";
}

void SpriteScene::draw()
{
    currentImage =  mSpriteImage.copy(QRect(mPos.x(), mPos.y(), 100, 100));
            // mPos.x(),mPos.y(), *mSpriteImage, mCurrentFrame, 0, 100,100 );
}

QPoint SpriteScene::pos() const
{
    return mPos;
}

void SpriteScene::setFrame(int x, int y)
{
    CurFRMx=x;
    CurFRMy=y;
    mCurrentFrameX = 100 * x;
    mCurrentFrameY = 100 * y;

    mPos.setX( mCurrentFrameX );
    mPos.setY( mCurrentFrameY );

    draw();

    croc->setPixmap(QPixmap(currentImage));
}


void SpriteScene::setSquare(int x, int y, int h, int w)
{
    //following variable keeps track which
    //frame to show from sprite sheet
    sizer->setRect(x,y,w-1,h-1);
}


void CalibrationMain::on_FrameX_valueChanged(int arg1)
{
    int x=arg1, y=ui->FrameY->value();
    Scene->setFrame(x, y);

    if((framesX.size()>=10)&& (framesX[x].size()>=10))
    {
        ui->Height->setValue(framesX[x][y].H);
        ui->Width->setValue(framesX[x][y].W);
        ui->OffsetX->setValue(framesX[x][y].offsetX);
        ui->OffsetY->setValue(framesX[x][y].offsetY);
        ui->EnableFrame->setChecked(framesX[x][y].used);
    }
}

void CalibrationMain::on_FrameY_valueChanged(int arg1)
{
    int x=ui->FrameX->value(), y=arg1;
    Scene->setFrame(x, y);
    if((framesX.size()>=10)&& (framesX[x].size()>=10))
    {
        ui->Height->setValue(framesX[x][y].H);
        ui->Width->setValue(framesX[x][y].W);
        ui->OffsetX->setValue(framesX[x][y].offsetX);
        ui->OffsetY->setValue(framesX[x][y].offsetY);
        ui->EnableFrame->setChecked(framesX[x][y].used);
    }
}

void CalibrationMain::on_Height_valueChanged(int arg1)
{
    int x,y,h,w, ax, ay;

    ax = Scene->mCurrentFrameX/100;
    ay = Scene->mCurrentFrameY/100;

    x = framesX[ax][ay].offsetX;
    y = framesX[ax][ay].offsetY;
    h = framesX[ax][ay].H;
    w = framesX[ax][ay].W;
    h = arg1;
    Scene->setSquare(x,y,h,w);
    if(ui->Height->hasFocus())
        framesX[ax][ay].H = h;
}

void CalibrationMain::on_Width_valueChanged(int arg1)
{
    int x,y,h,w, ax, ay;

    ax = Scene->mCurrentFrameX/100;
    ay = Scene->mCurrentFrameY/100;

    x = framesX[ax][ay].offsetX;
    y = framesX[ax][ay].offsetY;
    h = framesX[ax][ay].H;
    w = framesX[ax][ay].W;
    w = arg1;
    Scene->setSquare(x,y,h,w);
    if(ui->Width->hasFocus())
        framesX[ax][ay].W = w;
}

void CalibrationMain::on_OffsetX_valueChanged(int arg1)
{
    int x,y,h,w, ax, ay;

    ax = Scene->mCurrentFrameX/100;
    ay = Scene->mCurrentFrameY/100;

    x = framesX[ax][ay].offsetX;
    y = framesX[ax][ay].offsetY;
    h = framesX[ax][ay].H;
    w = framesX[ax][ay].W;
    x = arg1;
    Scene->setSquare(x,y,h,w);
    if(ui->OffsetX->hasFocus())
        framesX[ax][ay].offsetX = x;
}

void CalibrationMain::on_OffsetY_valueChanged(int arg1)
{
    int x,y,h,w, ax, ay;

    ax = Scene->mCurrentFrameX/100;
    ay = Scene->mCurrentFrameY/100;

    x = framesX[ax][ay].offsetX;
    y = framesX[ax][ay].offsetY;
    h = framesX[ax][ay].H;
    w = framesX[ax][ay].W;
    y = arg1;
    Scene->setSquare(x,y,h,w);
    if(ui->OffsetY->hasFocus())
        framesX[ax][ay].offsetY = y;
}

void CalibrationMain::on_CopyButton_clicked()
{
    buffer.H=ui->Height->value();
    buffer.W=ui->Width->value();
    buffer.offsetX=ui->OffsetX->value();
    buffer.offsetY=ui->OffsetY->value();
    buffer.used=ui->EnableFrame->isChecked();
}

void CalibrationMain::on_PasteButton_clicked()
{
    int ax, ay;
    ax = Scene->mCurrentFrameX/100;
    ay = Scene->mCurrentFrameY/100;
    framesX[ax][ay] = buffer;

    ui->Height->setValue(buffer.H);
    ui->Width->setValue(buffer.W);
    ui->OffsetX->setValue(buffer.offsetX);
    ui->OffsetY->setValue(buffer.offsetY);
    ui->EnableFrame->setChecked(buffer.used);
}

bool CalibrationMain::on_OpenSprite_clicked()
{
     QString fileName_DATA = QFileDialog::getOpenFileName(this,
            trUtf8("Open sprite file"),(LastOpenDir.isEmpty()? QApplication::applicationDirPath() : LastOpenDir),
            tr("SMBX playble sprite (mario-*.gif peach-*.gif toad-*.gif luigi-*.gif link-*.gif);;"
               "GIF images (*.gif);;"
               "PNG images (*.png);;"
                "All Files (*.*)"));

     if(fileName_DATA==NULL) return false;

     OpenFile(fileName_DATA);

     return true;
}

void CalibrationMain::on_AboutButton_clicked()
{
    about dialog;
    dialog.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    dialog.exec();
}

void CalibrationMain::on_SaveConfigButton_clicked()
{
    saveConfig(currentFile);
}

// Copy current sizes and offsets to ALL frames
void CalibrationMain::on_applyToAll_clicked()
{
    buffer.H=ui->Height->value();
    buffer.W=ui->Width->value();
    buffer.offsetX=ui->OffsetX->value();
    buffer.offsetY=ui->OffsetY->value();
    for(int i=0; i<10; i++)
        for(int j=0; j<10; j++)
            framesX[i][j] = buffer;

}

//Set using this frame on template
void CalibrationMain::on_EnableFrame_clicked(bool checked)
{
    int ax, ay;
    if(ui->EnableFrame->hasFocus())
    {
        ax = Scene->mCurrentFrameX/100;
        ay = Scene->mCurrentFrameY/100;
        framesX[ax][ay].used = checked;
    }
}


void CalibrationMain::on_Matrix_clicked()
{
    Matrix dialog(framesX, Scene);
    this->hide();
    dialog.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    if(dialog.exec()==QDialog::Accepted)
    {
        ui->FrameX->setValue(dialog.frameX);
        ui->FrameY->setValue(dialog.frameY);
    }
    this->show();

    framesX = dialog.FrameConfig;

    int x=ui->FrameX->value(), y=ui->FrameY->value();
    Scene->setFrame(x, y);

    if((framesX.size()>=10)&& (framesX[x].size()>=10))
    {
        ui->Height->setValue(framesX[x][y].H);
        ui->Width->setValue(framesX[x][y].W);
        ui->OffsetX->setValue(framesX[x][y].offsetX);
        ui->OffsetY->setValue(framesX[x][y].offsetY);
        ui->EnableFrame->setChecked(framesX[x][y].used);
    }
}

void CalibrationMain::on_AnimatorButton_clicked()
{
    this->hide();
    Animate dialog(framesX, AnimationFrames, Scene);
    dialog.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    dialog.exec();
    this->show();
    AnimationFrames = dialog.AniFrames;
}

void CalibrationMain::on_editSizes_clicked()
{
    QMessageBox::StandardButton reply;
      reply = QMessageBox::question(this, "Warning",
        "This is a physical settings for a sprite, this need only for creation of new character sprite\nIf you want to use this sprite in SMBX, please, don't edit this settings.\nDo you want to continue?",
                                    QMessageBox::Yes|QMessageBox::No);
      if (reply == QMessageBox::Yes)
      {
            ui->EnableFrame->setEnabled(true);
            ui->Height->setEnabled(true);
            ui->Width->setEnabled(true);
            ui->OffsetX->setEnabled(true);
            ui->OffsetY->setEnabled(true);
            ui->CopyButton->setEnabled(true);
            ui->PasteButton->setEnabled(true);
            ui->applyToAll->setEnabled(true);
            ui->editSizes->setVisible(false);
      }
}

void CalibrationMain::on_calibrateImage_clicked()
{
    ImageCalibrator imgCalibrator;
    imgCalibrator.setWindowFlags(Qt::Window | Qt::WindowCloseButtonHint);
    if(!imgCalibrator.init(currentFile)) return;

    imgCalibrator.Scene = Scene;

    this->hide();
    imgCalibrator.exec();
    this->show();
    OpenFile(imgCalibrator.targetPath);
}
