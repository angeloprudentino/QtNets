/**
 ** @file qnnetwork.cpp
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

#include "qnnetwork.h"
#include "qnnetworkmodel.h"


bool QNNetwork::compareNodes(const QNNetwork& aOther) const{
    QStringList keys = this->getAllNodesKeys();
    foreach (QString k, keys) {
        QNNodePtr n1 = aOther.getNode(k);
        QNNodePtr n2 = this->getNode(k);
        if(n1 == nullptr || !(*n1 == *n2))
            return false;
    }

    return true;
}

bool QNNetwork::compareEdges(const QNNetwork& aOther) const{
    QStringList keys = this->getAllEdgesKeys();
    foreach (QString k, keys) {
        QNEdgePtr e1 = aOther.getEdge(k);
        QNEdgePtr e2 = this->getEdge(k);
        if(e1 == nullptr || !(*e1 == *e2))
            return false;
    }

    return true;
}

QNNetwork::QNNetwork(const QString &aId, const QString &aVersion){
    this->setId(aId);
    this->setVersion(aVersion);
    this->nodes.reset();
    this->edges.reset();
    this->netModel.reset();
}

QNNetwork::~QNNetwork(){
    if(!this->nodes.isNull()){
        QList<QNNodePtr> l = this->nodes->values();
        int size = l.size();
        for(int i=0; i<size; i++){
            if(l.at(i) != nullptr)
                delete l.at(i);
        }
        this->nodes->clear();
        this->nodes.reset();
    }

    if(!this->edges.isNull()){
        QList<QNEdgePtr> l = this->edges->values();
        int size = l.size();
        for(int i=0; i<size; i++){
            if(l.at(i) != nullptr)
                delete l.at(i);
        }
        this->edges->clear();
        this->edges.reset();
    }

    if(!this->netModel.isNull())
        this->netModel.reset();
}

bool QNNetwork::operator==(const QNNetwork& aOther) const{
    bool result = false;
    if(this->id == aOther.id)
        if(this->version == aOther.version)
            if((this->netModel.isNull() && aOther.netModel.isNull()) ||
               (!this->netModel.isNull() && !aOther.netModel.isNull() &&
                *(this->netModel.data()) == *(aOther.netModel.data())))
                if(this->compareNodes(aOther))
                    result = this->compareEdges(aOther);

    return result && QNProperties::operator==(aOther);
}

bool QNNetwork::operator!=(const QNNetwork& aOther) const{
    return !this->operator==(aOther);
}

QString QNNetwork::getId() const{
    return this->id;
}

void QNNetwork::setId(const QString& aId){
    if(!aId.isEmpty()){
        this->id = aId;
        if(this->netModel != nullptr)
            this->netModel->setId(aId);
    }
}

QString QNNetwork::getVersion() const{
    return this->version;
}

void QNNetwork::setVersion(const QString& aVersion){
    if(!aVersion.isEmpty()){
        this->version = aVersion;
        if(this->netModel != nullptr)
            this->netModel->setVersion(aVersion);
    }
}

QNNodePtr QNNetwork::getNode(const QString& aNodeId) const{
    if(!this->nodes.isNull()){
        if(this->nodes->contains(aNodeId))
            return this->nodes->value(aNodeId);
        else
            return nullptr;
    }
    else
        return nullptr;
}

QStringList QNNetwork::getAllNodesKeys() const{
    if(!this->nodes.isNull())
        return this->nodes->keys();
    else
        return QStringList();
}

void QNNetwork::addNode(QNNodePtr aNode){
    if(aNode != nullptr){
        if(this->nodes.isNull())
            this->nodes.reset(new QNNodeMap());

        this->nodes->insert(aNode->getId(), aNode);
    }
}

void QNNetwork::removeNode(QNNodePtr aNode, const bool aDelete){
    if(aNode != nullptr)
        if(!this->nodes.isNull()){
            QNNodePtr n = this->nodes->take(aNode->getId());
            if(aDelete)
                delete n;
        }
}

QStringList QNNetwork::getAllEdgesKeys() const{
    if(!this->edges.isNull())
        return this->edges->keys();
    else
        return QStringList();
}

QNEdgePtr QNNetwork::getEdge(const QString& aEdgeKey) const{
    if(!this->edges.isNull()){
        if(this->edges->contains(aEdgeKey))
            return this->edges->value(aEdgeKey);
        else
            return nullptr;
    }
    else
        return nullptr;
}

void QNNetwork::addEdge(QNEdgePtr aEdge){
    if(aEdge != nullptr){
        if(this->edges.isNull())
            this->edges.reset(new QNEdgeMap());

        this->edges->insert(aEdge->getId(), aEdge);
    }
}

void QNNetwork::removeEdge(QNEdgePtr aEdge, const bool aDelete){
    if(aEdge != nullptr){
        if(!this->edges.isNull()){
            QNNodePtr src = aEdge->getSrc();
            QNNodePtr dst = aEdge->getDst();
            if(src != nullptr && dst != nullptr){
                dst->removeAncestor(src);
                src->removeSuccessor(dst);
            }
            QNEdgePtr a = this->edges->take(aEdge->getId());
            if(aDelete)
                delete a;
        }
    }
}

QNNetworkModelPtr QNNetwork::getNetModel() const{
    return (this->netModel.isNull()) ? nullptr : this->netModel.data();
}

void QNNetwork::setNetModel(QNNetworkModelPtr value){
    this->netModel.reset(value);
}

QStringList QNNetwork::getAllKeys() const{
    QStringList res = QNProperties::getAllKeys();
    res.push_front("id");
    res.push_front("version");

    return res;
}

QVariant QNNetwork::getProperty(const QString& aKey) const{
    if(aKey == "id")
        return this->getId();
    else if(aKey == "version")
        return this->getVersion();
    else
        return QNProperties::getProperty(aKey);
}

QNError QNNetwork::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "id"){
        this->setId(aValue.toString());
        return QNError();
    }
    else if(aKey == "version"){
        this->setVersion(aValue.toString());
        return QNError();
    }
    else
        return QNProperties::setProperty(aKey, aValue);
}

QNNodeList QNNetwork::getNodesByProperties(const QNPropertyList& aProperties, const QNFilterStrategy& aStrategy){
    QNNodeList result;

    if(aStrategy > QNFilterStrategy::OR)
        return result; //invalid strategy

    if(!this->nodes.isNull()){
        foreach (QNNodePtr n, this->nodes->values()) {
            //filter nodes
            bool match = true;
            foreach (QNProperty p, aProperties) {
                QString key = p.first;
                QVariant value = p.second;

                //check for existence
                QVariant prop = n->getProperty(key);
                if(prop.isValid()){
                    if(value.isValid()){
                        //check value
                        match = (value == prop);
                    }
                    else
                        match = true;
                }
                else
                    match = false;

                if(aStrategy == QNFilterStrategy::AND){
                    //all properties must match
                    if(!match)
                        break;
                }
                else if(aStrategy == QNFilterStrategy::OR){
                    //at least one property must match
                    if(match)
                        break;
                }
            }

            if(match)
                result.push_back(n);
        }
    }

    return result;
}

QNEdgeList QNNetwork::getEdgesByProperties(const QNPropertyList& aProperties, const QNFilterStrategy& aStrategy){
    QNEdgeList result;

    if(aStrategy > QNFilterStrategy::OR)
        return result; //invalid strategy

    if(!this->edges.isNull()){
        foreach (QNEdgePtr a, this->edges->values()) {
            //filter nodes
            bool match = true;
            foreach (QNProperty p, aProperties) {
                QString key = p.first;
                QVariant value = p.second;

                //check for existence
                QVariant prop = a->getProperty(key);
                if(prop.isValid()){
                    if(value.isValid()){
                        //check value
                        match = (value == prop);
                    }
                    else
                        match = true;
                }
                else
                    match = false;

                if(aStrategy == QNFilterStrategy::AND){
                    //all properties must match
                    if(!match)
                        break;
                }
                else if(aStrategy == QNFilterStrategy::OR){
                    //at least one property must match
                    if(match)
                        break;
                }
            }

            if(match)
                result.push_back(a);
        }
    }

    return result;
}
