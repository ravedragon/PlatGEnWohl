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

#include "../../ui_mainwindow.h"
#include "../../mainwindow.h"

#include "../../common_features/levelfilelist.h"
#include "../../file_formats/file_formats.h"

bool world_settings_lock_fields=false;
QList<QCheckBox * > WLD_CharacterCheckBoxes;


void MainWindow::setCurrentWorldSettings()
{
    world_settings_lock_fields=true;
    int WinType = activeChildWindow();
    //QCheckBox *
    if(WinType==3)
    {
        WorldEdit * edit = activeWldEditWin();
        ui->WLD_Title->setText( edit->WldData.EpisodeTitle );

        ui->WLD_AutostartLvl->setText( edit->WldData.autolevel );

        ui->WLD_Stars->setValue( edit->WldData.stars );

        ui->WLD_NoWorldMap->setChecked( edit->WldData.noworldmap );
        ui->actionWLDDisableMap->setChecked( edit->WldData.noworldmap );
        ui->WLD_RestartLevel->setChecked( edit->WldData.restartlevel );
        ui->actionWLDFailRestart->setChecked( edit->WldData.restartlevel );

        QString credits;
        credits += (edit->WldData.author1.isEmpty())? "" : edit->WldData.author1+"\n";
        credits += (edit->WldData.author2.isEmpty())? "" : edit->WldData.author2+"\n";
        credits += (edit->WldData.author3.isEmpty())? "" : edit->WldData.author3+"\n";
        credits += (edit->WldData.author4.isEmpty())? "" : edit->WldData.author4+"\n";
        credits += (edit->WldData.author5.isEmpty())? "" : edit->WldData.author5;
        ui->WLD_Credirs->setText( credits );

        //clear character list
        while(!WLD_CharacterCheckBoxes.isEmpty())
        {
            QCheckBox * tmp = WLD_CharacterCheckBoxes.first();
            WLD_CharacterCheckBoxes.pop_front();
            delete tmp;
        }

        ui->menuDisable_characters->clear();
        //ui->WLD_DisableCharacters


    }
    world_settings_lock_fields=false;
}








void MainWindow::on_actionWLDProperties_triggered()
{
    ui->actionWorld_settings->trigger();
}



void MainWindow::on_WLD_Title_textChanged(const QString &arg1)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        activeWldEditWin()->WldData.EpisodeTitle = arg1;
        activeWldEditWin()->WldData.modified = true;
        activeWldEditWin()->setWindowTitle(arg1 =="" ? activeWldEditWin()->userFriendlyCurrentFile() : arg1 );
    }

}

void MainWindow::on_WLD_NoWorldMap_clicked(bool checked)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        ui->actionWLDDisableMap->setChecked(checked);
        activeWldEditWin()->WldData.noworldmap = checked;
        activeWldEditWin()->WldData.modified = true;
    }
}

void MainWindow::on_actionWLDDisableMap_triggered(bool checked)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        ui->WLD_NoWorldMap->setChecked(checked);
        activeWldEditWin()->WldData.noworldmap = checked;
        activeWldEditWin()->WldData.modified = true;
    }
}


void MainWindow::on_WLD_RestartLevel_clicked(bool checked)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        ui->actionWLDFailRestart->setChecked(checked);
        activeWldEditWin()->WldData.restartlevel = checked;
        activeWldEditWin()->WldData.modified = true;
    }
}
void MainWindow::on_actionWLDFailRestart_triggered(bool checked)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        ui->WLD_RestartLevel->setChecked(checked);
        activeWldEditWin()->WldData.restartlevel = checked;
        activeWldEditWin()->WldData.modified = true;
    }
}


void MainWindow::on_WLD_AutostartLvl_textEdited(const QString &arg1)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        activeWldEditWin()->WldData.autolevel = arg1;
        activeWldEditWin()->WldData.modified = true;
    }
}

void MainWindow::on_WLD_AutostartLvlBrowse_clicked()
{
    if(world_settings_lock_fields) return;

    QString dirPath;
    if(activeChildWindow()==3)
    {
        dirPath = activeWldEditWin()->WldData.path;
    }
    else
        return;

    LevelFileList levelList(dirPath, ui->WLD_AutostartLvl->text());
    if( levelList.exec() == QDialog::Accepted )
    {
        ui->WLD_AutostartLvl->setText(levelList.SelectedFile);
        on_WLD_AutostartLvl_textEdited(levelList.SelectedFile);
    }

}

void MainWindow::on_WLD_Stars_valueChanged(int arg1)
{
    if(world_settings_lock_fields) return;
    if (activeChildWindow()==3)
    {
        activeWldEditWin()->WldData.stars = arg1;
        activeWldEditWin()->WldData.modified = true;
    }

}

void MainWindow::on_WLD_DoCountStars_clicked()
{
    if(world_settings_lock_fields) return;

    //Count stars of all used levels on this world map

    QString dirPath;
    long starzzz=0;

    if (activeChildWindow()==3)
    {
        WorldEdit * edit = activeWldEditWin();
        dirPath = edit->WldData.path;

        QRegExp lvlext = QRegExp("*.lvl");
        lvlext.setPatternSyntax(QRegExp::Wildcard);

        QProgressDialog progress(tr("Counting stars of placed levels"), tr("Abort"), 0, edit->WldData.levels.size(), this);
             progress.setWindowTitle(tr("Counting stars..."));
             progress.setWindowModality(Qt::WindowModal);
             progress.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint | Qt::WindowStaysOnTopHint);
             progress.setFixedSize(progress.size());
             progress.setGeometry(QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, progress.size(), qApp->desktop()->availableGeometry()));
             progress.setMinimumDuration(0);

        for(int i=0;i<edit->WldData.levels.size();i++)
        {
            //Attempt to read stars quantity of level:

            if(edit->WldData.levels[i].lvlfile.isEmpty()) continue;
            QString FilePath = dirPath+"/"+edit->WldData.levels[i].lvlfile;
            if(!QFile(FilePath).exists()) continue;

            QFile file(FilePath);
            if (!file.open(QIODevice::ReadOnly)) continue;

            LevelData getLevelHead;
            getLevelHead.stars = 0;
            if( lvlext.exactMatch(FilePath) )
            {
                getLevelHead = FileFormats::ReadLevelFile(file); //function in file_formats.cpp
                if( !getLevelHead.ReadFileValid ) continue;
            }
            file.close();
            starzzz+=getLevelHead.stars;

            if(progress.wasCanceled()) break;
            progress.setValue(i);
            qApp->processEvents();
        }

        if(progress.wasCanceled()) return;
        ui->WLD_Stars->setValue(starzzz);
        progress.close();
    }
}

void MainWindow::on_WLD_Credirs_textChanged()
{
    if(world_settings_lock_fields) return;

    QStringList credits = ui->WLD_Credirs->toPlainText().split(QChar('\n'));

    if (activeChildWindow()==3)
    {
        activeWldEditWin()->WldData.author1 = (credits.size()>0) ? credits[0] : "";
        activeWldEditWin()->WldData.author2 = (credits.size()>1) ? credits[1] : "";
        activeWldEditWin()->WldData.author3 = (credits.size()>2) ? credits[2] : "";
        activeWldEditWin()->WldData.author4 = (credits.size()>3) ? credits[3] : "";
        activeWldEditWin()->WldData.author5 = (credits.size()>4) ? credits[4] : "";
        activeWldEditWin()->WldData.modified = true;
    }
}



