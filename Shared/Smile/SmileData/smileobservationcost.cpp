/**
 ** @file smileobservationcost.cpp
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

#include "smileobservationcost.h"

/*
 *
 * SmileObscostNode
 *
 */
SmileObscostNode::SmileObscostNode(const QString& aId) {
    this->id = aId;
}

QString SmileObscostNode::getId() const{
    return this->id;
}

QStringList SmileObscostNode::getParents() const {
    return this->parents;
}

void SmileObscostNode::addParent(const QString& aParent){
    this->parents.append(aParent);
}

DoubleList SmileObscostNode::getCosts() const{
    return this->costs;
}

void SmileObscostNode::addCost(const double aCost){
    this->costs.append(aCost);
}

QString SmileObscostNode::toString(){
    QString p = "";
    foreach (QString s, this->parents) {
        p.append(s + "");
    }

    QString c = "";
    foreach (double d, this->costs) {
        c.append(QString::number(d));
    }

    QString s = QString("\t-observationcost node:\n"
                        "\t\t-id: %1\n"
                        "\t\t-parents: %2\n"
                        "\t\t-costs: %3")
                .arg(this->id).arg(p).arg(c);

    return s;
}

bool SmileObscostNode::operator==(const SmileObscostNode& aOther) const{
    return this->id == aOther.id &&
            this->parents == aOther.parents &&
            this->costs == aOther.costs;
}

bool SmileObscostNode::operator!=(const SmileObscostNode& aOther) const{
    return !this->operator==(aOther);
}

bool SmileObscostNode::operator<(const SmileObscostNode& aOther) const{
    if(this->id == aOther.id)
        if(this->parents == aOther.parents)
            if(this->costs == aOther.costs)
                return false;
            else
                return this->costs < aOther.costs;
        else
            return this->parents < aOther.parents;
    else
        return this->id < aOther.id;
}

/*
 *
 * SmileObservationcost
 *
 */
SmileObservationcost::SmileObservationcost(){
    this->nodes.reset();
}

SmileObservationcost::~SmileObservationcost(){
    this->nodes.reset();
}

SmileObservationcost::SmileObservationcost(const SmileObservationcost& aOther){
    if(aOther.nodes != nullptr){
        this->nodes.reset(new SmileObscostNodeList());

        for(SmileObscostNodeList::iterator it = aOther.nodes->begin(); it != aOther.nodes->end(); it++){
            SmileObscostNode tmp(*it);
            this->addNode(tmp);
        }
    }
}

const SmileObservationcost& SmileObservationcost::operator=(const SmileObservationcost& aOther){
    if(this != &aOther){
        if(aOther.nodes != nullptr){
            this->nodes.reset(new SmileObscostNodeList());

            for(SmileObscostNodeList::iterator it = aOther.nodes->begin(); it != aOther.nodes->end(); it++){
                SmileObscostNode tmp(*it);
                this->addNode(tmp);
            }
        }
        else
            this->nodes.reset();
    }
    return *this;
}

void SmileObservationcost::addNode(SmileObscostNode& aNode){
    if(this->nodes == nullptr)
        this->nodes.reset(new SmileObscostNodeList());

    this->nodes->push_back(aNode);
}

SmileObscostNodeListPtr SmileObservationcost::getNodes() const {
    return (this->nodes.isNull() || this->nodes->isEmpty()) ? nullptr : this->nodes.data();
}

QString SmileObservationcost::toString(){
    QString s = "observationcost: ";
    if(this->nodes.isNull())
        s.append("[empty]");
    else{
        for(SmileObscostNodeList::iterator it = this->nodes->begin(); it != this->nodes->end(); it++)
            s.append("\n").append(it->toString());
    }

    return s;
}

bool SmileObservationcost::operator==(const SmileObservationcost& aOther) const{
    if(this->nodes.isNull())
        return aOther.nodes.isNull();
    else if(aOther.nodes.isNull())
        return false;
    else
        return *(this->nodes.data()) == *(aOther.nodes.data());
}

bool SmileObservationcost::operator!=(const SmileObservationcost& aOther) const{
    return !this->operator==(aOther);
}

bool SmileObservationcost::operator<(const SmileObservationcost& aOther) const{
    if(this->nodes.isNull())
        return aOther.nodes.isNull();
    else if(aOther.nodes.isNull())
        return false;
    else
        return *(this->nodes.data()) < *(aOther.nodes.data());
}
