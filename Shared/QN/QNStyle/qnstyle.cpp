/**
 ** @file qnstyle.cpp
 ** @author Angelo Prudentino
 ** @date 2016-12-29
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

#include "qnstyle.h"
#include "qnstyleclass.h"


QNStyle::QNStyle(const QString& aId, const QString& aVersion){
    this->id = aId;
    this->version = aVersion;
    this->classes.reset();
}

QNStyle::~QNStyle(){
    if(!this->classes.isNull()){
        QNStyleClassList l = this->classes->values();
        int size = l.size();
        for(int i=0; i<size; i++){
            if(l.at(i) != nullptr)
                delete l.at(i);
        }
        this->classes->clear();
        this->classes.reset();
    }
}

bool QNStyle::operator==(const QNStyle& aOther) const{
    if(this->id == aOther.id)
        if(this->version == aOther.version){
            if(this->classes.isNull() && aOther.classes.isNull())
                return true;

            if(!(!this->classes.isNull() && !aOther.classes.isNull()))
                return false;

            QStringList keys = this->getAllClassesNames();
            foreach (QString k, keys) {
                QNStyleClassPtr c1 = this->getClass(k);
                QNStyleClassPtr c2 = aOther.getClass(k);
                if(c2 == nullptr || !(*c1 == *c2))
                    return false;
            }

            return true;
        }

    return false;
}

QString QNStyle::getId() const{
    return this->id;
}

void QNStyle::setId(const QString& aValue){
    if(!aValue.isEmpty())
        this->id = aValue;
}

QString QNStyle::getVersion() const{
    return this->version;
}

void QNStyle::setVersion(const QString& aValue){
    if(!aValue.isEmpty())
        this->version = aValue;
}

QNStyleClassPtr QNStyle::getClass(const QString& aName) const{
    return (this->classes.isNull() || !this->classes->contains(aName))? nullptr: this->classes->value(aName);
}

QStringList QNStyle::getAllClassesNames() const{
    return (this->classes.isNull())? QStringList(): this->classes->keys();
}

QNStyleClassPtr QNStyle::addClass(QNStyleClassPtr aClass){
    if(aClass != nullptr){
        if(this->classes.isNull())
            this->classes.reset(new QNStyleClassMap());

        //check if class already in the map
        if(this->classes->contains(aClass->getName())){
            QNStyleClassPtr c = this->classes->value(aClass->getName());

            //fuse the two instances of the same class
            QStringList keys = aClass->getAllKeys();
            for(QString k: keys){
                QVariant qv = aClass->getProperty(k);
                c->setProperty(k, qv);
            }
            delete aClass;
            return c;
        }
        else
            this->classes->insert(aClass->getName(), aClass);
    }

    return aClass;
}

void QNStyle::removeClass(const QString& aName){
    if(!this->classes.isNull() && this->classes->contains(aName)){
        QNStyleClassPtr c = this->classes->take(aName);
        delete c;
    }
}

QNStyleClassPtr QNStyle::moveClass(const QString& aOldName, const QString& aNewName){
    if(!aOldName.isEmpty() && !aNewName.isEmpty() && aOldName != aNewName){
        if(!this->classes.isNull() && this->classes->contains(aOldName)){
            QNStyleClassPtr c = this->classes->take(aOldName);
            c->setName(aNewName);
            return this->addClass(c);
        }
        else
            return nullptr;
    }
    else
        return nullptr;
}
