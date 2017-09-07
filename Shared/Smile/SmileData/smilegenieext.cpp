/**
 ** @file smilegenieext.cpp
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

#include "smilegenieext.h"

/*
 *
 * GenieDAF
 *
 */
GenieDAF::GenieDAF(){
}

QString GenieDAF::getTargetnr() const{
    return this->targetnr;
}

QString GenieDAF::getObservation() const{
    return this->observation;
}

QString GenieDAF::getObservationnr() const{
    return this->observationnr;
}

QString GenieDAF::getAuxiliary() const{
    return this->auxiliary;
}


QString GenieDAF::getTarget() const{
    return this->target;
}

void GenieDAF::setTarget(const QString& value){
    this->target = value;
}

void GenieDAF::setTargetnr(const QString& value){
    this->targetnr = value;
}

void GenieDAF::setObservation(const QString& value){
    this->observation = value;
}

void GenieDAF::setObservationnr(const QString& value){
    this->observationnr = value;
}

void GenieDAF::setAuxiliary(const QString& value){
    this->auxiliary = value;
}

QString GenieDAF::toString(){
    QString s = QString("\t-diagautoformat:\n"
                        "\t\t-target: %1\n"
                        "\t\t-targetnr: %2\n"
                        "\t\t-observation: %3\n"
                        "\t\t-observationnr: %4\n"
                        "\t\t-auxiliary: %5\n")
                .arg(this->target).arg(this->targetnr).arg(this->observation)
                .arg(this->observationnr).arg(this->auxiliary);

    return s;
}

bool GenieDAF::operator==(const GenieDAF& aOther) const{
    return this->target == aOther.target &&
            this->targetnr == aOther.targetnr &&
            this->observation == aOther.observation &&
            this->observationnr == aOther.observationnr &&
            this->auxiliary == aOther.auxiliary;
}

bool GenieDAF::operator!=(const GenieDAF& aOther) const{
    return !this->operator==(aOther);
}

bool GenieDAF::operator<(const GenieDAF& aOther) const{
    if(this->target == aOther.target)
        if(this->targetnr == aOther.targetnr)
            if(this->observation == aOther.observation)
                if(this->observationnr == aOther.observationnr)
                    if(this->auxiliary == aOther.auxiliary)
                        return false;
                    else
                        return this->auxiliary < aOther.auxiliary;
                else
                    return this->observationnr < aOther.observationnr;
            else
                return this->observation < aOther.observation;
        else
            return this->targetnr < aOther.targetnr;
    else
        return this->target < aOther.target;
}


/*
 *
 * SmileGenieExt
 *
 */
SmileGenieExt::SmileGenieExt(){
    this->diagautoformat.reset();
}

SmileGenieExt::SmileGenieExt(const SmileGenieExt& aOther){
    this->name = aOther.name;
    this->version = aOther.version;
    this->faultnameformat = aOther.faultnameformat;
    this->comment = aOther.comment;
    if(!aOther.diagautoformat.isNull()){
        this->diagautoformat.reset(new GenieDAF());
        this->diagautoformat->setTarget(aOther.diagautoformat->getTarget());
        this->diagautoformat->setTargetnr(aOther.diagautoformat->getTargetnr());
        this->diagautoformat->setObservation(aOther.diagautoformat->getObservation());
        this->diagautoformat->setObservationnr(aOther.diagautoformat->getObservationnr());
        this->diagautoformat->setAuxiliary(aOther.diagautoformat->getAuxiliary());
    }
}

const SmileGenieExt&SmileGenieExt::operator=(const SmileGenieExt& aOther){
    if(this != &aOther){
        this->name = aOther.name;
        this->version = aOther.version;
        this->faultnameformat = aOther.faultnameformat;
        this->comment = aOther.comment;
        if(!aOther.diagautoformat.isNull()){
            this->diagautoformat.reset(new GenieDAF());
            this->diagautoformat->setTarget(aOther.diagautoformat->getTarget());
            this->diagautoformat->setTargetnr(aOther.diagautoformat->getTargetnr());
            this->diagautoformat->setObservation(aOther.diagautoformat->getObservation());
            this->diagautoformat->setObservationnr(aOther.diagautoformat->getObservationnr());
            this->diagautoformat->setAuxiliary(aOther.diagautoformat->getAuxiliary());
        }
    }
    return *this;
}

SmileGenieExt::~SmileGenieExt(){
    this->diagautoformat.reset();
}

QString SmileGenieExt::getName() const{
    return this->name;
}

QString SmileGenieExt::getVersion() const{
    return this->version;
}

QString SmileGenieExt::getComment() const{
    return this->comment;
}

GenieFNF SmileGenieExt::getFaultnameformat() const{
    return this->faultnameformat;
}

GenieDAFPtr SmileGenieExt::getDiagautoformat() const{
    if(this->diagautoformat.isNull())
        return nullptr;
    else
        return this->diagautoformat.data();
}

void SmileGenieExt::setName(const QString& value){
    this->name = value;
}

void SmileGenieExt::setVersion(const QString& value){
    this->version = value;
}

void SmileGenieExt::setComment(const QString& value){
    this->comment = value;
}

void SmileGenieExt::setFaultnameformat(const GenieFNF& value){
    this->faultnameformat = value;
}

void SmileGenieExt::setDiagautoformat(GenieDAFPtr value){
    this->diagautoformat.reset(value);
}

QString SmileGenieExt::toString(){
    //faultnameformat
    QString fnf;
    if(this->faultnameformat == GenieFNF::node)
        fnf = "node";
    else if(this->faultnameformat == GenieFNF::nodestate)
        fnf = "nodestate";
    else if(this->faultnameformat == GenieFNF::inherit)
        fnf = "inherit";
    else if(this->faultnameformat == GenieFNF::user)
        fnf = "user";
    else
        fnf = "[invalid]";

    //diagautoformat
    QString daf = "[empty]";
    if(!this->diagautoformat.isNull())
        daf = "\n" + this->diagautoformat->toString();

    QString s = QString("genie:\n "
                        "\t-name: %1\n"
                        "\t-version: %2\n"
                        "\t-comment: %3\n"
                        "\t-faultnameformat: %4\n"
                        "\t-diagautoformat: %5\n")
                .arg(this->name).arg(this->version).arg(this->comment)
                .arg(fnf).arg(daf);

    return s;
}

bool SmileGenieExt::operator==(const SmileGenieExt& aOther) const{
    return this->name == aOther.name &&
            this->version == aOther.version &&
            this->comment == aOther.comment &&
            this->faultnameformat == aOther.faultnameformat &&
            ((this->diagautoformat.isNull() && aOther.diagautoformat.isNull()) ||
             (!this->diagautoformat.isNull() && !aOther.diagautoformat.isNull() &&
              *(this->diagautoformat.data()) == *(aOther.diagautoformat.data())));
}

bool SmileGenieExt::operator!=(const SmileGenieExt& aOther) const{
    return !this->operator==(aOther);
}

bool SmileGenieExt::operator<(const SmileGenieExt& aOther) const{
    if(this->name == aOther.name)
        if(this->version == aOther.version)
            if(this->comment == aOther.comment)
                if(this->faultnameformat == aOther.faultnameformat)
                    if((this->diagautoformat.isNull() && aOther.diagautoformat.isNull()) ||
                       (!this->diagautoformat.isNull() && !aOther.diagautoformat.isNull() &&
                        *(this->diagautoformat.data()) == *(aOther.diagautoformat.data())))
                        return false;
                    else
                        return (!this->diagautoformat.isNull() && !aOther.diagautoformat.isNull() &&
                                *(this->diagautoformat.data()) < *(aOther.diagautoformat.data()));
                else
                    return this->faultnameformat < aOther.faultnameformat;
            else
                return this->comment < aOther.comment;
        else
            return this->version < aOther.version;
    else
        return this->name < aOther.name;
}
