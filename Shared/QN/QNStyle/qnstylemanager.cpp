/**
 ** @file qnstylemanager.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-28
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

#include "qnstylemanager.h"
#include "qnstyle.h"
#include <QFile>
#include <QPluginLoader>
#include <QLibrary>


QNStyleManager::QNStyleManager(const QDir& aPluginsDir){
    this->current = nullptr;
    this->workers.reset();
    this->style.reset();

    foreach (QString fileName, aPluginsDir.entryList(QDir::Files)) {
        if(QLibrary::isLibrary(fileName)){
            QPluginLoader pluginLoader(aPluginsDir.absoluteFilePath(fileName));
            QObjectPtr plugin = pluginLoader.instance();
            if (plugin != nullptr && qobject_cast<QNStyleInterfacePtr>(plugin)) {
                if(this->workers.isNull())
                    this->workers.reset(new QNStyleList());

                this->workers->push_back(qobject_cast<QNStyleInterfacePtr>(plugin));
            }
            else
                pluginLoader.unload();
        }
    }
}

QNStyleManager::~QNStyleManager(){
    this->current = nullptr;

    if(!this->workers.isNull())
        this->workers.reset();

    if(!this->style.isNull())
        this->style.reset();
}

QRegExpList QNStyleManager::supportedFormats(){
    QRegExpList result;
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNStyleInterfacePtr temp = this->workers->at(i);
            result += temp->supportedFormats();
        }
    }

    return result;
}

bool QNStyleManager::isSupported(const QString& aFile){
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNStyleInterfacePtr temp = this->workers->at(i);
            if(temp->isSupported(aFile))
                return true;
        }
    }

    return false;
}

QNError QNStyleManager::resetStyle(){
    this->current = nullptr;
    this->style.reset();
    return QNError();
}


QNError QNStyleManager::newStyle(const QString& aFile, const QString& aStyleId, const QString& aStyleVersion){
    if(this->current == nullptr || !this->current->isSupported(aFile)){
        //the current plugin does not support the required format;
        //find a new plugin
        this->current = nullptr;
        if(!this->workers.isNull()){
            int size = this->workers->size();
            for(int i=0; i<size; i++){
                QNStyleInterfacePtr temp = this->workers->at(i);
                if(temp->isSupported(aFile)){
                    this->current = temp;
                }
            }
        }
    }

    if(this->current != nullptr){
        //plugin found
        this->style.reset(new QNStyle(aStyleId, aStyleVersion));
        return QNError();
    }

    return QNError(STYLE_FORMAT_NOT_SUPPORTED, aFile + " cannot be created because its format is not supported");
}

QNError QNStyleManager::load(const QString& aFile){
    if(this->current != nullptr && this->current->isSupported(aFile))
        return this->current->load(aFile, this->style);

    //the current plugin does not support the required format;
    //find a new plugin
    this->current = nullptr;
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNStyleInterfacePtr temp = this->workers->at(i);
            if(temp->isSupported(aFile)){
                this->current = temp;
                return this->current->load(aFile, this->style);
            }
        }
    }

    return QNError(STYLE_FORMAT_NOT_SUPPORTED, aFile + " cannot be loaded because its format is not supported");
}

QNError QNStyleManager::store(const QString& aFile){
    if(this->current != nullptr && this->current->isSupported(aFile))
        return this->current->store(aFile, this->style);

    //the current plugin does not support the required format;
    //find a new plugin
    this->current = nullptr;
    if(!this->workers.isNull()){
        int size = this->workers->size();
        for(int i=0; i<size; i++){
            QNStyleInterfacePtr temp = this->workers->at(i);
            if(temp->isSupported(aFile)){
                this->current = temp;
                return this->current->store(aFile, this->style);
            }
        }
    }

    return QNError(STYLE_FORMAT_NOT_SUPPORTED, aFile + " cannot be stored because its format is not supported");
}

QNError QNStyleManager::updateFont(QGraphicsItemPtr aItem){
    if(this->current != nullptr)
        return this->current->updateFont(this->getStyle(), aItem);
    else
        return QNError(INVALID_STYLE_STATE, "there is no valid style plugin selected");
}

QNError QNStyleManager::paint(QGraphicsItemPtr aItem, QPainterPtr aPainter,
                              QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget){

    if(this->current != nullptr)
        return this->current->paint(this->getStyle(), aItem, aPainter, aOption, aWidget);
    else
        return QNError(INVALID_STYLE_STATE, "there is no valid style plugin selected");
}

QNStylePtr QNStyleManager::getStyle() const{
    return (this->style.isNull())? nullptr : this->style.data();
}
