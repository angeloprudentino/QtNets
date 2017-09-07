/**
 ** @file qnnetworkmodel.cpp
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

#include "qnnetworkmodel.h"
#include "qnnodeicon.h"
#include "qntextbox.h"


bool QNNetworkModel::compareAllIcons(const QNNetworkModel& aOther) const{
    if(this->allIcons.isNull() && aOther.allIcons.isNull())
        return true;

    if(!(!this->allIcons.isNull() && !aOther.allIcons.isNull()))
        return false;

    QStringList keys = this->allIcons->keys();
    foreach (QString k, keys) {
        QNIconPtr ic1 = this->getIconG(k);
        QNIconPtr ic2 = aOther.getIconG(k);
        if(ic2 == nullptr || !(*ic1 == *ic2))
            return false;
    }

    return true;
}

bool QNNetworkModel::compareEdges(const QNNetworkModel& aOther) const{
    if(this->edges.isNull() && aOther.edges.isNull())
        return true;

    if(!(!this->edges.isNull() && !aOther.edges.isNull()))
        return false;

    QStringList keys = this->getAllEdgesKeys();
    foreach (QString k, keys) {
        QNEdgeStylePtr es1 = this->getEdge(k);
        QNEdgeStylePtr es2 = aOther.getEdge(k);
        if(es2 == nullptr || !(*es1 == *es2))
            return false;
    }

    return true;
}

bool QNNetworkModel::compareAllTextboxes(const QNNetworkModel& aOther) const{
    if(this->allTextboxes.isNull() && aOther.allTextboxes.isNull())
        return true;

    if(!(!this->allTextboxes.isNull() && !aOther.allTextboxes.isNull()))
        return false;

    if(this->allTextboxes->size() != aOther.allTextboxes->size())
        return false;

    int size = this->allTextboxes->size();
    for(int i=0; i<size; i++){
        bool found = false;
        QNTextBoxPtr tb1 = this->allTextboxes->at(i);
        for(int j=0; j<size; j++){
            QNTextBoxPtr tb2 = aOther.allTextboxes->at(j);
            if(tb1!=nullptr && tb2!=nullptr && *tb1 == *tb2)
                found = true;
        }
        if(!found)
            return false;
    }

    return true;
}

QNNetworkModel::QNNetworkModel(QNNetworkPtr aNet): QNModel(nullptr){
    this->net = aNet;
    if(aNet != nullptr){
        this->setId(aNet->getId());
        this->setVersion(aNet->getVersion());
    }
    else{
        this->initId();
        this->version = "";
    }

    this->allIcons.reset();
    this->allTextboxes.reset();
    this->edges.reset();
}

QNNetworkModel::~QNNetworkModel(){
    this->net = nullptr;

    if(!this->allIcons.isNull()){
        this->allIcons->clear();
        this->allIcons.reset();
    }

    if(!this->allTextboxes.isNull()){
        this->allTextboxes->clear();
        this->allTextboxes.reset();
    }

    if(!this->edges.isNull()){
        QList<QNEdgeStylePtr> l = this->edges->values();
        int size = l.size();
        for(int i=0; i<size; i++){
            if(l.at(i) != nullptr)
                delete l.at(i);
        }
        this->edges->clear();
        this->edges.reset();
    }
}

bool QNNetworkModel::operator==(const QNNetworkModel& aOther) const{
    bool result = false;
    if(this->version == aOther.version)
        if((this->net==nullptr && aOther.net==nullptr) ||
           (this->net!=nullptr && aOther.net!=nullptr && this->net->getId()==aOther.net->getId()))
            if(this->compareAllIcons(aOther))
                if(this->compareEdges(aOther))
                    result = this->compareAllTextboxes(aOther);

    return result && QNModel::operator==(aOther);
}

bool QNNetworkModel::operator!=(const QNNetworkModel& aOther) const{
    return !this->operator==(aOther);
}

QString QNNetworkModel::getVersion() const{
    return this->version;
}

QNNetworkPtr QNNetworkModel::getNet() const{
    return this->net;
}

QStringList QNNetworkModel::getAllEdgesKeys() const{
    if(!this->edges.isNull())
        return this->edges->keys();
    else
        return QStringList();
}

QNEdgeStylePtr QNNetworkModel::getEdge(const QString& aEdgeKey) const{
    if(!this->edges.isNull()){
        if(this->edges->contains(aEdgeKey))
            return this->edges->value(aEdgeKey);
        else
            return nullptr;
    }
    else
        return nullptr;
}

void QNNetworkModel::setVersion(const QString& aValue){
    if(!aValue.isEmpty()){
        this->version = aValue;
    }
}

void QNNetworkModel::addEdge(QNEdgeStylePtr aEdge){
    if(aEdge != nullptr){
        if(this->edges.isNull())
            this->edges.reset(new QNEdgeStyleMap());

        this->edges->insert(aEdge->getId(), aEdge);
    }
}

void QNNetworkModel::removeEdge(QNEdgeStylePtr aEdge, const bool aDelete){
    if(aEdge != nullptr)
        if(!this->edges.isNull()){
            this->edges->remove(aEdge->getId());
            if(aDelete)
                delete aEdge;
        }
}

void QNNetworkModel::addIconToAll(QNIconPtr aIcon){
    if(aIcon != nullptr){
        if(this->allIcons.isNull())
            this->allIcons.reset(new QNIconMap());

        this->allIcons->insert(aIcon->getId(), aIcon);
    }
}

bool QNNetworkModel::removeIconFromAll(QNIconPtr aIcon){
    if(aIcon != nullptr && !this->allIcons.isNull())
        return this->allIcons->remove(aIcon->getId()) > 0;
    else
        return false;
}

void QNNetworkModel::addTextBoxToAll(QNTextBoxPtr aTextBox){
    if(aTextBox != nullptr){
        if(this->allTextboxes.isNull())
            this->allTextboxes.reset(new QNTextBoxList());

        this->allTextboxes->push_back(aTextBox);
    }
}

bool QNNetworkModel::removeTextBoxFromAll(QNTextBoxPtr aTextBox){
    if(aTextBox != nullptr && !this->allTextboxes.isNull())
        return this->allTextboxes->removeOne(aTextBox);
    else
        return false;
}

QStringList QNNetworkModel::getAllKeys() const{
    QStringList res = QNModel::getAllKeys();
    res.push_front("version");

    return res;
}

QVariant QNNetworkModel::getProperty(const QString& aKey) const{
    if(aKey == "version")
        return this->getVersion();
    else
        return QNModel::getProperty(aKey);
}

QNError QNNetworkModel::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "version"){
        this->setVersion(aValue.toString());
        return QNError();
    }
    else
        return QNModel::setProperty(aKey, aValue);
}

QNIconList QNNetworkModel::getIconsByProperties(const QNPropertyList& aProperties, const QNFilterStrategy& aStrategy){
    QNIconList result;

    if(aStrategy > QNFilterStrategy::OR)
        return result; //invalid strategy

    if(!this->allIcons.isNull()){
        foreach (QNIconPtr ic, this->allIcons->values()) {
            //filter nodes
            bool match = true;
            foreach (QNProperty p, aProperties) {
                QString key = p.first;
                QVariant value = p.second;

                //check for existence
                QVariant prop = ic->getProperty(key);
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
                result.push_back(ic);
        }
    }

    return result;
}

QNTextBoxList QNNetworkModel::getTextboxesByProperties(const QNPropertyList& aProperties, const QNFilterStrategy& aStrategy){
    QNTextBoxList result;

    if(aStrategy > QNFilterStrategy::OR)
        return result; //invalid strategy

    if(!this->allTextboxes.isNull()){
        int size = this->allTextboxes->size();
        for(int i=0; i<size; i++) {
            QNTextBoxPtr tb = this->allTextboxes->at(i);
            //filter nodes
            bool match = true;
            foreach (QNProperty p, aProperties) {
                QString key = p.first;
                QVariant value = p.second;

                //check for existence
                QVariant prop = tb->getProperty(key);
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
                result.push_back(tb);
        }
    }

    return result;
}

QNEdgeStyleList QNNetworkModel::getEdgesByProperties(const QNPropertyList& aProperties, const QNFilterStrategy& aStrategy){
    QNEdgeStyleList result;

    if(aStrategy > QNFilterStrategy::OR)
        return result; //invalid strategy

    if(!this->edges.isNull()){
        foreach (QNEdgeStylePtr a, this->edges->values()) {
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

QNStylableList QNNetworkModel::getStylablesByClass(const QStringList& aClasses, const QNFilterStrategy& aStrategy){
    QNStylableList result;

    if(aStrategy > QNFilterStrategy::OR)
        return result; //invalid strategy

    //list of ALL stylables
    QNStylableList all = QNStylableList();
    if(!this->allIcons.isNull()){
        foreach (QNIconPtr ic, this->allIcons->values()) {
            all.append(ic);
        }
    }
    if(!this->allTextboxes.isNull()){
        int size = this->allTextboxes->size();
        for(int i=0; i<size; i++) {
            QNTextBoxPtr tb = this->allTextboxes->at(i);
            all.append(tb);
        }
    }
    if(!this->edges.isNull()){
        foreach (QNEdgeStylePtr a, this->edges->values()) {
            all.append(a);
        }
    }

    foreach (QNStylablePtr stylable, all) {
        //filter stylables
        bool match = true;
        foreach (QString c, aClasses) {
            //check if has the class
            match = stylable->hasClass(c);

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
            result.push_back(stylable);
    }

    return result;
}
