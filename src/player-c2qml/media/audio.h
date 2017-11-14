#ifndef AUDIO_H
#define AUDIO_H

#include "smilparser/video.h"
#include "base_media.h"

class Audio : public BaseMedia
{
        Q_OBJECT
    public:
        Audio(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
        ~Audio();
        void                        init(TMedia *media);
        void                        deinit();
        void                        setParentItem(QQuickItem *parent);
    protected:
        TAudio                     *MyAudio;
        QScopedPointer<QQuickItem>  audio_item;
    public slots:
        void                        finished();
};

#endif // AUDIO_H