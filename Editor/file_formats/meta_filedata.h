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
#ifndef META_FILEDATA_H
#define META_FILEDATA_H

#include "../script/scriptholder.h"
#include <QtCore>

struct Bookmark{
    QString bookmarkName;
    qreal x;
    qreal y;
};


struct MetaData
{
    QVector<Bookmark> bookmarks;

    //only for level
    ScriptHolder* script;
};

#endif // META_FILEDATA_H
