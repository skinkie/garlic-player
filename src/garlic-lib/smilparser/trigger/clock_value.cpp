/*************************************************************************************
    garlic-player: SMIL Player for Digital Signage
    Copyright (C) 2016 Nikolaos Saghiadinos <ns@smil-control.com>
    This file is part of the garlic-player source code

    This program is free software: you can redistribute it and/or  modify
    it under the terms of the GNU Affero General Public License, version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*************************************************************************************/

#include "clock_value.h"

ClockValue::ClockValue(QObject *parent) : QObject(parent)
{
    Q_UNUSED(parent);
}

void ClockValue::parse(QString attr_value)
{
    ms     = calculateMilliseconds(attr_value);
}

qint64 ClockValue::getNextTimerTrigger()
{
    return ms;
}

/**
 * @brief TTiming::calculateDuration converts clock or timecount values to milliseconds
 *
 * @param  dur
 * @return
 */
qint64 ClockValue::calculateMilliseconds(QString dur)
{
    int length = dur.length();
    int duration = 0;
    if (dur.contains(QChar(':'))) // Full clock or partial clock values
    {
        QStringList ar = dur.split(QChar(':'));
        if (ar.size() == 3) // Full
            duration = (ar[0].toInt()*3600000) + (ar[1].toInt()*60000) + (ar[2].toFloat()*1000);
        else if (ar.size() == 2) // partial
            duration = (ar[0].toInt()*60000) + (ar[1].toFloat()*1000);
        // size 1 not possible here, cause no ':' would be treated as timecount
    }
    else // Timecount values
    {
        if (dur[length-1] == QChar('s'))
            duration = dur.mid(0, length-1).toFloat()*1000;
        else if (dur[length-1] == QChar('h'))
            duration = dur.mid(0, length-1).toFloat()*3600000;
        else if (dur.contains("min"))
            duration = dur.mid(0, length-3).toFloat()*60000;
        else if (dur.contains("ms"))
             duration = dur.mid(0, length-2).toInt();
        else if (dur=="indefinite")
            duration = -1;
        else if (dur=="media")
            duration = -1;
        else // no marker means seconds
            duration = dur.toFloat()*1000;
    }
    return duration;
}
