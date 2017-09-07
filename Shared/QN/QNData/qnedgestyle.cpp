/**
 ** @file qnedgestyle.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-20
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

#include "qnedgestyle.h"
#include "qnedge.h"


QNEdgeStyle::QNEdgeStyle(QNEdgePtr aData){
    this->setData(aData);
    this->setComment("");
    this->setProperty("empty", "");
    this->addClass("edge");
}

QNEdgeStyle::~QNEdgeStyle(){
    this->data = nullptr;
}

bool QNEdgeStyle::operator==(const QNEdgeStyle& aOther) const{
    bool result = false;
    if(this->comment == aOther.comment)
        result = (this->data==nullptr && aOther.data==nullptr) ||
                 (this->data!=nullptr && aOther.data!=nullptr && this->data->getId()==aOther.data->getId());

    return result && QNProperties::operator==(aOther) && QNStylable::operator==(aOther);
}

bool QNEdgeStyle::operator!=(const QNEdgeStyle& aOther) const{
    return !this->operator==(aOther);
}

QString QNEdgeStyle::getId() const{
    return this->id;
}

QNEdgePtr QNEdgeStyle::getData() const{
    return this->data;
}

void QNEdgeStyle::setData(QNEdgePtr aData){
    if(aData != nullptr){
        this->data = aData;

        if(aData != nullptr)
            this->id = aData->getSrc()->getId() + " - " + aData->getDst()->getId();
        else
            this->id = "";
    }
}

QString QNEdgeStyle::getComment() const{
    return this->comment;
}

void QNEdgeStyle::setComment(const QString& value){
    this->comment = value;
}

QStringList QNEdgeStyle::getAllKeys() const{
    QStringList result;
    if(this->data)
        result = this->data->getAllKeys();

    result += QNProperties::getAllKeys();
    result.append("id");
    result.append("comment");
    result.removeDuplicates();
    return result;
}

QVariant QNEdgeStyle::getProperty(const QString& aKey) const{
    if(this->data){
        QVariant qv = this->data->getProperty(aKey);
        if(qv.isValid())
            return qv;
    }

    if(aKey == "id")
        return this->getId();
    else if(aKey == "comment")
        return this->getComment();
    else
        return QNProperties::getProperty(aKey);
}

QNError QNEdgeStyle::setProperty(const QString& aKey, const QVariant& aValue){
    if(this->data){
        QVariant qv = this->data->getProperty(aKey);
        if(qv.isValid())
            return this->data->setProperty(aKey, aValue);
    }

    if(aKey == "id")
        return QNError(INVALID_REQ, "ID cannot be set directly"); //return this->setId(aValue.toString());
    else if(aKey == "comment"){
        this->setComment(aValue.toString());
        return QNError();
    }
    else
        return QNProperties::setProperty(aKey, aValue);
}
