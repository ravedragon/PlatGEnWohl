#include "mode_line.h"

#include "../lvlscene.h"
#include "../../common_features/mainwinconnect.h"
#include "../../common_features/item_rectangles.h"
#include "../lvl_item_placing.h"

#include "../item_bgo.h"
#include "../item_block.h"
#include "../item_npc.h"
#include "../item_water.h"
#include "../item_playerpoint.h"
#include "../item_door.h"

LVL_ModeLine::LVL_ModeLine(QGraphicsScene *parentScene, QObject *parent)
    : EditMode("Line", parentScene, parent)
{
    drawStartPos = QPointF(0,0);
}

LVL_ModeLine::~LVL_ModeLine()
{}

void LVL_ModeLine::set()
{
    if(!scene) return;
    LvlScene *s = dynamic_cast<LvlScene *>(scene);

    s->EraserEnabled=false;
    s->PasteFromBuffer=false;
    s->DrawMode=true;
    s->disableMoveItems=false;

    s->clearSelection();
    s->resetResizers();

    s->_viewPort->setInteractive(true);
    s->_viewPort->setCursor(Qt::CrossCursor);
    s->_viewPort->setDragMode(QGraphicsView::NoDrag);
    s->_viewPort->setRenderHint(QPainter::Antialiasing, true);
    s->_viewPort->viewport()->setMouseTracking(true);
}

void LVL_ModeLine::mousePress(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!scene) return;
    LvlScene *s = dynamic_cast<LvlScene *>(scene);

    if( mouseEvent->buttons() & Qt::RightButton )
    {
        item_rectangles::clearArray();
        MainWinConnect::pMainWin->on_actionSelect_triggered();
        dontCallEvent = true;
        return;
    }

    s->last_block_arrayID=s->LvlData->blocks_array_id;
    s->last_bgo_arrayID=s->LvlData->bgo_array_id;
    s->last_npc_arrayID=s->LvlData->npc_array_id;

    WriteToLog(QtDebugMsg, QString("Line mode %1").arg(s->EditingMode));

    if(s->cursor)
    {
        drawStartPos = QPointF(s->applyGrid( mouseEvent->scenePos().toPoint()-
                                          QPoint(LvlPlacingItems::c_offset_x,
                                                 LvlPlacingItems::c_offset_y),
                                          LvlPlacingItems::gridSz,
                                          LvlPlacingItems::gridOffset));
        //cursor->setPos( drawStartPos );
        s->cursor->setVisible(true);

        QPoint hw = s->applyGrid( mouseEvent->scenePos().toPoint()-
                               QPoint(LvlPlacingItems::c_offset_x,
                                      LvlPlacingItems::c_offset_y),
                               LvlPlacingItems::gridSz,
                               LvlPlacingItems::gridOffset);
        dynamic_cast<QGraphicsLineItem *>(s->cursor)->setLine(drawStartPos.x(), drawStartPos.y(),
                                          hw.x(), hw.y());
    }
}

void LVL_ModeLine::mouseMove(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(!scene) return;
    LvlScene *s = dynamic_cast<LvlScene *>(scene);

    if(!LvlPlacingItems::layer.isEmpty() && LvlPlacingItems::layer!="Default")
        s->setMessageBoxItem(true, mouseEvent->scenePos(), LvlPlacingItems::layer + ", " +
                     QString::number( mouseEvent->scenePos().toPoint().x() ) + "x" +
                     QString::number( mouseEvent->scenePos().toPoint().y() )
                      );
    else
        s->setMessageBoxItem(false);

        if(s->cursor)
        {
            if(s->cursor->isVisible())
            {
                QPoint hs = s->applyGrid( mouseEvent->scenePos().toPoint()-
                                       QPoint(LvlPlacingItems::c_offset_x,
                                              LvlPlacingItems::c_offset_y),
                                       LvlPlacingItems::gridSz,
                                       LvlPlacingItems::gridOffset);

                QLineF sz = item_rectangles::snapLine(QLineF(drawStartPos.x(),drawStartPos.y(), (qreal)hs.x(), (qreal)hs.y()),
                                                     QSizeF((qreal)LvlPlacingItems::itemW, (qreal)LvlPlacingItems::itemH) );

                QPoint hw = s->applyGrid( sz.p2().toPoint(),
                                    LvlPlacingItems::gridSz,
                                    LvlPlacingItems::gridOffset);

                sz.setP2(QPointF((qreal)hw.x(),(qreal)hw.y()));

                dynamic_cast<QGraphicsLineItem *>(s->cursor)->setLine(sz);

                item_rectangles::drawLine(s, sz,
                       QSize(LvlPlacingItems::itemW, LvlPlacingItems::itemH)
                                            );
            }
        }

}

void LVL_ModeLine::mouseRelease(QGraphicsSceneMouseEvent *mouseEvent)
{
    Q_UNUSED(mouseEvent);

    if(!scene) return;
    LvlScene *s = dynamic_cast<LvlScene *>(scene);

    if(s->cursor)
    {
        WriteToLog(QtDebugMsg, "Line tool -> Placing items");
        s->placeItemsByRectArray();

        s->Debugger_updateItemList();

        s->LvlData->modified = true;
        s->cursor->hide();
    }
}

void LVL_ModeLine::keyPress(QKeyEvent *keyEvent)
{
    Q_UNUSED(keyEvent);
}

void LVL_ModeLine::keyRelease(QKeyEvent *keyEvent)
{
    Q_UNUSED(keyEvent);
    switch(keyEvent->key())
    {
        case (Qt::Key_Escape):
            item_rectangles::clearArray();
            MainWinConnect::pMainWin->on_actionSelect_triggered();
            break;
        default:
            break;
    }
}
