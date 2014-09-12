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

#ifndef FILE_FORMATS_H
#define FILE_FORMATS_H

#include <QRegExp>
#include <QString>
#include <QFile>


#include "lvl_filedata.h"
#include "npc_filedata.h"
#include "wld_filedata.h"

#include "../data_configs/obj_npc.h"


//SMBX64 standard data
class SMBX64
{
public:
    SMBX64() {}

    // /////////////Validators///////////////
    //returns TRUE on wrong data
    static bool Int(QString in); // UNSIGNED INT
    static bool sInt(QString in); // SIGNED INT
    static bool sFloat(QString in); // SIGNED FLOAT
    static bool qStr(QString in); // QUOTED STRING
    static bool wBool(QString in); //Worded BOOL
    static bool dBool(QString in); //Worded BOOL

    //Convert from string to internal data
    static bool wBoolR(QString in);

    //SMBX64 parameter string generators
    static QString IntS(long input);
    static QString BoolS(bool input);
    static QString qStrS(QString input);
    static QString FloatS(float input);
};

class PGEFile
{
public:
    PGEFile() {}

    // /////////////Validators///////////////
    //returns TRUE on wrong data
    //static bool Int(QString in); // UNSIGNED INT
    //static bool sInt(QString in); // SIGNED INT

    //static bool Float(QString in); // UNSIGNED FLOAT
    //static bool sFloat(QString in); // SIGNED FLOAT

    //static bool Bool(QString in); //Digit BOOL

    //static bool qStr(QString in); // QUOTED STRING
    //static bool heStr(QString in); // Hex Encoded String

    //static bool intArray(QString in); //Integer array
    //static bool strArray(QString in); //String array
    //static bool BoolArray(QString in); //Bool array

    static bool IsQStr(QString in);
    static bool IsHex(QString in);
    static bool IsIntU(QString in);
    static bool IsIntS(QString in);
    static bool IsFloat(QString in);
    static bool IsBool(QString in);
    static bool IsBoolArray(QString in);
    static bool IsIntArray(QString in);
    static bool IsStringArray(QString in);

    //Split string into data values
    static QList<QStringList> splitDataLine(QString src_data, bool *valid = 0);

    //PGE Extended File parameter string generators
    static QString IntS(long input);
    static QString BoolS(bool input);
    static QString FloatS(float input);
    static QString qStrS(QString input);
    static QString hStrS(QString input);
    static QString strArrayS(QStringList input);
    static QString intArrayS(QList<int > input);
    static QString BoolArrayS(QList<bool > input);

    static QString X2STR(QString input);

    static QString escapeStr(QString input);
    static QString restoreStr(QString input);
    static QString encodeEscape(QString input);
    static QString decodeEscape(QString input);

    static QString value(QString marker, QString data);

};


class FileFormats
{
public:
    //File format read functions

    static LevelData dummyLvlDataArray();                   //Create new
    // SMBX64 LVL File
    static LevelData ReadLevelFile(QFile &inf);             //read
    static QString WriteSMBX64LvlFile(LevelData FileData);  //write
    // PGE Extended Level File
    static LevelData ReadExtendedLevelFile(QFile &inf);
    static QString WriteExtendedLvlFile(LevelData FileData); //Write

    // Lvl Data
    static LevelNPC dummyLvlNpc();
    static LevelDoors dummyLvlDoor();
    static LevelBlock dummyLvlBlock();
    static LevelBGO dummyLvlBgo();
    static LevelPhysEnv dummyLvlPhysEnv();
    static LevelLayers dummyLvlLayer();
    static LevelEvents dummyLvlEvent();
    static PlayerPoint dummyLvlPlayerPoint(int id=0);
    static LevelSection dummyLvlSection();

    //Wld Data
    static WorldTiles dummyWldTile();
    static WorldScenery dummyWldScen();
    static WorldPaths dummyWldPath();
    static WorldLevels dummyWldLevel();
    static WorldMusic dummyWldMusic();

    // SMBX64 NPC.TXT File
    static NPCConfigFile ReadNpcTXTFile(QFile &inf, bool IgnoreBad=false); //read
    static QString WriteNPCTxtFile(NPCConfigFile FileData);                //write

    static NPCConfigFile CreateEmpytNpcTXTArray();
    static obj_npc mergeNPCConfigs(obj_npc &global, NPCConfigFile &local, QSize captured=QSize(0,0));

    // SMBX64 WLD File
    static WorldData ReadWorldFile(QFile &inf);             //read
    static QString WriteSMBX64WldFile(WorldData FileData);  //Write
    static WorldData dummyWldDataArray();                   //Create new


    //common
    static void BadFileMsg(QString fileName_DATA, int str_count, QString line);
    static QString removeQuotes(QString str); // Remove quotes from begin and end

};

#endif // FILE_FORMATS_H
