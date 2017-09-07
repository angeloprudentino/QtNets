/**
 ** @file qnnode.cpp
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

#include "qnnode.h"
#include "qnnetworkmodel.h"
#include "qnnodeicon.h"

bool QNNode::compareAncestors(const QNNode& aOther) const{
    if(this->ancestors==nullptr && aOther.ancestors==nullptr)
        return true;

    if(!(this->ancestors!=nullptr && aOther.ancestors!=nullptr))
        return false;

    if(this->ancestors->size() != aOther.ancestors->size())
        return false;

    int size = this->ancestors->size();
    for(int i=0; i<size; i++){
        bool found = false;
        QNNodePtr n1 = this->ancestors->at(i);
        for(int j=0; j<size; j++){
            QNNodePtr n2 = aOther.ancestors->at(j);
            if(n1!=nullptr && n2!=nullptr && n1->getId() == n2->getId())
                found = true;
        }
        if(!found)
            return false;
    }

    return true;
}

bool QNNode::compareSuccessors(const QNNode& aOther) const{
    if(this->successors==nullptr && aOther.successors==nullptr)
        return true;

    if(this->successors!=nullptr && aOther.successors!=nullptr && this->successors->size() != aOther.successors->size())
        return false;

    int size = this->successors->size();
    for(int i=0; i<size; i++){
        bool found = false;
        QNNodePtr n1 = this->successors->at(i);
        for(int j=0; j<size; j++){
            QNNodePtr n2 = aOther.successors->at(j);
            if(n1!=nullptr && n2!=nullptr && n1->getId() == n2->getId())
                found = true;
        }
        if(!found)
            return false;
    }

    return true;
}

QNNode::QNNode(QNNetworkPtr aNet){
    this->net = aNet;
    this->icon = nullptr;
    this->ancestors.reset();
    this->successors.reset();
}

QNNode::~QNNode(){
    this->net = nullptr;
    this->icon = nullptr;

    if(!this->ancestors.isNull()){
        this->ancestors->clear();
        this->ancestors.reset();
    }

    if(!this->successors.isNull()){
        this->successors->clear();
        this->successors.reset();
    }
}

bool QNNode::operator==(const QNNode& aOther) const{
    bool result = false;
    if(this->id == aOther.id)
        if((this->net==nullptr && aOther.net==nullptr) || (this->net!=nullptr && aOther.net!=nullptr && this->net->getId() == aOther.net->getId()))
            if((this->icon==nullptr && aOther.icon==nullptr) || (this->icon!=nullptr && aOther.icon!=nullptr && *(this->icon) == *(aOther.icon)))
                if(this->compareAncestors(aOther))
                    result = this->compareSuccessors(aOther);

    return result && QNProperties::operator==(aOther);
}

bool QNNode::operator!=(const QNNode& aOther) const{
    return !this->operator==(aOther);
}

QNNetworkPtr QNNode::getNet() const {
    return this->net;
}

QString QNNode::getId() const {
    return this->id;
}

QNError QNNode::setId(const QString& aValue){
    if(!aValue.isEmpty()){
        //check if not already used
        if(this->net->getNode(aValue) == nullptr){
            QNNetworkModelPtr netModel = this->net->getNetModel();
            if(netModel == nullptr || netModel->getIcon(aValue) == nullptr){
                //update id and return
                this->id = aValue;

                if(this->icon != nullptr)
                    this->icon->setId(aValue);

                return QNError();
            }
        }

        return QNError(DUPLICATED_ID, "Duplicated ID");
    }

    return QNError(NO_DATA, "Empty ID");
}

QNError QNNode::initId(const uint aLen){
    QNError err;

    if(this->net != nullptr){
        int attempts = DEFAULT_RETRY;
        do{
            err = this->setId(genRandString(aLen, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
            attempts--;
        }
        while(attempts >= 0 && err);
    }

    return err;
}

void QNNode::addAncestor(QNNodePtr aAncestor){
    if(aAncestor != nullptr){
        if(this->ancestors.isNull())
            this->ancestors.reset(new QNNodeList());

        this->ancestors->push_back(aAncestor);
    }
}

void QNNode::removeAncestor(QNNodePtr aAncestor){
    if(aAncestor != nullptr)
        if(!this->ancestors.isNull())
            this->ancestors->removeOne(aAncestor);
}

QNNodeListPtr QNNode::getAncestors() const {
    return (this->ancestors.isNull() || this->ancestors->isEmpty()) ? nullptr : this->ancestors.data();
}

void QNNode::addSuccessor(QNNodePtr aSuccessor){
    if(aSuccessor != nullptr){
        if(this->successors == nullptr)
            this->successors.reset(new QNNodeList());

        this->successors->push_back(aSuccessor);
    }
}

void QNNode::removeSuccessor(QNNodePtr aSuccessor){
    if(aSuccessor != nullptr)
        if(!this->successors.isNull())
            this->successors->removeOne(aSuccessor);
}

QNNodeListPtr QNNode::getSuccessors() const{
    return (this->successors.isNull() || this->successors->isEmpty()) ? nullptr : this->successors.data();
}

QNNodeIconPtr QNNode::getIcon() const{
    return this->icon;
}

void QNNode::setIcon(QNNodeIconPtr value){
    this->icon = value;
}

QStringList QNNode::getAllKeys() const{
    QStringList res = QNProperties::getAllKeys();
    res.push_front("id");

    return res;
}

QVariant QNNode::getProperty(const QString& aKey) const{
    if(aKey == "id")
        return this->getId();
    else
        return QNProperties::getProperty(aKey);
}

QNError QNNode::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "id")
        return this->setId(aValue.toString());
    else
        return QNProperties::setProperty(aKey, aValue);
}
