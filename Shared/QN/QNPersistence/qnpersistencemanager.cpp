/**
 ** @file qnpersistencemanager.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-06
 **
 ** Copyright 2016 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
 **
 ** This program is free software: you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation, either version 3 of the License, or
 ** (at your option) any later version.
 **
 ** This program is distributed in the hope that it will be useful,
 ** but WITHOUT ANY WARRANTY; without even the implied warranty of
 ** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ** GNU General Public License for more details.
 **
 ** You should have received a copy of the GNU General Public License
 ** along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include <QFile>
#include <QPluginLoader>
#include <QLibrary>
#include "qnpersistencemanager.h"

QNPersistenceManager::QNPersistenceManager(const QDir& aPluginsDir){
    //load persistency plugins
    this->workers.reset();
    foreach (QString fileName, aPluginsDir.entryList(QDir::Files)) {
        if(QLibrary::isLibrary(fileName)){
            QPluginLoader pluginLoader(aPluginsDir.absoluteFilePath(fileName));
            QObjectPtr plugin = pluginLoader.instance();
            if (plugin != nullptr && qobject_cast<QNPersistenceInterfacePtr>(plugin)) {
                if(this->workers.isNull())
                    this->workers.reset(new QNPersistenceList());

                this->workers->push_back(qobject_cast<QNPersistenceInterfacePtr>(plugin));
            }
            else
                pluginLoader.unload();
        }
    }
}

QNPersistenceManager::~QNPersistenceManager(){
    if(!this->workers.isNull())
        this->workers.reset();
}

QRegExpList QNPersistenceManager::supportedFormats(){
    QRegExpList result;
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNPersistenceInterfacePtr temp = this->workers->at(i);
            result += temp->supportedFormats();
        }
    }

    return result;
}

bool QNPersistenceManager::isSupported(const QString& aFile){
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNPersistenceInterfacePtr temp = this->workers->at(i);
            if(temp->isSupported(aFile))
                return true;
        }
    }

    return false;
}

QNError QNPersistenceManager::load(const QString& aFile, ScopedQNNetwork& aNet){
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNPersistenceInterfacePtr temp = this->workers->at(i);
            if(temp->isSupported(aFile))
                return temp->load(aFile, aNet);
        }
    }

    return QNError(NET_FORMAT_NOT_SUPPORTED, aFile + " cannot be loaded because its format is not supported");
}

QNError QNPersistenceManager::store(const QString& aFile, const ScopedQNNetwork& aNet){
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNPersistenceInterfacePtr temp = this->workers->at(i);
            if(temp->isSupported(aFile))
                return temp->store(aFile, aNet);
        }
    }

    return QNError(NET_FORMAT_NOT_SUPPORTED, aFile + " cannot be stored because its format is not supported");
}
