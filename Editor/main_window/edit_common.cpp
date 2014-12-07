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

#include <ui_mainwindow.h>
#include "../mainwindow.h"

#include "../file_formats/file_formats.h"
#include "music_player.h"
#include "global_settings.h"
#include "../script/gui/additionalsettings.h"

//Reload opened file data
void MainWindow::on_actionReload_triggered()
{
    QString filePath;
    QRect wnGeom;

    if (activeChildWindow()==1)
    {
        LevelData FileData;
        filePath = activeLvlEditWin()->curFile;

        if(activeLvlEditWin()->isUntitled)
        {
                    QMessageBox::warning(this, tr("File not saved"),
                    tr("File doesn't saved on disk."), QMessageBox::Ok);
                        return;
        }

        if (!QFileInfo(filePath).exists() ) {
            QMessageBox::critical(this, tr("File open error"),
            tr("Can't open the file.\nFile not exist."), QMessageBox::Ok);
                return;
        }

        //Open level file
        FileData = FileFormats::OpenLevelFile(filePath);

        if( !FileData.ReadFileValid ){
            statusBar()->showMessage(tr("Reloading error"), 2000);
            return;}

        FileData.playmusic = GlobalSettings::autoPlayMusic;
        activeLvlEditWin()->LvlData.modified = false;

        QFile file(filePath+".meta");
        if(QFileInfo(filePath+".meta").exists())
        {
            if (file.open(QIODevice::ReadOnly))
            {
                QString metaRaw;
                QTextStream meta(&file);
                meta.setCodec("UTF-8");
                metaRaw = meta.readAll();
                if(FileData.metaData.script){
                    delete FileData.metaData.script;
                    FileData.metaData.script = NULL;
                }
                FileData.metaData = FileFormats::ReadNonSMBX64MetaData(metaRaw, filePath+".meta");
            }
            else
            {
                QMessageBox::critical(this, tr("File open error"),
                tr("Can't open the file."), QMessageBox::Ok);
            }
        }

        activeLvlEditWin()->close();
        wnGeom = ui->centralWidget->activeSubWindow()->geometry();
        ui->centralWidget->activeSubWindow()->close();

        leveledit *child = createLvlChild();
        if ((bool) (child->loadFile(filePath, FileData, configs, GlobalSettings::LvlOpts))) {
            child->show();
            ui->centralWidget->activeSubWindow()->setGeometry(wnGeom);
            child->updateGeometry();
            child->ResetPosition();
            statusBar()->showMessage(tr("Level file reloaded"), 2000);
            updateMenus(true);
            SetCurrentLevelSection(0);

            if(GlobalSettings::autoPlayMusic) ui->actionPlayMusic->setChecked(true);
            LvlMusPlay::musicForceReset=true; //reset musics
            on_actionPlayMusic_triggered(ui->actionPlayMusic->isChecked());

        } else {
                WriteToLog(QtDebugMsg, ">>File loading aborted");
            child->show();
                WriteToLog(QtDebugMsg, ">>Window showed");
            if(activeChildWindow()==1) activeLvlEditWin()->LvlData.modified = false;
                WriteToLog(QtDebugMsg, ">>Option set");
            ui->centralWidget->activeSubWindow()->close();
                WriteToLog(QtDebugMsg, ">>Windows closed");
        }
    }
    else
    if (activeChildWindow()==2)
    {
        filePath = activeNpcEditWin()->curFile;
        QFile fileIn(filePath);

        if(activeNpcEditWin()->isUntitled)
        {
                    QMessageBox::warning(this, tr("File not saved"),
                    tr("File doesn't saved on disk."), QMessageBox::Ok);
                        return;
        }

        if (!fileIn.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, tr("File open error"),
        tr("Can't open the file."), QMessageBox::Ok);
            return;
        }

        NPCConfigFile FileData = FileFormats::ReadNpcTXTFile(fileIn);
        if( !FileData.ReadFileValid ) return;
        wnGeom = ui->centralWidget->activeSubWindow()->geometry();
        activeNpcEditWin()->isModyfied = false;
        //activeNpcEditWin()->close();
        ui->centralWidget->activeSubWindow()->close();

        npcedit *child = createNPCChild();
        if (child->loadFile(filePath, FileData)) {
            statusBar()->showMessage(tr("NPC Config reloaded"), 2000);
            child->show();
            ui->centralWidget->activeSubWindow()->setGeometry(wnGeom);
            updateMenus(true);
        } else {
            child->close();
        }
    }
    else
    if (activeChildWindow()==3)
    {
        WorldData FileData;
        filePath = activeWldEditWin()->curFile;

        if(activeWldEditWin()->isUntitled)
        {
                    QMessageBox::warning(this, tr("File not saved"),
                    tr("File doesn't saved on disk."), QMessageBox::Ok);
                        return;
        }

        if (!QFileInfo(filePath).exists() ) {
            QMessageBox::critical(this, tr("File open error"),
            tr("Can't open the file.\nFile not exist."), QMessageBox::Ok);
                return;
        }

        FileData = FileFormats::OpenWorldFile(filePath);

        if( !FileData.ReadFileValid ){
            statusBar()->showMessage(tr("Reloading error"), 2000);
            return;}

        FileData.filename = QFileInfo(filePath).baseName();
        FileData.path = QFileInfo(filePath).absoluteDir().absolutePath();
        FileData.playmusic = GlobalSettings::autoPlayMusic;
        activeWldEditWin()->WldData.modified = false;

        QFile file(filePath+".meta");
        if(QFileInfo(filePath+".meta").exists())
        {
            if (file.open(QIODevice::ReadOnly))
            {
                QString metaRaw;
                QTextStream meta(&file);
                meta.setCodec("UTF-8");
                metaRaw = meta.readAll();
                FileData.metaData = FileFormats::ReadNonSMBX64MetaData(metaRaw, filePath+".meta");
            }
            else
            {
                QMessageBox::critical(this, tr("File open error"),
                tr("Can't open the file."), QMessageBox::Ok);
            }
        }

        activeWldEditWin()->close();
        wnGeom = ui->centralWidget->activeSubWindow()->geometry();
        ui->centralWidget->activeSubWindow()->close();

        WorldEdit *child = createWldChild();
        if ( (bool)(child->loadFile(filePath, FileData, configs, GlobalSettings::LvlOpts)) ) {
            child->show();
            ui->centralWidget->activeSubWindow()->setGeometry(wnGeom);
            child->updateGeometry();
            child->ResetPosition();
            updateMenus(true);
            setCurrentWorldSettings();
            if(FileData.HubStyledWorld)
            {
                ui->WorldSettings->setVisible(true);
                ui->WorldSettings->raise();
            }
            statusBar()->showMessage(tr("World map file loaded"), 2000);
        } else {
            WriteToLog(QtDebugMsg, ">>File loading aborted");
            child->show();
            WriteToLog(QtDebugMsg, ">>Window showed");
            if(activeChildWindow()==1) activeWldEditWin()->WldData.modified = false;
            WriteToLog(QtDebugMsg, ">>Option set");
            ui->centralWidget->activeSubWindow()->close();
            WriteToLog(QtDebugMsg, ">>Windows closed");
        }

    }

    clearFilter();
}


void MainWindow::on_actionExport_to_image_triggered()
{
    on_actionSelect_triggered();

    if(activeChildWindow()==1)
    {
        activeLvlEditWin()->ExportToImage_fn_piece();
    }
    else
    if(activeChildWindow()==3)
    {
        activeWldEditWin()->ExportToImage_fn();
    }
}

void MainWindow::on_actionExport_to_image_section_triggered()
{
    if(activeChildWindow()==1)
    {
        activeLvlEditWin()->ExportToImage_fn();
    }
}

void MainWindow::on_actionReset_position_triggered()
{
    if (activeChildWindow()==1)
    {
       activeLvlEditWin()->ResetPosition();
    }
    else
    if (activeChildWindow()==3)
    {
       activeWldEditWin()->ResetPosition();
    }
}



void MainWindow::on_actionAnimation_triggered(bool checked)
{
    GlobalSettings::LvlOpts.animationEnabled = checked;
    if (activeChildWindow()==1)
    {
        activeLvlEditWin()->scene->opts.animationEnabled = GlobalSettings::LvlOpts.animationEnabled;
        if(GlobalSettings::LvlOpts.animationEnabled)
        {
            activeLvlEditWin()->scene->startBlockAnimation();
        }
        else
            activeLvlEditWin()->scene->stopAnimation();
    }
    else
    if (activeChildWindow()==3)
    {
        activeWldEditWin()->scene->opts.animationEnabled = GlobalSettings::LvlOpts.animationEnabled;
        if(GlobalSettings::LvlOpts.animationEnabled)
        {
            activeWldEditWin()->scene->startAnimation();
        }
        else
            activeWldEditWin()->scene->stopAnimation();
    }
}


void MainWindow::on_actionCollisions_triggered(bool checked)
{
    GlobalSettings::LvlOpts.collisionsEnabled = checked;
    if (activeChildWindow()==1)
    {
        activeLvlEditWin()->scene->opts.collisionsEnabled = GlobalSettings::LvlOpts.collisionsEnabled;
    }
    else
    if (activeChildWindow()==3)
    {
        activeWldEditWin()->scene->opts.collisionsEnabled = GlobalSettings::LvlOpts.collisionsEnabled;
    }

}

// //////////////////////////////////////////////////////////////


void MainWindow::on_actionGridEn_triggered(bool checked)
{
    if (activeChildWindow()==1)
    {
       activeLvlEditWin()->scene->grid = checked;
    }
    else
    if (activeChildWindow()==3)
    {
       activeWldEditWin()->scene->grid = checked;
    }
}

// //History Manager
void MainWindow::on_actionUndo_triggered()
{
    ui->ItemProperties->hide();
    ui->WLD_ItemProps->hide();
    if (activeChildWindow()==1)
    {
        //Here must be call
        activeLvlEditWin()->scene->historyBack();
        ui->actionUndo->setEnabled( activeLvlEditWin()->scene->canUndo() );
        ui->actionRedo->setEnabled( activeLvlEditWin()->scene->canRedo() );
    }
    else if(activeChildWindow()==3)
    {
        activeWldEditWin()->scene->historyBack();
        ui->actionUndo->setEnabled( activeWldEditWin()->scene->canUndo() );
        ui->actionRedo->setEnabled( activeWldEditWin()->scene->canRedo() );
    }
}

void MainWindow::on_actionRedo_triggered()
{
    ui->ItemProperties->hide();
    ui->WLD_ItemProps->hide();
    if (activeChildWindow()==1)
    {
        //Here must be call
        activeLvlEditWin()->scene->historyForward();
        ui->actionUndo->setEnabled( activeLvlEditWin()->scene->canUndo() );
        ui->actionRedo->setEnabled( activeLvlEditWin()->scene->canRedo() );
    }
    else if(activeChildWindow()==3)
    {
        activeWldEditWin()->scene->historyForward();
        ui->actionUndo->setEnabled( activeWldEditWin()->scene->canUndo() );
        ui->actionRedo->setEnabled( activeWldEditWin()->scene->canRedo() );
    }
}


bool MainWindow::getCurrentSceneCoordinates(qreal &x, qreal &y)
{
    if(activeChildWindow() == 1)
    {
        leveledit* edit = activeLvlEditWin();
        QPointF coor = edit->getGraphicsView()->mapToScene(0,0);
        x = coor.x();
        y = coor.y();
        return true;
    }
    else if(activeChildWindow() == 3)
    {
        WorldEdit* edit = activeWldEditWin();
        QPointF coor = edit->getGraphicsView()->mapToScene(0,0);
        x = coor.x();
        y = coor.y();
        return true;
    }
    return false;
}




void MainWindow::on_actionAlign_selected_triggered()
{

    if(activeChildWindow()==1)
    {
        activeLvlEditWin()->scene->applyGridToEach(
                    activeLvlEditWin()->scene->selectedItems()   );
    }

}

void MainWindow::on_actionRotateLeft_triggered()
{

}

void MainWindow::on_actionRotateRight_triggered()
{

}

void MainWindow::on_actionFlipHorizontal_triggered()
{

}

void MainWindow::on_actionFlipVertical_triggered()
{

}


void MainWindow::on_actionAdditional_Settings_triggered()
{
    if(activeChildWindow() == 1){
        if(!activeLvlEditWin()->LvlData.metaData.script)
            return;

        AdditionalSettings *addSetting = new AdditionalSettings(configs.config_dir + "lunadll_settings.ini", activeLvlEditWin()->LvlData.metaData.script);

        addSetting->exec();
        addSetting->cleanup();


        delete addSetting;
    }

}

void MainWindow::on_actionCompile_To_triggered()
{
    if(activeChildWindow() == 1){
        if(!activeLvlEditWin()->LvlData.metaData.script)
            return;

        LevelData *lvlData = &activeLvlEditWin()->LvlData;

        QDir pathOfFile = QDir(lvlData->path);
        if(!pathOfFile.exists(lvlData->filename)){
            pathOfFile.mkdir(lvlData->filename);
        }

        if(lvlData->metaData.script->usingCompilerType() == Script::COMPILER_LUNALUA){



            QFile lunaLuaFile(lvlData->path+"/"+lvlData->filename+"/"+"lunadll.lua");
            if(lunaLuaFile.exists()){
                if(QMessageBox::warning(this, tr("Already exsist"), tr("Lunadll.lua already exsist!\nOverwrite?"), QMessageBox::Yes|QMessageBox::No)==QMessageBox::No){
                    return;
                }
                lunaLuaFile.remove();
            }

            if(!lunaLuaFile.open(QFile::WriteOnly | QFile::Text)){
                QMessageBox::warning(this, tr("File save error"),
                                     tr("Cannot save file %1:\n%2.")
                                     .arg("lunadll.lua")
                                     .arg(lunaLuaFile.errorString()));
                return;
            }

            {
                QTextStream out(&lunaLuaFile);
                out << lvlData->metaData.script->compileCode();
            }
            lunaLuaFile.close();

        }

    }
}
