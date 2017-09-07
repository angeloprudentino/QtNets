/**
 ** @file smilenode.cpp
 ** @author Angelo Prudentino
 ** @date 2016-10-28
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

#include "smilenode.h"

SmileNodeState::SmileNodeState(){

}

QString SmileNodeState::getId() const {
    return this->id;
}

QString SmileNodeState::getLabel() const {
    return this->label;
}

OptionalBool SmileNodeState::getTarget() const {
    return this->target;
}

OptionalBool SmileNodeState::getDefault() const {
    return this->_default;
}

OptionalBool SmileNodeState::getFault() const {
    return this->fault;
}

void SmileNodeState::setId(const QString& aValue) {
    this->id = aValue;
}

void SmileNodeState::setLabel(const QString& aValue){
    this->label = aValue;
}

void SmileNodeState::setTarget(const bool aValue){
    this->target = aValue;
}

void SmileNodeState::setDefault(const bool aValue){
    this->_default = aValue;
}

void SmileNodeState::setFault(const bool aValue){
    this->fault = aValue;
}

QString SmileNodeState::toString(){
    QString s = QString("\t-node state:\n"
                        "\t\t-id: %1\n"
                        "\t\t-label: %2\n"
                        "\t\t-target: %3\n"
                        "\t\t-default: %4\n"
                        "\t\t-fault: %5\n")
                .arg(this->id).arg(this->label).arg(this->target)
                .arg(this->_default).arg(this->fault);

    return s;
}

bool SmileNodeState::operator==(const SmileNodeState& aOther) const{
    return this->id == aOther.id &&
            this->label == aOther.label &&
            this->target == aOther.target &&
            this->_default == aOther._default &&
            this->fault == aOther.fault;
}

bool SmileNodeState::operator!=(const SmileNodeState& aOther) const{
    return !this->operator==(aOther);
}

bool SmileNodeState::operator<(const SmileNodeState& aOther) const{
    if(this->id == aOther.id)
        if(this->label == aOther.label)
            if(this->target == aOther.target)
                if(this->_default == aOther._default)
                    if(this->fault == aOther.fault)
                        return false;
                    else
                        return this->fault < aOther.fault;
                else
                    return this->_default < aOther._default;
            else
                return this->target < aOther.target;
        else
            return this->label < aOther.label;
    else
        return this->id < aOther.id;
}

/*
 *
 * SmileNodeStateList
 *
 */
QString SmileNodeStateList::toString(){
    QString s = "state list:\n";
    int size = this->size();
    for(int i=0; i<size; i++){
        SmileNodeState ns = this->at(i);
        s.append("\t -" + ns.toString() + "\n");
    }

    return s;
}
