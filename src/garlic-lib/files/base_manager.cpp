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

#include "base_manager.h"

Files::BaseManager::BaseManager(QObject *parent) : QObject(parent)
{
    setParent(parent);
}


// ==================  protected methods =======================================

bool Files::BaseManager::isRemote(QString src)
{
    if (src.mid(0, 4) == "http" || src.mid(0,3) == "ftp")
        return true;
    else
        return false;
}


bool Files::BaseManager::isRelative(QString src)
{
    return (src.at(0) != '/');
}

void Files::BaseManager::renameDownloadedFile(QString file_path)
{
    QFile file(file_path+FILE_DOWNLOADED_SUFFIX);
    if (file.exists())
    {
        QFile::remove(file_path); // rename cannot overwrite
        file.rename(file_path);
    }
}
