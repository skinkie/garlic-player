#include "player_base_media.h"

PlayerBaseMedia::PlayerBaseMedia(QQmlComponent *mc, QString r_id, QObject *parent) : QObject(parent)
{
    Q_UNUSED(mc);
    setRegionId(r_id);
}

QString PlayerBaseMedia::getRegionId() const
{
    return region_id;
}

void PlayerBaseMedia::setRegionId(const QString &value)
{
    region_id = value;
}

void PlayerBaseMedia::setStartTime()
{
    start_time = MyLogger.getCurrentIsoDateTime();;
}

QString PlayerBaseMedia::createPlayLogXml()
{
    if (start_time == "")
        return "";

    QString xml = MyLogger.createPlayLogEntry(start_time, MyMedia->getLogContentId());

    // set times to Zero
    start_time = "";
    return xml;
}


bool PlayerBaseMedia::load(QQuickItem *item)
{
    QString source = MyMedia->getLoadablePath();
    bool ret = false;
    if (isFileExists(source))
    {
        ret = true;
        item->setProperty("source", "file:"+source);
    }
    else
    {
        QStringList list;
        list  << "resourceURI: " << MyMedia->getSrc();
        qCritical(MediaPlayer) << MyLogger.createEventLogMetaData("MEDIA_NOT_AVAILABLE", list);
    }
    return ret;
}

QQuickItem *PlayerBaseMedia::createMediaItem(QQmlComponent *mc, QString str)
{
    mc->setData(str.toUtf8(), QUrl());
    if (mc->isError())
    {
        qCritical(MediaControl) << mc->errorString();
        return Q_NULLPTR;
    }

    return qobject_cast<QQuickItem *>(mc->create());
}


bool PlayerBaseMedia::isFileExists(QString path)
{
    return QFileInfo::exists(path);
}
