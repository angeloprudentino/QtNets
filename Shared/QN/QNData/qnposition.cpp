/**
 ** @file qnposition.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-15
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

#include "qnposition.h"

QNPosition::QNPosition(const int aLeft, const int aTop, const int aRight, const int aBottom) {
    this->left = aLeft;
    this->top = aTop;
    this->right = aRight;
    this->bottom = aBottom;
}

bool QNPosition::operator==(const QNPosition& aOther) const{
    return this->top == aOther.top &&
           this->left == aOther.left &&
           this->bottom == aOther.bottom &&
            this->right == aOther.right;
}

bool QNPosition::operator!=(const QNPosition& aOther) const{
    return !this->operator==(aOther);
}

int QNPosition::getLeft() const{
    return this->left;
}

int QNPosition::getBottom() const{
    return this->bottom;
}

int QNPosition::getRight() const{
    return this->right;
}

int QNPosition::getTop() const{
    return this->top;
}

void QNPosition::setLeft(int value){
    this->left = value;
}

void QNPosition::setBottom(int value){
    this->bottom = value;
}

void QNPosition::setRight(int value){
    this->right = value;
}

void QNPosition::setTop(int value){
    this->top = value;
}
