/**
 ** @file qntextbox.cpp
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

#include "qntextbox.h"
#include "qnmodel.h"


QNTextBox::QNTextBox(QNModelPtr aContainer){
    this->container = aContainer;
    this->setCaption("");
    this->addClass("textbox");
    this->updatePropToShow("caption");
}

QNTextBox::~QNTextBox(){
    this->container = nullptr;
}

bool QNTextBox::operator==(const QNTextBox& aOther) const{
    bool result = false;
    if(this->caption == aOther.caption)
        result = (this->container==nullptr && aOther.container==nullptr) ||
                 (this->container!=nullptr && aOther.container!=nullptr && this->container->getId() == aOther.container->getId());

    return result && QNCommonIcon::operator==(aOther) && QNStylable::operator==(aOther);
}

bool QNTextBox::operator!=(const QNTextBox& aOther) const{
    return !this->operator==(aOther);
}

QNModelPtr QNTextBox::getContainer() const{
    return this->container;
}

QString QNTextBox::getCaption() const{
    return this-> caption;
}

void QNTextBox::setContainer(QNModelPtr aValue){
    if(aValue != nullptr)
        this->container = aValue;
}

void QNTextBox::setCaption(const QString& aValue){
    this->caption = aValue;
}

QStringList QNTextBox::getAllKeys() const{
    QStringList res = QNCommonIcon::getAllKeys();
    res.push_front("caption");

    return res;
}

QVariant QNTextBox::getProperty(const QString& aKey) const{
    if(aKey == "caption")
        return this->getCaption();
    else
        return QNCommonIcon::getProperty(aKey);
}

QNError QNTextBox::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "caption"){
        this->setCaption(aValue.toString());
        return QNError();
    }
    else
        return QNCommonIcon::setProperty(aKey, aValue);
}
