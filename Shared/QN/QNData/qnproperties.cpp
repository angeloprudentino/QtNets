/**
 ** @file qnproperties.cpp
 ** @author Angelo Prudentino
 ** @date 2017-01-08
 **
 ** Copyright 2017 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
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

#include "qnproperties.h"

QNProperties::QNProperties(){
    this->properties.reset();
}

QNProperties::~QNProperties(){
    if(!this->properties.isNull()){
        this->properties->clear();
        this->properties.reset();
    }
}

bool QNProperties::operator==(const QNProperties& aOther) const{
    if(this->properties.isNull() && aOther.properties.isNull())
        return true;

    if(!(!this->properties.isNull() && !aOther.properties.isNull()))
        return false;

    QStringList keys = this->getAllKeys();
    foreach (QString k, keys) {
        QVariant qv1 = this->getProperty(k);
        QVariant qv2 = aOther.getProperty(k);
        if(qv1 != qv2)
            return false;
    }

    return true;
}

bool QNProperties::operator!=(const QNProperties& aOther) const{
    return !this->operator==(aOther);
}

QStringList QNProperties::getAllKeys() const{
    if(!this->properties.isNull())
        return this->properties->keys();
    else
        return QStringList();
}

QVariant QNProperties::getProperty(const QString &aKey) const{
    if(this->properties.isNull())
        return QVariant();
    else{
        if(this->properties->contains(aKey))
            return this->properties->value(aKey);
        else
            return QVariant();
    }
}

QNError QNProperties::setProperty(const QString &aKey, const QVariant &aValue){
    if(!aKey.isEmpty() && aValue.isValid()){
        if(this->properties.isNull())
            this->properties.reset(new QNVariantMap());

        this->properties->insert(aKey, aValue);
        return QNError();
    }
    return QNError(NO_DATA, "Empty key");
}
