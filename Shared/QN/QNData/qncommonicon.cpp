/**
 ** @file qncommonicon.cpp
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

#include "qnfont.h"
#include "qnposition.h"
#include "qncommonicon.h"

QNCommonIcon::QNCommonIcon(){
    this->setComment("");
    this->setProperty("empty", "");
    this->pos.reset();
    this->font.reset();
}

QNCommonIcon::~QNCommonIcon(){
    this->pos.reset();
    this->font.reset();
}

bool QNCommonIcon::operator==(const QNCommonIcon& aOther) const{
    bool result = false;
    if(this->labelLen == aOther.labelLen &&
       this->showContent == aOther.showContent &&
       this->propToShow == aOther.propToShow &&
       this->comment == aOther.comment)
        if((this->pos.isNull() && aOther.pos.isNull()) ||
           (!this->pos.isNull() && !aOther.pos.isNull() && *(this->pos.data()) == *(aOther.pos.data())))
            result = (this->font.isNull() && aOther.font.isNull()) || (!this->font.isNull() && !aOther.font.isNull() && *(this->font.data()) == *(aOther.font.data()));

    return result && QNProperties::operator==(aOther);
}

bool QNCommonIcon::operator!=(const QNCommonIcon& aOther) const{
    return !this->operator==(aOther);
}

QString QNCommonIcon::getComment() const{
    return this->comment;
}

bool QNCommonIcon::isShowingContent() const{
    return this->showContent;
}

QString QNCommonIcon::getPropToShow() const{
    return this->propToShow;
}

int QNCommonIcon::getLabelLen() const{
    return this->labelLen;
}

QNPositionPtr QNCommonIcon::getPosition() const{
    return (this->pos.isNull())? nullptr : this->pos.data();
}

QNFontPtr QNCommonIcon::getFont() const{
    return (this->font.isNull())? nullptr : this->font.data();
}

void QNCommonIcon::setComment(const QString& aValue){
    this->comment = aValue;
}

void QNCommonIcon::setShowContent(const bool aValue){
    this->showContent = aValue;
}

void QNCommonIcon::updatePropToShow(const QString& aProp){
    if(!aProp.isEmpty())
        this->propToShow = aProp;
}

void QNCommonIcon::setLabelLen(int aValue){
    this->labelLen = aValue;
}

void QNCommonIcon::setPosition(QNPositionPtr aValue){
    if(aValue != nullptr)
        this->pos.reset(aValue);
}

void QNCommonIcon::setFont(QNFontPtr aValue){
    if(aValue != nullptr)
        this->font.reset(aValue);
}

QStringList QNCommonIcon::getAllKeys() const{
    QStringList res = QNProperties::getAllKeys();
    res.push_front("comment");
    return res;
}

QVariant QNCommonIcon::getProperty(const QString& aKey) const{
    if(aKey == "comment")
        return this->getComment();
    else
        return QNProperties::getProperty(aKey);
}

QNError QNCommonIcon::setProperty(const QString& aKey, const QVariant& aValue){
    if(aKey == "comment"){
        this->setComment(aValue.toString());
        return QNError();
    }
    else
        return QNProperties::setProperty(aKey, aValue);
}
