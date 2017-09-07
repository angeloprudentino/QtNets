/**
 ** @file qnedge.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-06
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


#include "qnedge.h"
#include "qnedgestyle.h"


void QNEdge::updateID(){
    if(this->src != nullptr && this->dst != nullptr)
        this->id = this->src->getId() + " - " + this->dst->getId();
    else
        this->id = "";
}

QNEdge::QNEdge(const QString& aId, QNNetworkPtr aNet){
    this->id = aId;
    this->src = nullptr;
    this->dst = nullptr;
    this->net = aNet;
    this->icon = nullptr;
}

QNEdge::QNEdge(QNNodePtr aSrc, QNNodePtr aDst, QNNetworkPtr aNet){
    this->src = aSrc;
    this->dst = aDst;
    this->updateID();
    this->net = aNet;
    this->icon = nullptr;
}

QNEdge::~QNEdge(){
    this->id = "";
    this->src = nullptr;
    this->dst = nullptr;
    this->net = nullptr;
    this->icon = nullptr;
}

bool QNEdge::operator==(const QNEdge& aOther) const{
    bool result = false;
    if(this->id == aOther.id)
        if((this->net==nullptr && aOther.net==nullptr) || (this->net!=nullptr && aOther.net!=nullptr && this->net->getId() == aOther.net->getId()))
            if((this->icon==nullptr && aOther.icon==nullptr) || (this->icon!=nullptr && aOther.icon!=nullptr && *(this->icon) == *(aOther.icon)))
                if((this->src==nullptr && aOther.src==nullptr) || (this->src!=nullptr && aOther.src!=nullptr && *(this->src) == *(aOther.src)))
                    result = (this->dst==nullptr && aOther.dst==nullptr) || (this->dst!=nullptr && aOther.dst!=nullptr && *(this->dst) == *(aOther.dst));

    return result && QNProperties::operator==(aOther);
}

bool QNEdge::operator!=(const QNEdge& aOther) const{
    return !this->operator==(aOther);
}

QString QNEdge::getId() const{
    return this->id;
}

QNNodePtr QNEdge::getSrc() const {
    return this->src;
}

void QNEdge::setSrc(const QNNodePtr& value){
    if(value != nullptr){
        this->src = value;
        this->updateID();
    }
}

QNNodePtr QNEdge::getDst() const {
    return this->dst;
}

void QNEdge::setDst(const QNNodePtr& value){
    if(value != nullptr){
        this->dst = value;
        this->updateID();
    }
}

QNEdgeStylePtr QNEdge::getIcon() const{
    return this->icon;
}

void QNEdge::setIcon(QNEdgeStylePtr value){
    this->icon = value;
}

QNNetworkPtr QNEdge::getNet() const{
    return this->net;
}

QStringList QNEdge::getAllKeys() const{
    QStringList res = QNProperties::getAllKeys();
    res.push_front("id");

    return res;
}

QVariant QNEdge::getProperty(const QString& aKey) const{
    if(aKey == "id")
        return this->getId();
    else
        return QNProperties::getProperty(aKey);
}

QNError QNEdge::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "id"){
        this->id = aValue.toString();
        return QNError();
    }
    else
        return QNProperties::setProperty(aKey, aValue);
}
