/**
 ** @file qnfont.cpp
 ** @author Angelo Prudentino
 ** @date 2016-11-17
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

#include "qnfont.h"


QNFont::QNFont(const QString& aFontName, const int aFontSize, const QString& aFontColor){
    this->fontName = aFontName;
    this->fontSize = aFontSize;
    this->fontColor = aFontColor;
}

bool QNFont::operator==(const QNFont& aOther) const{
    return this->fontName == aOther.fontName &&
           this->fontColor == aOther.fontColor &&
           this->fontSize == aOther.fontSize &&
           this->bold == aOther.bold &&
           this->italic == aOther.italic &&
            this->align == aOther.align;
}

bool QNFont::operator!=(const QNFont& aOther) const{
    return !this->operator==(aOther);
}

QString QNFont::getFontName() const{
    return this->fontName;
}

int QNFont::getFontSize() const{
    return this->fontSize;
}

QString QNFont::getFontColor() const{
    return this->fontColor;
}

OptionalBool QNFont::isBold() const{
    return this->bold;
}

OptionalBool QNFont::isItalic() const{
    return this->italic;
}

QNAlignment QNFont::getAlign() const{
    return this->align;
}

void QNFont::setFontName(const QString& value){
    if(!value.isEmpty())
        this->fontName = value;
}

void QNFont::setFontSize(int value){
    this->fontSize = value;
}

void QNFont::setFontColor(const QString& value){
    if(!value.isEmpty())
        this->fontColor = value;
}

void QNFont::setBold(bool value){
    this->bold = value;
}

void QNFont::setItalic(bool value){
    this->italic = value;
}

void QNFont::setAlign(const QNAlignment& value){
    this->align = value;
}
