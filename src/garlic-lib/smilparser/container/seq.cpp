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

#include "seq.h"

TSeq::TSeq(TContainer *pc, QObject *parent) : TContainer(parent)
{
    parent_container = pc;
    setObjectName("TSeq");
}

TSeq::~TSeq()
{
}

bool TSeq::parse(QDomElement element)
{
    root_element   = element;
    parseTimingAttributes();
    if (element.hasChildNodes())
    {
        active_element = element.firstChildElement();
        traverseChilds();
        if (active_element.tagName() == "metadata")
        {
            MyShuffle = new TShuffle(childs_list, this);
            MyShuffle->parse(active_element);
        }

    }
    return true;
}

void TSeq::preload()
{
    for (QList<QDomElement>::iterator i = childs_list.begin(); i != childs_list.end(); i++)
    {
        active_element        = *i;
        emitPreLoad();
    }
}

void TSeq::next(BaseTimings *ended_element)
{
    childEnded(ended_element);

    if (canGetNextItem())  // cause .end() pointing to the imaginary item after the last item in the list
    {
        active_element = getNextItem();
        emitFoundElement();
    }
    else // end check if repeat value is indefined or Limnit not reached
    {
        handlePossibleRepeat();
    }
}

bool TSeq::isChildPlayable(BaseTimings *element)
{
    qDebug(Develop) << getID() << "is ChildPlayable in seq";
    childStarted(element);
    return true;
}

void TSeq::play()
{
    if (childs_list.size() > 0)
    {
        if (MyShuffle == Q_NULLPTR)
        {
            childs_list_iterator  = childs_list.begin();
            active_element        = getNextItem();
        }
        else
        {
            active_element = MyShuffle->getNextItem();
        }
        emitFoundElement();
    }
    qDebug(Develop) << getID() << "played";
    status = _playing;
}

void TSeq::resume()
{
    qDebug(Develop) << getID() << "plaused";
    status = _paused;
}

void TSeq::stop()
{
    qDebug(Develop) << getID() << "stopped";
    status = _stopped;
}

void TSeq::pause()
{
    status = _paused;
}

void TSeq::setDurationTimerBeforePlay()
{
    if (startDurTimer() || isEndTimerActive() || childs_list.size() > 0)
    {
        resetInternalRepeatCount();
        if (!is_resumed)
            emit startedContainer(parent_container, this);
    }
    else
    {
        initInternalTimer();
    }
}

bool TSeq::canGetNextItem()
{
    if (MyShuffle == Q_NULLPTR)
    {
        return (childs_list_iterator != childs_list.end());
    }
    else
    {
        return MyShuffle->canGetNextItem();
    }
}

QDomElement TSeq::getNextItem()
{
    QDomElement ret;
    if (MyShuffle == Q_NULLPTR)
    {
        if (childs_list_iterator == childs_list.end())
        {
            childs_list_iterator = childs_list.begin();
        }
        ret = *childs_list_iterator;
        childs_list_iterator++;
    }
    else
    {
        ret = MyShuffle->getNextItem();
    }
    return ret;
}

void TSeq::handlePossibleRepeat()
{
    if (checkRepeatCountStatus())
    {
        qDebug() << getID() << "Repeats: " << count++;
        play();
    }
    else
    {
        finishedActiveDuration();
    }
}

void TSeq::traverseChilds()
{
    QDomNodeList childs = root_element.childNodes();
    QDomElement  element;
    for (int i = 0; i < childs.length(); i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "metadata" && element.tagName() != "") // e.g. comments
        {
            childs_list.append(element);
        }
    }
    childs_list_iterator = childs_list.begin();
}

