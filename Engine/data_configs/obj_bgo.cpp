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

#include "config_manager.h"

/*****Level BGO************/
QVector<obj_bgo >     ConfigManager::lvl_bgo;
QMap<long, obj_bgo>   ConfigManager::lvl_bgo_indexes;
CustomDirManager ConfigManager::Dir_BGO;
QVector<SimpleAnimator *> ConfigManager::Animator_BGO;
/*****Level BGO************/

bool ConfigManager::loadLevelBGO()
{
    unsigned int i;

    obj_bgo sbgo;
    unsigned long bgo_total=0;
    QString bgo_ini = config_dir + "lvl_bgo.ini";

    if(!QFile::exists(bgo_ini))
    {
        addError(QString("ERROR LOADING lvl_bgo.ini: file does not exist"), QtCriticalMsg);
        return false;
    }

    QSettings bgoset(bgo_ini, QSettings::IniFormat);
    bgoset.setIniCodec("UTF-8");

    lvl_bgo.clear();   //Clear old
    lvl_bgo_indexes.clear();

    bgoset.beginGroup("background-main");
        bgo_total = bgoset.value("total", "0").toInt();
        total_data +=bgo_total;
    bgoset.endGroup();


    for(i=1; i<=bgo_total; i++)
    {

        sbgo.isInit = false;
        sbgo.image = NULL;
        sbgo.textureArrayId = 0;
        sbgo.animator_ID = 0;

        bgoset.beginGroup( QString("background-"+QString::number(i)) );

            sbgo.name = bgoset.value("name", "").toString();

                if(sbgo.name=="")
                {
                    addError(QString("BGO-%1 Item name isn't defined").arg(i));
                    goto skipBGO;
                }
            sbgo.group = bgoset.value("group", "_NoGroup").toString();
            sbgo.category = bgoset.value("category", "_Other").toString();
            //sbgo.grid = bgoset.value("grid", default_grid).toInt();

            {
                QString tmpStr=bgoset.value("view", "background").toString();

                if(tmpStr=="foreground2")
                    sbgo.view = 2;
                else
                if(tmpStr=="foreground")
                    sbgo.view = 1;
                else
                if(tmpStr=="background")
                    sbgo.view = 0;
                else
                if(tmpStr=="background2")
                    sbgo.view = -1;
                else
                    sbgo.view = 0;
            }

            sbgo.offsetX = bgoset.value("offset-x", "0").toInt();
            sbgo.offsetY = bgoset.value("offset-y", "0").toInt();
            sbgo.zOffset = bgoset.value("z-offset", "0").toInt();
            imgFile = bgoset.value("image", "").toString();
            sbgo.image_n = imgFile;
            if( (imgFile!="") )
            {
                tmp = imgFile.split(".", QString::SkipEmptyParts);
                if(tmp.size()==2)
                    imgFileM = tmp[0] + "m." + tmp[1];
                else
                    imgFileM = "";
                sbgo.mask_n = imgFileM;
            }
            else
            {
                addError(QString("BGO-%1 Image filename isn't defined").arg(i));
                goto skipBGO;
            }
            sbgo.climbing = (bgoset.value("climbing", "0").toString()=="1");
            sbgo.animated = (bgoset.value("animated", "0").toString()=="1");
            sbgo.frames = bgoset.value("frames", "1").toInt();
            sbgo.framespeed = bgoset.value("frame-speed", "125").toInt();

            sbgo.frame_h = 0;//(sbgo.animated? qRound(qreal(sbgo.image.height())/sbgo.frames) : sbgo.image.height());

            sbgo.display_frame = bgoset.value("display-frame", "0").toInt();
            sbgo.id = i;
            lvl_bgo.push_back(sbgo);

            //Add to Index
            lvl_bgo_indexes[lvl_bgo.last().id] = lvl_bgo.last();

        skipBGO:
        bgoset.endGroup();

        if( bgoset.status() != QSettings::NoError )
        {
            addError(QString("ERROR LOADING lvl_bgo.ini N:%1 (bgo-%2)").arg(bgoset.status()).arg(i), QtCriticalMsg);
        }
    }

    if((unsigned int)lvl_bgo.size()<bgo_total)
    {
        addError(QString("Not all BGOs loaded! Total: %1, Loaded: %2").arg(bgo_total).arg(lvl_bgo.size()));
    }
    return true;
}
