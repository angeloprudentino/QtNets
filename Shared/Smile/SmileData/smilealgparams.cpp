/**
 ** @file smilealgparams.cpp
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

#include "smilealgparams.h"

/*
 *
 * SmileEpis
 *
 */
SmileEpis::SmileEpis(){

}

int SmileEpis::getProplen() const{
    return this->proplen;
}

int SmileEpis::getNumStates1() const{
    return this->numStates1;
}

int SmileEpis::getNumStates2() const{
    return this->numStates2;
}

int SmileEpis::getNumStates3() const{
    return this->numStates3;
}

double SmileEpis::getEps1() const{
    return this->eps1;
}

double SmileEpis::getEps2() const{
    return this->eps2;
}

double SmileEpis::getEps3() const{
    return this->eps3;
}

double SmileEpis::getEps4() const{
    return this->eps4;
}

void SmileEpis::setProplen(int value){
    this->proplen = value;
}

void SmileEpis::setNumStates1(int value){
    this->numStates1 = value;
}

void SmileEpis::setNumStates2(int value){
    this->numStates2 = value;
}

void SmileEpis::setNumStates3(int value){
    this->numStates3 = value;
}

void SmileEpis::setEps1(double value){
    this->eps1 = value;
}

void SmileEpis::setEps2(double value){
    this->eps2 = value;
}

void SmileEpis::setEps3(double value){
    this->eps3 = value;
}

void SmileEpis::setEps4(double value){
    this->eps4 = value;
}

QString SmileEpis::toString(){
    QString s = QString("epis:\n"
                        "\t\t-proplen: %1\n"
                        "\t\t-numStates1: %2\n"
                        "\t\t-numStates2: %3\n"
                        "\t\t-numStates3: %4\n"
                        "\t\t-eps1: %5\n"
                        "\t\t-eps2: %6\n"
                        "\t\t-eps3: %7\n"
                        "\t\t-eps4: %8")
                .arg(this->proplen).arg(this->numStates1).arg(this->numStates2)
                .arg(this->numStates3).arg(this->eps1).arg(this->eps2).arg(this->eps3).arg(this->eps4);

    return s;
}

bool SmileEpis::operator==(const SmileEpis& aOther) const{
    return this->proplen == aOther.proplen &&
            this->numStates1 == aOther.numStates1 &&
            this->numStates2 == aOther.numStates2 &&
            this->numStates3 == aOther.numStates3 &&
            this->eps1 == aOther.eps1 &&
            this->eps2 == aOther.eps2 &&
            this->eps3 == aOther.eps3 &&
            this->eps4 == aOther.eps4;
}

bool SmileEpis::operator!=(const SmileEpis& aOther) const{
    return !this->operator==(aOther);
}

bool SmileEpis::operator<(const SmileEpis& aOther) const{
    if(this->proplen == aOther.proplen)
        if(this->numStates1 == aOther.numStates1)
            if(this->numStates2 == aOther.numStates2)
                if(this->numStates3 == aOther.numStates3)
                    if(this->eps1 == aOther.eps1)
                        if(this->eps2 == aOther.eps2)
                            if(this->eps3 == aOther.eps3)
                                if(this->eps4 == aOther.eps4)
                                    return false;
                                else
                                    return this->eps4 < aOther.eps4;
                            else
                                return this->eps3 < aOther.eps3;
                        else
                            return this->eps2 < aOther.eps2;
                    else
                        return this->eps1 < aOther.eps1;
                else
                    return this->numStates3 < aOther.numStates3;
            else
                return this->numStates2 < aOther.numStates2;
        else
            return this->numStates1 < aOther.numStates1;
    else
        return this->proplen < aOther.proplen;
}



/*
 *
 * SmileAlgparams
 *
 */
SmileAlgparams::SmileAlgparams(){
    this->epis.reset();
}

SmileAlgparams::~SmileAlgparams(){
    this->epis.reset();
}

SmileAlgparams::SmileAlgparams(const SmileAlgparams& aOther){
    if(aOther.epis != nullptr){
        SmileEpis* e = new SmileEpis();
        e->setProplen(aOther.epis->getProplen());
        e->setNumStates1(aOther.epis->getNumStates1());
        e->setNumStates2(aOther.epis->getNumStates2());
        e->setNumStates3(aOther.epis->getNumStates3());
        e->setEps1(aOther.epis->getEps1());
        e->setEps2(aOther.epis->getEps2());
        e->setEps3(aOther.epis->getEps3());
        e->setEps4(aOther.epis->getEps4());

        this->epis.reset(e);
    }
    else
        this->epis.reset();
}

const SmileAlgparams& SmileAlgparams::operator=(const SmileAlgparams& aOther){
    if(this != &aOther){
        if(aOther.epis != nullptr){
            SmileEpis* e = new SmileEpis();
            e->setProplen(aOther.epis->getProplen());
            e->setNumStates1(aOther.epis->getNumStates1());
            e->setNumStates2(aOther.epis->getNumStates2());
            e->setNumStates3(aOther.epis->getNumStates3());
            e->setEps1(aOther.epis->getEps1());
            e->setEps2(aOther.epis->getEps2());
            e->setEps3(aOther.epis->getEps3());
            e->setEps4(aOther.epis->getEps4());

            this->epis.reset(e);
        }
        else
            this->epis.reset();
    }
    return *this;
}

SmileEpisPtr SmileAlgparams::getEpis(){
    if(this->epis.isNull())
        return nullptr;
    else
        return this->epis.data();
}

void SmileAlgparams::setEpis(ScopedSmileEpis& value){
    this->epis.reset(value.take());
}

QString SmileAlgparams::toString(){
    QString s = "algparams: ";
    if(this->epis.isNull())
        s.append("\n\t-").append(this->epis->toString());
    else
        s.append("[empty]");

    return s;
}

bool SmileAlgparams::operator==(const SmileAlgparams& aOther) const{
    if(this->epis.isNull())
        return aOther.epis.isNull();
    else
        return *(this->epis.data()) == *(aOther.epis.data());
}

bool SmileAlgparams::operator!=(const SmileAlgparams& aOther) const{
    return !this->operator==(aOther);
}

bool SmileAlgparams::operator<(const SmileAlgparams& aOther) const{
    if(this->epis.isNull())
        return aOther.epis.isNull();
    else
        return *(this->epis.data()) < *(aOther.epis.data());
}
