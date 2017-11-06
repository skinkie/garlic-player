#ifndef WEB_H
#define WEB_H

#include "smilparser/web.h"
#include "base_media.h"

class Web : public BaseMedia
{
        Q_OBJECT
    public:
        Web(QQmlComponent *mc, QString r_id, QObject *parent = nullptr);
        ~Web();
        void          init(TMedia *media);
        void          deinit();
        void          setParentItem(QQuickItem *parent);
    protected:
        TWeb          *MyWeb;
        QScopedPointer<QQuickItem>   web_item;
};

#endif // WEB_H
