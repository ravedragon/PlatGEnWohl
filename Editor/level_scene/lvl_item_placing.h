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

#ifndef LVL_ITEM_PLACING_H
#define LVL_ITEM_PLACING_H

#include "../file_formats/lvl_filedata.h"
class LvlPlacingItems
{
public:
    static LevelNPC npcSet;
    static long npcGrid;
    static long npcGfxOffsetX1;
    static long npcGfxOffsetX2;
    static long npcGfxOffsetY;

    static long itemW;
    static long itemH;

    static LevelBlock blockSet;

    static bool sizableBlock;
    static bool fillingMode;
    static bool lineMode;

    static bool overwriteMode;

    static LevelBGO bgoSet;
    static LevelPhysEnv waterSet;

    enum doorType{
        DOOR_Entrance=0,
        DOOR_Exit
    };


    static int c_offset_x;
    static int c_offset_y;

    static int doorType;
    static long doorArrayId;

    static int waterType;
    static int playerID;

    static int gridSz;
    static QPoint gridOffset;

    static QList<QPair<int, QVariant > > flags;
};

typedef QPair<int, QVariant > dataFlag;

#endif // ITEM_PLACING_H
