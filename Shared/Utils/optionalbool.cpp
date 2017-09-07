/**
 ** @file optionalbool.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-09
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

#include "optionalbool.h"

OptionalBool::OptionalBool(){
    this->valid = false;
}

OptionalBool::OptionalBool(const bool aValue) {
    this->valid = true;
    this->value = aValue;
}

OptionalBool::OptionalBool(const OptionalBool& aNewValue){
    this->valid = aNewValue.isValid();
    this->value = aNewValue;
}

OptionalBool& OptionalBool::operator=(const OptionalBool& aNewValue){
    if(this != &aNewValue){
        this->valid = aNewValue.isValid();
        this->value = aNewValue;
    }
    return *this;
}

bool OptionalBool::isValid() const {
    return this->valid;
}

bool OptionalBool::operator==(const OptionalBool& aOther) const{
    return this->valid == aOther.valid && this->value == aOther.value;
}

bool OptionalBool::operator!=(const OptionalBool& aOther) const{
    return !(this->operator==(aOther));
}

OptionalBool::operator bool() const{
    return this->value;
}
