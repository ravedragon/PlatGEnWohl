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

#ifndef LEVELEDIT_H
#define LEVELEDIT_H

#include <QtWidgets>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "../file_formats/lvl_filedata.h"

#include "../level_scene/lvl_scene.h"

#include "../data_configs/data_configs.h"

namespace Ui {
class leveledit;
}

class leveledit : public QWidget
{
    Q_OBJECT
    friend class LvlScene;
public:
    explicit leveledit(QWidget *parent = 0);
    ~leveledit();

    LevelData LvlData;
    //QGraphicsScene LvlScene;

    void newFile(dataconfigs &configs, LevelEditingSettings options);
    bool loadFile(const QString &fileName, LevelData FileData, dataconfigs &configs, LevelEditingSettings options);
    bool save(bool savOptionsDialog = false);
    bool saveAs(bool savOptionsDialog = false);
    bool saveFile(const QString &fileName, const bool addToRecent = true);
    QString userFriendlyCurrentFile();
    QString currentFile() { return curFile; }

    void setCurrentSection(int scId);
    void ResetPosition();
    void goTo(long x, long y, bool SwitchToSection=false, QPoint offset=QPoint(0,0), bool center=false);

    void ResetZoom();
    void zoomIn();
    void zoomOut();
    void setZoom(int percent);
    int  getZoom();

    QGraphicsView* getGraphicsView();

    enum CusrorMode
    {
        MODE_HandDrag=-1,
        MODE_Selecting=0,
        MODE_Erasing,
        MODE_PlaceItem,
        MODE_DrawSquares,
        MODE_Pasting,
        MODE_Resizing
    };

    void changeCursor(int mode);

    void ExportToImage_fn();
    void ExportToImage_fn_piece();

    LvlScene * scene;

    bool sceneCreated;
    bool isUntitled;

    QString curFile;
        
    QTimer *updateTimer;
    void setAutoUpdateTimer(int ms);
    void stopAutoUpdateTimer();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    virtual void mouseReleaseEvent( QMouseEvent * event );
    virtual void leaveEvent(QEvent * leaveEvent);
    virtual void focusInEvent(QFocusEvent *event);
    void updateScene();

    void ExportingReady();

private:
    void documentWasModified();
    Ui::leveledit *ui;

    LevelData StartLvlData;
    bool DrawObjects(QProgressDialog &progress);

    bool isModified;
    bool maybeSave();
    void setCurrentFile(const QString &fileName);
    void setDataBoxes();
    QString strippedName(const QString &fullFileName);
    QString latest_export_path;
    unsigned int FileType;

};

#endif // LEVELEDIT_H
