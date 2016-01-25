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

#include "audio.h"

TAudio::TAudio(QObject *parent)
{
    parent_playlist = parent;
    initTimer();
    setObjectName("TAudio");
}

TAudio::~TAudio()
{
    delete media_player;
    delete output;
}

bool TAudio::parse(QDomElement element)
{
    root_element   = element;
    setAttributes();
    setBaseParameters();
    return true;
}

showAudio TAudio::getMediaForShow()
{
    return show_audio;
}

void TAudio::beginPlay()
{
    setBeginEndTimer();
    return;
}

void TAudio::setDurationTimerBeforePlay()
{
    if (!hasDurAttribute()  && !end_timer->isActive()) // when end or dur is not specified use audio duration for simple duration
        connect(media_player, SIGNAL(stopped()), this, SLOT(finishedSimpleDuration()));
    play();
    emit startedMedia(parent_playlist, this);
    return;
}

// ====================  protected methods =================================
void TAudio::play()
{
    media_player->play();
    status = _playing;
}

void TAudio::stop()
{
    media_player->stop();
    status = _playing;
    return;
}


void TAudio::pause()
{
    media_player->pause();
    status = _playing;
}


void TAudio::setAttributes()
{
    setBaseMediaAttributes();
    show_audio.region      = region;
    output           = new QtAV::VideoOutput();
    media_player     = new QtAV::AVPlayer();
    connect(media_player, SIGNAL(stopped()), this, SLOT(emitfinished())); // 10s
    media_player->setRenderer(output);

    if (src.mid(0, 4) == "http")
        media_player->setFile(src);
    else // get relative paths
        media_player->setFile(index_path+src);

    return;
}

// ====================  private methods =================================


