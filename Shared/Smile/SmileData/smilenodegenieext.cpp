/**
 ** @file smilenodegenieext.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-16
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

#include "smilenodegenieext.h"

/*
 *
 * GenieTableComment
 *
 */
GenieTableComment::GenieTableComment(){

}

int GenieTableComment::getCol() const{
    return this->col;
}

void GenieTableComment::setCol(int value){
    this->col = value;
}

int GenieTableComment::getRow() const{
    return this->row;
}

void GenieTableComment::setRow(int value){
    this->row = value;
}

QString GenieTableComment::toString(){
    QString s = QString("\t-tableComment:\n"
                        "\t\t-col: %1\n"
                        "\t\t-row: %2\n")
                .arg(this->col).arg(this->row);

    return s;
}

bool GenieTableComment::operator==(const GenieTableComment& aOther) const{
    return this->col == aOther.col && this->row == aOther.row;
}

bool GenieTableComment::operator!=(const GenieTableComment& aOther) const{
    return !this->operator==(aOther);
}

bool GenieTableComment::operator<(const GenieTableComment& aOther) const{
    if(this->col == aOther.col)
        return this->row < aOther.row;
    else
        return this->col < aOther.col;
}


/*
 *
 * GenieLink
 *
 */
GenieLink::GenieLink(){

}

QString GenieLink::getTitle() const{
    return this->title;
}

QString GenieLink::getPath() const{
    return this->path;
}

void GenieLink::setTitle(const QString& value){
    this->title = value;
}

void GenieLink::setPath(const QString& value){
    this->path = value;
}

QString GenieLink::toString(){
    QString s = QString("\t-link:\n"
                        "\t\t-title: %1\n"
                        "\t\t-path: %2\n")
                .arg(this->title).arg(this->path);

    return s;
}

bool GenieLink::operator==(const GenieLink& aOther) const{
    return this->title == aOther.title && this->path == aOther.path;
}

bool GenieLink::operator!=(const GenieLink& aOther) const{
    return !this->operator==(aOther);
}

bool GenieLink::operator<(const GenieLink& aOther) const{
    if(this->title == aOther.title)
        return this->path < aOther.path;
    else
        return this->title < aOther.title;
}

/*
 *
 * GenieNodeState
 *
 */
GenieNodeState::GenieNodeState(){

}

GenieNodeState::GenieNodeState(const GenieNodeState& aOther){
    this->id = aOther.id;
    this->fix = aOther.fix;
    this->faultname = aOther.faultname;
    this->comment = aOther.comment;

    if(aOther.links != nullptr){
        this->links.reset(new GenieLinkList());

        for(GenieLinkList::iterator it = aOther.links->begin(); it != aOther.links->end(); it++){
            GenieLink tmp(*it);
            this->addLink(tmp);
        }
    }
}

GenieNodeState& GenieNodeState::operator=(const GenieNodeState& aOther){
    if(this != &aOther){
        this->id = aOther.id;
        this->fix = aOther.fix;
        this->faultname = aOther.faultname;
        this->comment = aOther.comment;

        if(aOther.links != nullptr){
            this->links.reset(new GenieLinkList());

            for(GenieLinkList::iterator it = aOther.links->begin(); it != aOther.links->end(); it++){
                GenieLink tmp(*it);
                this->addLink(tmp);
            }
        }
    }
    return *this;
}

GenieNodeState::~GenieNodeState(){
    this->links.reset();
}

QString GenieNodeState::getId() const{
    return this->id;
}

QString GenieNodeState::getFaultname() const{
    return this->faultname;
}

QString GenieNodeState::getFix() const{
    return this->fix;
}

QString GenieNodeState::getComment() const{
    return this->comment;
}

GenieLinkListPtr GenieNodeState::getLinks() const{
    return (this->links.isNull() || this->links->isEmpty()) ? nullptr : this->links.data();
}

void GenieNodeState::setId(const QString& value){
    this->id = value;
}

void GenieNodeState::setFaultname(const QString& value){
    this->faultname = value;
}

void GenieNodeState::setFix(const QString& value){
    this->fix = value;
}

void GenieNodeState::setComment(const QString& value){
    this->comment = value;
}

void GenieNodeState::setLinks(GenieLinkListPtr aLinks){
    this->links.reset(aLinks);
}

void GenieNodeState::addLink(const GenieLink& aLink){
    if(this->links == nullptr)
        this->links.reset(new GenieLinkList());

    this->links->push_back(aLink);
}

QString GenieNodeState::toString(){
    QString ls = "[empty]";
    if(!this->links.isNull()){
        ls = "";
        for(GenieLinkList::iterator it = this->links->begin(); it != this->links->end(); it++){
            ls.append(it->toString());
        }
    }

    QString s = QString("\t-node state:\n"
                        "\t\t-id: %1\n"
                        "\t\t-faultname: %2\n"
                        "\t\t-fix: %3\n"
                        "\t\t-comment: %4\n"
                        "\t\t-links: %5")
                .arg(this->id).arg(this->faultname).arg(this->fix)
                .arg(ls);

    return s;
}

bool GenieNodeState::operator==(const GenieNodeState& aOther) const{
    return this->id == aOther.id &&
            this->faultname == aOther.faultname &&
            this->fix == aOther.fix &&
            this->comment == aOther.comment &&
            ((this->links.isNull() && aOther.links.isNull()) ||
             (!this->links.isNull() && !aOther.links.isNull() &&
              *(this->links.data()) == *(aOther.links.data())));
}

bool GenieNodeState::operator!=(const GenieNodeState& aOther) const{
    return !this->operator==(aOther);
}

bool GenieNodeState::operator<(const GenieNodeState& aOther) const{
    if(this->id == aOther.id)
        if(this->faultname == aOther.faultname)
            if(this->fix == aOther.fix)
                if(this->comment == aOther.comment)
                    if((this->links.isNull() && aOther.links.isNull()) ||
                       (!this->links.isNull() && !aOther.links.isNull() &&
                        *(this->links.data()) == *(aOther.links.data())))
                        return false;
                    else
                        return (!this->links.isNull() && !aOther.links.isNull() &&
                                *(this->links.data()) < *(aOther.links.data()));
                else
                    return this->comment < aOther.comment;
            else
                return this->fix < aOther.fix;
        else
            return this->faultname < aOther.faultname;
    else
        return this->id < aOther.id;
}

/*
 *
 * GenieBarchart
 *
 */
GenieBarchart::GenieBarchart(){

}

bool GenieBarchart::isActive() const{
    return this->active;
}

int GenieBarchart::getWidth() const{
    return this->width;
}

int GenieBarchart::getHeight() const{
    return this->height;
}

void GenieBarchart::setActive(bool value){
    this->active = value;
}

void GenieBarchart::setWidth(int value){
    this->width = value;
}

void GenieBarchart::setHeight(int value){
    this->height = value;
}

QString GenieBarchart::toString(){
    QString s = QString("\t-barchart:\n"
                        "\t\t-active: %1\n"
                        "\t\t-width: %2\n"
                        "\t\t-height: %3")
                .arg((this->active)? "true": "false")
                .arg(this->width).arg(this->height);

    return s;
}

bool GenieBarchart::operator==(const GenieBarchart& aOther) const{
    return this->active == aOther.active &&
            this->width == aOther.width &&
            this->height == aOther.height;
}

bool GenieBarchart::operator!=(const GenieBarchart& aOther) const{
    return !this->operator==(aOther);
}

bool GenieBarchart::operator<(const GenieBarchart& aOther) const{
    if(this->active == aOther.active)
        if(this->width == aOther.width)
            if(this->height == aOther.height)
                return false;
            else
                return this->height < aOther.height;
        else
            return this->width < aOther.width;
    else
        return this->active < aOther.active;
}

/*
 *
 * GenieNodeStateList
 *
 */
QString GenieNodeStateList::toString(){
    QString s = "genie state list:\n";
    int size = this->size();
    for(int i=0; i<size; i++){
        GenieNodeState ns = this->at(i);
        s.append("\t -" + ns.toString() + "\n");
    }

    return s;
}

/*
 *
 * GenieTableCommentList
 *
 */
QString GenieTableCommentList::toString(){
    QString s = "genie table comments:\n";
    int size = this->size();
    for(int i=0; i<size; i++){
        GenieTableComment tc = this->at(i);
        s.append("\t -" + tc.toString() + "\n");
    }

    return s;
}

/*
 *
 * GenieLinkList
 *
 */
QString GenieLinkList::toString(){
    QString s = "genie links:\n";
    int size = this->size();
    for(int i=0; i<size; i++){
        GenieLink l = this->at(i);
        s.append("\t -" + l.toString() + "\n");
    }

    return s;
}
