/**
 ** @file qnicon.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-17
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

#include "qnicon.h"
#include "qnmodel.h"


QNIcon::QNIcon(QNModelPtr aContainer){
    this->container = aContainer;
    this->setId("");
    this->setName("");
    this->updatePropToShow("name");
    this->setColor(DEFAULT_BG_COLOR);
    this->setOutlineColor(DEFAULT_FG_COLOR);
    this->setOutlineWidth(DEFAULT_WIDTH);
}

QNIcon::~QNIcon(){
    this->container = nullptr;
}

bool QNIcon::operator==(const QNIcon& aOther) const{
    bool result = false;
    if(this->id == aOther.id)
        if(this->name == aOther.name)
            if(this->color == aOther.color)
                if(this->outlineColor == aOther.outlineColor)
                    if(this->outlineWidth == aOther.outlineWidth)
                        result = (this->container==nullptr && aOther.container==nullptr) ||
                                 (this->container!=nullptr && aOther.container!=nullptr && this->container->getId() == aOther.container->getId());

    return result && QNCommonIcon::operator==(aOther) && QNStylable::operator==(aOther);
}

bool QNIcon::operator!=(const QNIcon& aOther) const{
    return !this->operator==(aOther);
}

QString QNIcon::getId() const{
    return this->id;
}

QString QNIcon::getName() const{
    return this->name;
}

QString QNIcon::getColor() const{
    return this->color;
}

QString QNIcon::getOutlineColor() const{
    return this->outlineColor;
}

int QNIcon::getOutlineWidth() const{
    return this->outlineWidth;
}


QNModelPtr QNIcon::getContainer() const{
    return this->container;
}

QNError QNIcon::setId(const QString& value){
    if(!value.isEmpty()){
        this->id = value;
        return QNError();
    }

    return QNError(NO_DATA, "empty id");
}

void QNIcon::setName(const QString& value){
    if(!value.isEmpty()){
        this->name = value;
    }
}

void QNIcon::setColor(const QString& value){
    if(!value.isEmpty()){
        this->color = value;
    }
}

void QNIcon::setOutlineColor(const QString& value){
    if(!value.isEmpty()){
        this->outlineColor = value;
    }
}

void QNIcon::setOutlineWidth(int value){
    this->outlineWidth = value;
}

void QNIcon::setContainer(QNModelPtr value){
    if(value != nullptr)
        this->container = value;
}

QStringList QNIcon::getAllKeys() const{
    QStringList res = QNCommonIcon::getAllKeys();
    res.push_front("id");
    res.push_front("name");
    res.push_front("color");
    res.push_front("outlineColor");
    res.push_front("outlineWidth");

    return res;
}

QVariant QNIcon::getProperty(const QString& aKey) const{
    if(aKey == "id")
        return this->getId();
    else if(aKey == "name")
        return this->getName();
    else if(aKey == "color")
        return this->getColor();
    else if(aKey == "outlineColor")
        return this->getOutlineColor();
    else if(aKey == "outlineWidth")
        return this->getOutlineWidth();
    else
        return QNCommonIcon::getProperty(aKey);
}

QNError QNIcon::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "id"){
        return this->setId(aValue.toString());
    }
    else if(aKey == "name"){
        this->setName(aValue.toString());
        return QNError();
    }
    else if(aKey == "color"){
        this->setColor(aValue.toString());
        return QNError();
    }
    else if(aKey == "outlineColor"){
        this->setOutlineColor(aValue.toString());
        return QNError();
    }
    else if(aKey == "outlineWidth"){
        this->setOutlineWidth(aValue.toInt());
        return QNError();
    }
    else
        return QNCommonIcon::setProperty(aKey, aValue);
}
