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

#ifndef TILESETITEMBUTTON_H
#define TILESETITEMBUTTON_H

#include <QFrame>
#include "../data_configs/data_configs.h"
#include "../defines.h"

class TilesetItemButton : public QFrame
{
    Q_OBJECT

public:
    explicit TilesetItemButton(dataconfigs* conf, QWidget *parent = 0);

    dataconfigs *config() const;
    void setConfig(dataconfigs *config);
    void applyItem(const int &i, const int &id, const int &width = -1, const int &height = -1);
    void applySize(const int &width, const int &height);

signals:

public slots:

protected:
    void paintEvent(QPaintEvent* ev);
    void mousePressEvent(QMouseEvent*);
    void mouseReleaseEvent(QMouseEvent*);

private:
    dataconfigs* m_config;
    QPixmap m_drawItem;
};

#endif // TILESETITEMBUTTON_H