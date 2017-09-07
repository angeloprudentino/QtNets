/**
 ** @file qnposition.h
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

#ifndef QNPOSITION_H
#define QNPOSITION_H

#include "qntypes.h"
#include "qnglobalexport.h"

/**
 * @brief The QNPosition class models the position of an item in its container item
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNPosition{
private:
    Q_DISABLE_COPY(QNPosition)

    /**
     * @brief left the top x coordinate
     */
    int left;

    /**
     * @brief top the top y coordinate
     */
    int top;

    /**
     * @brief right the bottom x coordinate
     */
    int right;

    /**
     * @brief bottom the bottom y coordinate
     */
    int bottom;

public:
    /**
     * @brief QNPosition constructor
     * @param aLeft the top x coordinate
     * @param aTop the top y coordinate
     * @param aRight the bottom x coordinate
     * @param aBottom the bottom y coordinate
     */
    QNPosition(const int aLeft, const int aTop, const int aRight, const int aBottom);

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNPosition& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNPosition& aOther) const;

    /**
     * @brief retrieves the bottom x coordinate
     * @return the bottom x coordinate
     */
    int getLeft() const;

    /**
     * @brief retrieves the bottom y coordinate
     * @return the bottom y coordinate
     */
    int getBottom() const;

    /**
     * @brief retrieves the top x coordinate
     * @return the top x coordinate
     */
    int getRight() const;

    /**
     * @brief retrieves the top y coordinate
     * @return the top y coordinate
     */
    int getTop() const;

    /**
     * @brief updates the bottom x coordinate
     * @param value the new value for the bottom x coordinate
     */
    void setLeft(int value);

    /**
     * @brief updates the bottom y coordinate
     * @param value the new value for the bottom y coordinate
     */
    void setBottom(int value);

    /**
     * @brief updates the top x coordinate
     * @param value the new value for the top x coordinate
     */
    void setRight(int value);

    /**
     * @brief updates the top y coordinate
     * @param value the new value for the top y coordinate
     */
    void setTop(int value);
};

#endif // QNPOSITION_H
