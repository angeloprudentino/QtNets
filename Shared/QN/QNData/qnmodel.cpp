/**
 ** @file qnmodel.cpp
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

#include "qnmodel.h"
#include "qnnetworkmodel.h"

QNModelPtr QNModel::getTopContainer(){
    if(this->getContainer() != nullptr){
        QNModelPtr curr = this->getContainer();
        QNModelPtr top = curr->getContainer();

        while(top != nullptr){
            curr = top;
            top = curr->getContainer();
        }
        return curr;
    }

    return this;
}

bool QNModel::compareIcons(const QNModel& aOther) const{
    if(this->icons.isNull() && aOther.icons.isNull())
        return true;

    if(!(!this->icons.isNull() && !aOther.icons.isNull()))
        return false;

    QStringList keys = this->getAllIconsKeys();
    foreach (QString k, keys) {
        QNIconPtr ic1 = this->getIcon(k);
        QNIconPtr ic2 = aOther.getIcon(k);
        if(ic2 == nullptr || !(*ic1 == *ic2))
            return false;
    }

    return true;
}

bool QNModel::compareTextboxes(const QNModel& aOther) const{
    if(this->textboxes.isNull() && aOther.textboxes.isNull())
        return true;

    if(!(!this->textboxes.isNull() && !aOther.textboxes.isNull()))
        return false;

    if(this->textboxes->size() != aOther.textboxes->size())
        return false;

    int size = this->textboxes->size();
    for(int i=0; i<size; i++){
        bool found = false;
        QNTextBoxPtr tb1 = this->textboxes->at(i);
        for(int j=0; j<size; j++){
            QNTextBoxPtr tb2 = aOther.textboxes->at(j);
            if(tb1!=nullptr && tb2!=nullptr && *tb1 == *tb2)
                found = true;
        }
        if(!found)
            return false;
    }

    return true;
}

QNModel::QNModel(QNModelPtr aContainer): QNIcon(aContainer){
    this->window.reset();
    this->textboxes.reset();
    this->icons.reset();
    this->addClass("model");
}

QNModel::~QNModel(){
    this->window.reset();

    if(!this->icons.isNull()){
        QList<QNIconPtr> l = this->icons->values();
        int size = l.size();
        for(int i=0; i<size; i++){
            if(l.at(i) != nullptr)
                delete l.at(i);
        }
        this->icons->clear();
        this->icons.reset();
    }

    if(!this->textboxes.isNull()){
        int size = this->textboxes->size();
        for(int i=0; i<size; i++){
            if(this->textboxes->at(i) != nullptr)
                delete this->textboxes->at(i);
        }
        this->textboxes.reset();
    }
}

bool QNModel::operator==(const QNModel& aOther) const{
    bool result = false;
    if((this->window.isNull() && aOther.window.isNull()) ||
       (!this->window.isNull() && !aOther.window.isNull() && *(this->window.data()) == *(aOther.window.data())))
        if(this->compareIcons(aOther))
            result = this->compareTextboxes(aOther);

    return result && QNIcon::operator==(aOther);
}

bool QNModel::operator!=(const QNModel& aOther) const{
    return !this->operator==(aOther);
}

QNError QNModel::setId(const QString& aValue){
    if(!aValue.isEmpty()){
        QNModelPtr top = this->getTopContainer();

        //check if not already used
        if(top->getIcon(aValue) == nullptr){
            //update id and return
            return QNIcon::setId(aValue);
        }

        return QNError(DUPLICATED_ID, "Duplicated ID");
    }
    return QNError(NO_DATA, "Empty ID");
}

QNError QNModel::initId(const uint aLen){
    int attempts = DEFAULT_RETRY;

    QNError err;
    do{
        err = this->setId(genRandString(aLen, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"));
        attempts--;
    }
    while(attempts >= 0 && err);

    return err;
}

QNPositionPtr QNModel::getWindow() const{
    if(this->window.isNull())
        return nullptr;
    else
        return this->window.data();
}

QStringList QNModel::getAllIconsKeys() const{
    if(!this->icons.isNull())
        return this->icons->keys();
    else
        return QStringList();
}

QNIconPtr QNModel::getIcon(const QString& aIconId) const{
    if(!this->icons.isNull()){
        if(this->icons->contains(aIconId))
            return this->icons->value(aIconId);
        else
            return nullptr;
    }
    else
        return nullptr;
}

QNIconPtr QNModel::getIconG(const QString& aId) const{
    QNIconPtr result = nullptr;
    QNModelPtr model = (const QNModelPtr)this;

    while(result == nullptr && model != nullptr){
        result = model->getIcon(aId);
        model = model->getContainer();
    }

    return result;
}

void QNModel::addIcon(QNIconPtr aIcon){
    if(aIcon != nullptr){
        if(this->icons.isNull())
            this->icons.reset(new QNIconMap());

        this->icons->insert(aIcon->getId(), aIcon);

        //add to networkModel if present
        QNNetworkModelPtr netModel = dynamic_cast<QNNetworkModelPtr>(this->getTopContainer());
        if(netModel != nullptr)
            netModel->addIconToAll(aIcon);
    }
}

bool QNModel::removeIcon(QNIconPtr aIcon){
    if(aIcon != nullptr){
        //first remove from networkModel, if present
        QNNetworkModelPtr netModel = dynamic_cast<QNNetworkModelPtr>(this->getTopContainer());
        if(netModel != nullptr)
            netModel->removeIconFromAll(aIcon);

        if(!this->icons.isNull()){
            if(this->icons->remove(aIcon->getId())){
                delete aIcon;
                return true;
            }
            else{
                QList<QNIconPtr> l = this->icons->values();
                int size = l.size();
                for(int i=0; i<size; i++){
                    QNIconPtr icon = l.at(i);
                    if(dynamic_cast<QNModelPtr>(icon) != nullptr){
                        bool ret = dynamic_cast<QNModelPtr>(icon)->removeIcon(aIcon);
                        if(ret)
                            return true;
                    }
                }
            }
        }
    }

    return false;
}

bool QNModel::moveIcon(QNIconPtr aIcon, QNModelPtr aTo){
    bool res = false;
    if(aTo != nullptr){
        if(this->icons != nullptr)
            res = (this->icons->remove(aIcon->getId()) > 0);

        if(res)
            aTo->addIcon(aIcon);
    }

    return res;
}

QNTextBoxListPtr QNModel::getTextboxes() const{
    return (this->textboxes.isNull() || this->textboxes->isEmpty()) ? nullptr : this->textboxes.data();
}

void QNModel::setWindow(QNPositionPtr value){
    if(value != nullptr)
        this->window.reset(value);
}

void QNModel::addTextBox(QNTextBoxPtr aTextBox){
    if(aTextBox != nullptr){
        if(this->textboxes.isNull())
            this->textboxes.reset(new QNTextBoxList());

        this->textboxes->push_back(aTextBox);

        //add to networkModel, if present
        QNNetworkModelPtr netModel = dynamic_cast<QNNetworkModelPtr>(this->getTopContainer());
        if(netModel != nullptr)
            netModel->addTextBoxToAll(aTextBox);
    }
}

bool QNModel::removeTextBox(QNTextBoxPtr aTextBox){
    if(aTextBox != nullptr){
        //first remove from networkModel, if present
        QNNetworkModelPtr netModel = dynamic_cast<QNNetworkModelPtr>(this->getTopContainer());
        if(netModel != nullptr)
            netModel->removeTextBoxFromAll(aTextBox);

        if(!this->textboxes.isNull()){
            if(this->textboxes->removeOne(aTextBox)){
                delete aTextBox;
                return true;
            }
            else{
                QList<QNIconPtr> l = this->icons->values();
                int size = l.size();
                for(int i=0; i<size; i++){
                    QNIconPtr icon = l.at(i);
                    if(dynamic_cast<QNModelPtr>(icon) != nullptr){
                        bool ret = dynamic_cast<QNModelPtr>(icon)->removeTextBox(aTextBox);
                        if(ret)
                            return true;
                    }
                }
            }
        }
    }

    return false;
}

bool QNModel::moveTextBox(QNTextBoxPtr aTextBox, QNModelPtr aTo){
    bool res = false;
    if(aTo != nullptr){
        if(this->textboxes != nullptr)
            res = this->textboxes->removeOne(aTextBox);

        if(res)
            aTo->addTextBox(aTextBox);
    }

    return res;
}
