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


#include "piecesmodel.h"

#include <QIcon>
#include <QMimeData>

PiecesModel::PiecesModel(dataconfigs* conf, PieceType pieceType, int pieceSize, QObject *parent)
    : QAbstractListModel(parent), m_PieceSize(pieceSize), m_conf(conf), m_type(pieceType)
{
}

QVariant PiecesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DecorationRole)
        return QIcon(pixmaps.value(index.row()).scaled(m_PieceSize, m_PieceSize,
                         Qt::KeepAspectRatio, Qt::SmoothTransformation));
    else if (role == Qt::DisplayRole)
        return pixmapNames.value(index.row());
    else if (role == Qt::UserRole)
        return pixmaps.value(index.row());
    else if (role == Qt::UserRole + 1)
        return pixmapId.value(index.row());

    return QVariant();
}

void PiecesModel::addPiece(const int &index)
{
    beginInsertRows(QModelIndex(), pixmaps.size(), pixmaps.size());

    if(m_type==LEVELPIECE_BLOCK)
    {
        pixmapNames.insert(pixmaps.size(), m_conf->main_block[index].name);
        pixmaps.insert(pixmaps.size(), m_conf->main_block[index].image.copy(0,0,m_conf->main_block[index].image.width(),
                                                                            qRound(qreal(m_conf->main_block[index].image.height())/ m_conf->main_block[index].frames) ));
        pixmapId.insert(pixmaps.size(), m_conf->main_block[index].id);
    }
    else
    if(m_type==LEVELPIECE_BGO)
    {
        pixmapNames.insert(pixmaps.size(), m_conf->main_bgo[index].name);
        pixmaps.insert(pixmaps.size(), m_conf->main_bgo[index].image.copy(0,0,m_conf->main_bgo[index].image.width(),
                                                                             qRound(qreal(m_conf->main_bgo[index].image.height()) / m_conf->main_bgo[index].frames)));
        pixmapId.insert(pixmaps.size(), m_conf->main_bgo[index].id);
    }
    else
    if(m_type==LEVELPIECE_NPC)
    {
        pixmapNames.insert(pixmaps.size(), m_conf->main_npc[index].name);
        pixmaps.insert(pixmaps.size(), m_conf->main_npc[index].image.copy(0,0, m_conf->main_npc[index].image.width(), m_conf->main_npc[index].gfx_h ));
        pixmapId.insert(pixmaps.size(), m_conf->main_npc[index].id);
    }
    else
    if(m_type==WORLDPIECE_TILE)
    {
        pixmapNames.insert(pixmaps.size(), QString("tile-%1").arg(index));
        pixmaps.insert(pixmaps.size(), m_conf->main_wtiles[index].image.copy(0,0,m_conf->main_wtiles[index].image.width(),
                                                                             qRound(qreal(m_conf->main_wtiles[index].image.height()) / m_conf->main_wtiles[index].frames)));
        pixmapId.insert(pixmaps.size(), m_conf->main_wtiles[index].id);
    }
    else
    if(m_type==WORLDPIECE_PATH)
    {
        pixmapNames.insert(pixmaps.size(), QString("path-%1").arg(index));
        pixmaps.insert(pixmaps.size(), m_conf->main_wpaths[index].image.copy(0,0,m_conf->main_wpaths[index].image.width(),
                                                                             qRound(qreal(m_conf->main_wpaths[index].image.height()) / m_conf->main_wpaths[index].frames)));
        pixmapId.insert(pixmaps.size(), m_conf->main_wpaths[index].id);
    }
    else
    if(m_type==WORLDPIECE_SCENERY)
    {
        pixmapNames.insert(pixmaps.size(), QString("scenery-%1").arg(index));
        pixmaps.insert(pixmaps.size(), m_conf->main_wscene[index].image.copy(0,0,m_conf->main_wscene[index].image.width(),
                                                                             qRound(qreal(m_conf->main_wscene[index].image.height()) / m_conf->main_wscene[index].frames)));
        pixmapId.insert(pixmaps.size(), m_conf->main_wscene[index].id);
    }
    else
    if(m_type==WORLDPIECE_LEVEL)
    {
        pixmapNames.insert(pixmaps.size(), QString("level-%1").arg(index));
        pixmaps.insert(pixmaps.size(), m_conf->main_wlevels[index].image.copy(0,0,m_conf->main_wlevels[index].image.width(),
                                                                             qRound(qreal(m_conf->main_wlevels[index].image.height()) / m_conf->main_wlevels[index].frames)));
        pixmapId.insert(pixmaps.size(), m_conf->main_wlevels[index].id);
    }
    endInsertRows();
}

QString PiecesModel::getMimeType() const
{
    switch (m_type) {
    case LEVELPIECE_BLOCK: return QString("text/x-pge-piece-block");
    case LEVELPIECE_BGO: return QString("text/x-pge-piece-bgo");
    case LEVELPIECE_NPC: return QString("text/x-pge-piece-npc");
    case WORLDPIECE_TILE: return QString("text/x-pge-piece-tile");
    case WORLDPIECE_PATH: return QString("text/x-pge-piece-path");
    case WORLDPIECE_SCENERY: return QString("text/x-pge-piece-scenery");
    case WORLDPIECE_LEVEL: return QString("text/x-pge-piece-level");
    default:
        break;
    }
    return QString("image/x-pge-piece");
}

Qt::ItemFlags PiecesModel::flags(const QModelIndex &index) const
{
    if (index.isValid())
        return (QAbstractListModel::flags(index)|Qt::ItemIsDragEnabled);

    return Qt::ItemIsDropEnabled;
}

bool PiecesModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if (parent.isValid())
        return false;

    if (row >= pixmaps.size() || row + count <= 0)
        return false;

//    int beginRow = qMax(0, row);
//    int endRow = qMin(row + count - 1, pixmaps.size() - 1);

//    beginRemoveRows(parent, beginRow, endRow);

//    while (beginRow <= endRow) {
//        pixmaps.removeAt(beginRow);
//        pixmapNames.removeAt(beginRow);
//        ++beginRow;
//    }

//    endRemoveRows();
    return false;
}

QStringList PiecesModel::mimeTypes() const
{
    QStringList types;
    types << getMimeType().toStdString().c_str();
    return types;
}

QMimeData *PiecesModel::mimeData(const QModelIndexList &indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            int id = data(index, Qt::UserRole+1).toInt();
            stream << id;
        }
    }

    mimeData->setData(getMimeType(), encodedData);
    return mimeData;
}

bool PiecesModel::dropMimeData(const QMimeData *data, Qt::DropAction action,
                               int /*row*/, int column, const QModelIndex &parent)
{
    Q_UNUSED(parent);

    if (!data->hasFormat(getMimeType()))
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    if (column > 0)
        return false;

//    int endRow;

//    if (!parent.isValid()) {
//        if (row < 0)
//            endRow = pixmaps.size();
//        else
//            endRow = qMin(row, pixmaps.size());
//    } else {
//        endRow = parent.row();
//    }

//    QByteArray encodedData = data->data("image/x-pge-piece");
//    QDataStream stream(&encodedData, QIODevice::ReadOnly);

//    while (!stream.atEnd()) {
//        QPixmap pixmap;
//        QPoint location;
//        stream >> pixmap >> location;

//        beginInsertRows(QModelIndex(), endRow, endRow);
//        pixmaps.insert(endRow, pixmap);
//        endInsertRows();

//        ++endRow;
//    }

    return false;
}

int PiecesModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    else
        return pixmaps.size();
}

Qt::DropActions PiecesModel::supportedDropActions() const
{
    return Qt::CopyAction | Qt::MoveAction;
}

