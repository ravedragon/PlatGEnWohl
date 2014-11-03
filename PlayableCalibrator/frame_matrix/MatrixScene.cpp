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

#include "MatrixScene.h"
#include <QPixmap>
#include "../main/globals.h"
#include "../main/mw.h"

MatrixScene::MatrixScene(QObject *parent) : QGraphicsScene(parent)
{

}

void MatrixScene::draw()
{
    QGraphicsItem * sprite;
    sprite = addPixmap(QPixmap(MW::sprite()).scaled(QSize(595,595),Qt::KeepAspectRatio));
    sprite->setData(0, "sprite");
}
