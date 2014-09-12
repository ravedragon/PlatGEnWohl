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

#include "file_formats.h"

namespace smbx64Format
{
    QRegExp isint = QRegExp("\\d+");     //Check "Is Numeric"
    QRegExp issint = QRegExp("^[\\-0]?\\d*$");     //Check "Is signed Numeric"
    QRegExp issfloat = QRegExp("^[\\-]?(\\d*)?[\\(.|,)]?\\d*[Ee]?[\\-\\+]?\\d*$");     //Check "Is signed Float Numeric"
    QRegExp qstr = QRegExp("^\"(?:[^\"\\\\]|\\\\.)*\"$");
    QRegExp boolwords = QRegExp("^(#TRUE#|#FALSE#)$");
    QRegExp booldeg = QRegExp("^(1|0)$");
}

// /////////////Validators///////////////
//returns TRUE on wrong data

bool SMBX64::Int(QString in) // UNSIGNED INT
{
    using namespace smbx64Format;
    return !isint.exactMatch(in);
}

bool SMBX64::sInt(QString in) // SIGNED INT
{
    using namespace smbx64Format;
    return !issint.exactMatch(in);
}

bool SMBX64::sFloat(QString in) // SIGNED FLOAT
{
    using namespace smbx64Format;
    return !issfloat.exactMatch(in);
}

bool SMBX64::qStr(QString in) // QUOTED STRING
{
    using namespace smbx64Format;
    return !qstr.exactMatch(in);
}

bool SMBX64::wBool(QString in) //Worded BOOL
{
    using namespace smbx64Format;
    return !boolwords.exactMatch(in);
}

bool SMBX64::dBool(QString in) //Digital BOOL
{
    using namespace smbx64Format;
    return !booldeg.exactMatch(in);
}


//Convert from string to internal data
bool SMBX64::wBoolR(QString in)
{
    using namespace smbx64Format;
    return ((in=="#TRUE#")?true:false);
}


//SMBX64 parameter string generators
QString SMBX64::IntS(long input)
{  return QString::number(input)+"\n"; }

QString SMBX64::FloatS(float input)
{  return QString::number(input)+"\n"; }

QString SMBX64::BoolS(bool input)
{  return QString( (input)?"#TRUE#":"#FALSE#" )+"\n"; }

QString SMBX64::qStrS(QString input)
{ return QString("\"%1\"\n").arg(input.simplified().remove('\"') ); }
