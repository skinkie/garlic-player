#include "mediaviewwrapper.h"

MediaViewWrapper::MediaViewWrapper(QWidget *parent)
{
    Q_UNUSED(parent);
#ifdef SUPPORT_QTAV
    VideoWidget = new QtAV::WidgetRenderer;
#else
    VideoWidget = new  QVideoWidget;
#endif
}

MediaViewWrapper::~MediaViewWrapper()
{
    delete VideoWidget;
}

#ifdef SUPPORT_QTAV
QtAV::WidgetRenderer *MediaViewWrapper::getVideoWidget()
{
    return VideoWidget;
}
#else
QVideoWidget *MediaViewWrapper::getVideoWidget()
{
    return VideoWidget;
}
#endif


void MediaViewWrapper::setAspectRatioFill()
{
#ifdef SUPPORT_QTAV
    VideoWidget->setOutAspectRatioMode(QtAV::VideoRenderer::RendererAspectRatio);
#else
    VideoWidget->setAspectRatioMode(Qt::IgnoreAspectRatio);
#endif
}

void MediaViewWrapper::setAspectRatioMeet()
{
#ifdef SUPPORT_QTAV
    VideoWidget->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
#else
    VideoWidget->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);
#endif
}

void MediaViewWrapper::setAspectRatioMeetBest()
{
#ifdef SUPPORT_QTAV
    VideoWidget->setOutAspectRatioMode(QtAV::VideoRenderer::VideoAspectRatio);
#else
    VideoWidget->setAspectRatioMode(Qt::KeepAspectRatio);
#endif
}
