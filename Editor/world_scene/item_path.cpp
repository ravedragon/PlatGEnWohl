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

#include "item_path.h"
#include "../common_features/logger.h"

#include "../common_features/mainwinconnect.h"


ItemPath::ItemPath(QGraphicsItem *parent)
    : QGraphicsItem(parent)
{
    gridSize=32;
    gridOffsetX=0;
    gridOffsetY=0;
    isLocked=false;

    animatorID=-1;
    imageSize = QRectF(0,0,10,10);

    mouseLeft=false;
    mouseMid=false;
    mouseRight=false;
}


ItemPath::~ItemPath()
{
    //WriteToLog(QtDebugMsg, "!<-BGO destroyed->!");
    //if(timer) delete timer;
}

void ItemPath::mousePressEvent ( QGraphicsSceneMouseEvent * mouseEvent )
{
    if(scene->DrawMode)
    {
        unsetCursor();
        ungrabMouse();
        this->setSelected(false);
        return;
    }

    //Discard multi-mouse keys
    if((mouseLeft)||(mouseMid)||(mouseRight))
    {
        mouseEvent->accept();
        return;
    }

    if( mouseEvent->buttons() & Qt::LeftButton )
        mouseLeft=true;
    if( mouseEvent->buttons() & Qt::MiddleButton )
        mouseMid=true;
    if( mouseEvent->buttons() & Qt::RightButton )
        mouseRight=true;

    QGraphicsItem::mousePressEvent(mouseEvent);
}


void ItemPath::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    int multimouse=0;
    bool callContext=false;
    if(((mouseMid)||(mouseRight))&&( mouseLeft^(mouseEvent->buttons() & Qt::LeftButton) ))
        multimouse++;
    if( (((mouseLeft)||(mouseRight)))&&( mouseMid^(mouseEvent->buttons() & Qt::MiddleButton) ))
        multimouse++;
    if((((mouseLeft)||(mouseMid)))&&( mouseRight^(mouseEvent->buttons() & Qt::RightButton) ))
        multimouse++;
    if(multimouse>0)
    {
        mouseEvent->accept(); return;
    }

    if( mouseLeft^(mouseEvent->buttons() & Qt::LeftButton) )
        mouseLeft=false;

    if( mouseMid^(mouseEvent->buttons() & Qt::MiddleButton) )
        mouseMid=false;

    if( mouseRight^(mouseEvent->buttons() & Qt::RightButton) )
    {
        if(!scene->IsMoved) callContext=true;
        mouseRight=false;
    }

    QGraphicsItem::mouseReleaseEvent(mouseEvent);


    /////////////////////////CONTEXT MENU:///////////////////////////////
    if((callContext)&&(!scene->contextMenuOpened))
    {
        if((!scene->lock_path)&&(!scene->DrawMode)&&(!isLocked))
        {
            scene->contextMenuOpened = true; //bug protector
            //Remove selection from non-bgo items
            if(!this->isSelected())
            {
                scene->clearSelection();
                this->setSelected(true);
            }

            this->setSelected(1);
            ItemMenu->clear();

            QAction *copyTile = ItemMenu->addAction(tr("Copy"));
            copyTile->deleteLater();
            QAction *cutTile = ItemMenu->addAction(tr("Cut"));
            cutTile->deleteLater();
            ItemMenu->addSeparator()->deleteLater();
            QAction *remove = ItemMenu->addAction(tr("Remove"));
            remove->deleteLater();

    QAction *selected = ItemMenu->exec(mouseEvent->screenPos());

            if(!selected)
            {
                #ifdef _DEBUG_
                WriteToLog(QtDebugMsg, "Context Menu <- NULL");
                #endif
                return;
            }

            if(selected==cutTile)
            {
                MainWinConnect::pMainWin->on_actionCut_triggered();
            }
            else
            if(selected==copyTile)
            {
                MainWinConnect::pMainWin->on_actionCopy_triggered();
            }
            else
            if(selected==remove)
            {
               scene->removeSelectedWldItems();
            }
        }

    }
}

void ItemPath::contextMenuEvent( QGraphicsSceneContextMenuEvent * event )
{
    QGraphicsItem::contextMenuEvent(event);
}


///////////////////MainArray functions/////////////////////////////
//void ItemPath::setLayer(QString layer)
//{
//    foreach(LevelLayers lr, scene->WldData->layers)
//    {
//        if(lr.name==layer)
//        {
//            pathData.layer = layer;
//            this->setVisible(!lr.hidden);
//            arrayApply();
//        break;
//        }
//    }
//}

void ItemPath::arrayApply()
{
    bool found=false;

    pathData.x = qRound(this->scenePos().x());
    pathData.y = qRound(this->scenePos().y());

    if(pathData.index < (unsigned int)scene->WldData->paths.size())
    { //Check index
        if(pathData.array_id == scene->WldData->paths[pathData.index].array_id)
        {
            found=true;
        }
    }

    //Apply current data in main array
    if(found)
    { //directlry
        scene->WldData->paths[pathData.index] = pathData; //apply current pathData
    }
    else
    for(int i=0; i<scene->WldData->paths.size(); i++)
    { //after find it into array
        if(scene->WldData->paths[i].array_id == pathData.array_id)
        {
            pathData.index = i;
            scene->WldData->paths[i] = pathData;
            break;
        }
    }
}

void ItemPath::removeFromArray()
{
    bool found=false;
    if(pathData.index < (unsigned int)scene->WldData->paths.size())
    { //Check index
        if(pathData.array_id == scene->WldData->paths[pathData.index].array_id)
        {
            found=true;
        }
    }

    if(found)
    { //directlry
        scene->WldData->paths.remove(pathData.index);
    }
    else
    for(int i=0; i<scene->WldData->paths.size(); i++)
    {
        if(scene->WldData->paths[i].array_id == pathData.array_id)
        {
            scene->WldData->paths.remove(i); break;
        }
    }
}

void ItemPath::setPathData(WorldPaths inD)
{
    pathData = inD;
}


QRectF ItemPath::boundingRect() const
{
    return imageSize;
}

void ItemPath::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if(animatorID<0)
    {
        painter->drawRect(QRect(0,0,1,1));
        return;
    }
    if(scene->animates_Paths.size()>animatorID)
        painter->drawPixmap(imageSize, scene->animates_Paths[animatorID]->image(), imageSize);
    else
        painter->drawRect(QRect(0,0,32,32));

    if(this->isSelected())
    {
        painter->setPen(QPen(QBrush(Qt::black), 2, Qt::SolidLine));
        painter->drawRect(1,1,imageSize.width()-2,imageSize.height()-2);
        painter->setPen(QPen(QBrush(Qt::blue), 2, Qt::DotLine));
        painter->drawRect(1,1,imageSize.width()-2,imageSize.height()-2);
    }
}

void ItemPath::setContextMenu(QMenu &menu)
{
    ItemMenu = &menu;
}

void ItemPath::setScenePoint(WldScene *theScene)
{
    scene = theScene;
}


////////////////Animation///////////////////

void ItemPath::setAnimator(long aniID)
{
    if(aniID<scene->animates_Paths.size())
    imageSize = QRectF(0,0,
                scene->animates_Paths[aniID]->image().width(),
                scene->animates_Paths[aniID]->image().height()
                );

    this->setData(9, QString::number(qRound(imageSize.width())) ); //width
    this->setData(10, QString::number(qRound(imageSize.height())) ); //height

    //WriteToLog(QtDebugMsg, QString("Tile Animator ID: %1").arg(aniID));

    animatorID = aniID;
}
