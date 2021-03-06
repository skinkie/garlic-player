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
#include "video.h"

PlayerVideo::PlayerVideo(QObject *parent) : PlayerBaseMedia(parent)
{
    MediaDecoder.reset(new MediaPlayerWrapper(this));
    connect(MediaDecoder.data(), SIGNAL(finished()), this, SLOT(finished()));

    VideoWidget.reset(new MediaWidgetWrapper);
    MediaDecoder.data()->setVideoOutput(VideoWidget.data());
}

PlayerVideo::~PlayerVideo()
{
    MediaDecoder.reset();
    VideoWidget.reset();
}

void PlayerVideo::init(BaseMedia *media)
{
   SmilMedia  = media;
   QString path = SmilMedia->getLoadablePath();
   if (isFileExists(path))
   {
       MediaDecoder.data()->load(path);
       MediaDecoder.data()->play();
       TVideo  *MyParser = qobject_cast<TVideo *>(media);
       MediaDecoder.data()->setVolume(MyParser->getSoundLevel());
       if (SmilMedia->getLogContentId() != "")
           setStartTime();
   }
}

void PlayerVideo::deinit()
{
    MediaDecoder.data()->stop();
    MediaDecoder.data()->unload();
    if (SmilMedia->getLogContentId() != "")
        qInfo(PlayLog).noquote() << createPlayLogXml();
}

void PlayerVideo::changeSize(int w, int h)
{
    Q_UNUSED(w)
    Q_UNUSED(h)
    if (!exists)
        return;

    QString fit = SmilMedia->getFit();
    if (fit == "fill")
        VideoWidget.data()->setAspectRatioFill();
    else if (fit == "meet")
        VideoWidget.data()->setAspectRatioMeet();
    else if (fit == "meetbest")
        VideoWidget.data()->setAspectRatioMeetBest();
}

QWidget *PlayerVideo::getView()
{
    if (!exists)
        return Q_NULLPTR;
    return VideoWidget.data()->getVideoWidget();
}

void PlayerVideo::finished()
{
   SmilMedia->finishedSimpleDuration();
}
