/**
 ** @file qnnodeicon.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-15
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

#include "qnnodeicon.h"
#include "qnnode.h"

QNNodeIcon::QNNodeIcon(QNModelPtr aContainer): QNIcon(aContainer){
    this->data = nullptr;
    this->addClass("node");
}

QNNodeIcon::~QNNodeIcon(){
    this->data = nullptr;
}

bool QNNodeIcon::operator==(const QNNodeIcon& aOther) const{
    bool result = false;
    result = (this->data==nullptr && aOther.data==nullptr) ||
             (this->data!=nullptr && aOther.data!=nullptr && this->data->getId() == aOther.data->getId());

    return result && QNIcon::operator==(aOther);
}

bool QNNodeIcon::operator!=(const QNNodeIcon& aOther) const{
    return !this->operator==(aOther);
}

QNError QNNodeIcon::initId(const uint aLen){
    if(this->data != nullptr)
        return this->data->initId(aLen);
    else
        return QNError(NO_DATA, "impossible to generate ID. No node data associated");
}

QNNodePtr QNNodeIcon::getData() const{
    return this->data;
}

void QNNodeIcon::setData(const QNNodePtr& value){
    if(value != nullptr){
        this->data = value;
        QNIcon::setId(this->data->getId());
    }
}

QStringList QNNodeIcon::getAllKeys() const{
    QStringList result;
    if(this->data)
        result = this->data->getAllKeys();

    result += QNIcon::getAllKeys();
    result.removeDuplicates();
    return result;
}

QVariant QNNodeIcon::getProperty(const QString& aKey) const{
    if(this->data){
        QVariant qv = this->data->getProperty(aKey);
        if(qv.isValid())
            return qv;
    }

    return QNIcon::getProperty(aKey);
}

QNError QNNodeIcon::setProperty(const QString& aKey, const QVariant& aValue){
    if(this->data){
        QVariant qv = this->data->getProperty(aKey);
        if(qv.isValid())
            return this->data->setProperty(aKey, aValue);
    }

    return QNIcon::setProperty(aKey, aValue);
}
