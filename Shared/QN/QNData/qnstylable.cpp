/**
 ** @file qnstylable.cpp
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

#include "qnstylable.h"

QNStylable::QNStylable(){
    this->classes.reset();
}

QNStylable::~QNStylable(){
    this->classes.reset();
}

bool QNStylable::operator==(const QNStylable& aOther) const{
    return (this->classes.isNull() && aOther.classes.isNull()) ||
            (!this->classes.isNull() && !aOther.classes.isNull() && *(this->classes.data()) == *(aOther.classes.data()));
}

bool QNStylable::operator!=(const QNStylable& aOther) const{
    return !this->operator==(aOther);
}

QStringListPtr QNStylable::getClasses() const{
    return (this->classes.isNull() || this->classes->isEmpty()) ? nullptr : this->classes.data();
}

bool QNStylable::hasClass(const QString& aClass) const{
    return (this->classes.isNull() || this->classes->isEmpty()) ? false : this->classes->contains(aClass);
}

void QNStylable::resetClasses(){
    this->classes.reset();
}

void QNStylable::initClasses(const QString& aClass){
    if(!aClass.isEmpty()){
        if(!this->classes.isNull())
            this->classes->clear();

        this->addClass(aClass);
    }
}

void QNStylable::addClass(const QString& aClass){
    if(!aClass.isEmpty()){
        if(this->classes.isNull())
            this->classes.reset(new QStringList());

        if(!this->classes->contains(aClass))
            this->classes->push_back(aClass);
    }
}

void QNStylable::remClass(const QString& aClass){
    if(!aClass.isEmpty() && !this->classes.isNull())
        this->classes->removeOne(aClass);
}
