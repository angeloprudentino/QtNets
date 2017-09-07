/**
 ** @file qnfont.h
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

#ifndef QNFONT_H
#define QNFONT_H

#include <QMetaType>
#include "qntypes.h"
#include "optionalbool.h"
#include "qnglobalexport.h"

/**
 * @brief The QNAlignment enum enumerates possible  values for text alignment property
 */
enum QNAlignment{
    undefAlign, ///undefined
    leftAlign, ///left-aligned
    centerAlign, ///center-aligned
    rightAlign ///right-aligned
};

/**
 * @brief The QNFont class models a font used to draw text in the plot
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNFont {
private:
    Q_DISABLE_COPY(QNFont)

    /**
     * @brief font family name
     * @note if the specified font family is not available, a default font will be used
     */
    QString fontName;

    /**
     * @brief size of the font
     * @note if the specified font size is not supported, a default size will be used
     */
    int fontSize;

    /**
     * @brief color of the font
     * @note if the specified font color is not supported, a default color will be used
     */
    QString fontColor;

    /**
     * @brief font's weigth
     */
    OptionalBool bold;

    /**
     * @brief font's style
     */
    OptionalBool italic;

    /**
     * @brief text alignment
     * @note default value is undefined
     */
    QNAlignment align = QNAlignment::undefAlign;

public:
    /**
     * @brief QNFont constructor
     * @param aFontName the font family name
     * @param aFontSize the font size
     * @param aFontColor the font color
     */
    QNFont(const QString& aFontName, const int aFontSize, const QString& aFontColor);

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNFont& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNFont& aOther) const;

    /**
     * @brief retrieves the font's name
     * @return the font's name
     */
    QString getFontName() const;

    /**
     * @brief retrieves the font's size
     * @return the font's size
     */
    int getFontSize() const;

    /**
     * @brief retrieves the font's color
     * @return the font's color
     */
    QString getFontColor() const;

    /**
     * @brief retrieves the font's weigth
     * @return true if the font is bold
     */
    OptionalBool isBold() const;

    /**
     * @brief retrieves the font's style
     * @return true if the font is italic
     */
    OptionalBool isItalic() const;

    /**
     * @brief retrieves the text alignment
     * @return the text alignment
     */
    QNAlignment getAlign() const;

    /**
     * @brief updates the font's name
     * @param value the new value for the font's name
     * @note if value is empty, the request will be ignored
     */
    void setFontName(const QString& value);

    /**
     * @brief updates the font's size
     * @param value the new value for the font's size
     */
    void setFontSize(int value);

    /**
     * @brief updates the font's color
     * @param value the new value for the font's color
     * @note if value is empty, the request will be ignored
     */
    void setFontColor(const QString& value);

    /**
     * @brief updates the font's weight
     * @param value the new value for the (is)bold property
     */
    void setBold(bool value);

    /**
     * @brief updates the font's style
     * @param value the new value for the (is)italic property
     */
    void setItalic(bool value);

    /**
     * @brief updates the text alignment property
     * @param value the new value for the text alignment property
     */
    void setAlign(const QNAlignment& value);
};

#endif // QNFONT_H
