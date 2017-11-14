#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <QObject>
#include <QFileInfo>
#include <QPointer>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "files/network_access.h"

class FileDownloader : public TNetworkAccess
{
        Q_OBJECT
    public:
        explicit FileDownloader(QNetworkAccessManager* nam, QByteArray agent, QObject* parent= nullptr);
        ~FileDownloader();

        void startDownload(QUrl url, QString fileName);
        void cancelDownload();
    signals:
        void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
        void downloadSuccessful();
        void downloadError(QString errorString);

        void goingBusy();
        void backReady();

    protected:
        QNetworkAccessManager  *network_manager;
        QUrl                    download_url;
        QFile                   destinationFile;
        QPointer<QNetworkReply> network_reply;
        void cleanupDownload();
    protected slots:
        void readData();
        void finishDownload();

};


#endif // FILE_DOWNLOADER_H