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
#ifndef INDEX_MANAGER_H
#define INDEX_MANAGER_H

#include <QObject>
#include <QTimerEvent>
#include "base_manager.h"
#include "index_model.h"
#include "tools/main_configuration.h"

namespace Files
{
    class IndexManager : public BaseManager
    {
            Q_OBJECT
        public:
            explicit IndexManager(MainConfiguration *config, QObject *parent=Q_NULLPTR);
            void            init(QString src);
            bool            load();
            void            activateRefresh(int value);
            void            deactivateRefresh();
            QDomElement     getHead();
            QDomElement     getBody();
            void            lookUpForUpdatedIndex();

        protected:

            int             refresh_time = 0;
            int             timer_id = 0;
            MainConfiguration *MyConfiguration;
            IndexModel     *MyIndexModel;
            Downloader     *MyDownloader;
            QString         src_index_path;
            bool            loadLocal(QString local_path);
            void            timerEvent(QTimerEvent *event);

        protected slots:
            void doSucceed(TNetworkAccess *downloader);
        signals:
            void newIndexDownloaded();

    };
}
#endif // INDEX_MANAGER_H
