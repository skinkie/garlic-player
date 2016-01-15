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

TSeq::TSeq(QObject * parent)
{
    parent_playlist = parent;
    setObjectName("TSeq");
}

bool TSeq::parse(QDomElement element)
{
    actual_element = element; // must set to get inherited Attributed
    setBaseAttributes();
    if (element.hasChildNodes())
    {
        actual_element = element.firstChildElement();
        if (actual_element.tagName() == "metadata")
            doMetaData();
        setPlaylist();
        iterator       = ar_playlist.begin();
        actual_element = *iterator;
    }
    return true;
}

void TSeq::beginPlay()
{
    setTimedStart();
    return;
}

void TSeq::play()
{
    if (setTimedEnd() || ar_playlist.length() > 0)
    {
        status = _playing;
        reactByTag();
        emit started(parent_playlist, this);
    }
    else // when end or duration is not specified or no child elements stop imediately
        emitfinished();
    return;
}

void TSeq::emitfinished()
{

}

void TSeq::doMetaData()
{
    QDomNodeList childs = actual_element.childNodes();
    QDomElement  element;
    int          length = childs.length();
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() == "meta" && element.hasAttribute("name") && element.hasAttribute("content"))
        {
            if (element.attribute("name") == "adapi:pickingAlgorithm")
                pickingAlgorithm = element.attribute("content");
            else if (element.attribute("name") == "adapi:pickingBehavior")
                pickingBehavior = element.attribute("content");
            else if (element.attribute("name") == "adapi:pickNumber")
                pickNumber = element.attribute("content").toInt();
        }
    }
    return;
}

bool TSeq::next()
{
    bool ret = false;
    iterator++; // inc iterator first only when inc result smaller than  .end()
    if (iterator < ar_playlist.end())  // cause .end() pointing to the imaginary item after the last item in the vector
    {
        actual_element = *iterator;
        ret            = true;
        reactByTag();
    }
    else // end check if repeat value is indefined or Limnit not reached
    {
        if (checkRepeatCountStatus())
        {
            iterator       = ar_playlist.begin();
            actual_element = *iterator;
            ret            = true;
            reactByTag();
        }
        else
        {
            // ToDo: get Info about end from TTiming. may bee is it secessary not end the playlist.
            // cause dur or end-Attribute exeeds the length of all the sequential played media
            emit finished(parent_playlist, this);
        }
    }
    return ret;
}

bool TSeq::previous()
{
    return false;
}

void TSeq::setPlaylist()
{
    QDomNodeList childs = actual_element.parentNode().childNodes();
    int          length = childs.length();
    QDomElement  element;
    for (int i = 0; i < length; i++)
    {
        element = childs.item(i).toElement();
        if (element.tagName() != "metadata")
            ar_playlist.append(element);
    }
    if (pickingAlgorithm == "shuffle" && pickingBehavior == "pickN" && pickNumber > 0)
    {
        randomizePlaylist();
        random = true;
        internal_pick = pickNumber;
    }
}

void TSeq::randomizePlaylist()
{
    // Knuth shuffle
    int length = ar_playlist.length();
    for (int i = length - 1; i > 0; --i)
    {
        int i2 = qrand() % (i + 1);
        std::swap(ar_playlist[i], ar_playlist[i2]);
    }
    // Let only pickNumber items in List
    for (int i = 0; i < (length-pickNumber); i++)
        ar_playlist.removeLast();
}
