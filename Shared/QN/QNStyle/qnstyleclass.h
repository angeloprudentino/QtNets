/**
 ** @file qnstyleclass.h
 ** @author Angelo Prudentino
 ** @date 2016-12-28
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

#ifndef QNSTYLECLASS_H
#define QNSTYLECLASS_H

#include "qntypes.h"
#include "qnglobalexport.h"
#include "qnproperties.h"

/**
 * @brief The QNStyleClass class models a style class with style properties
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNStyleClass: public QNProperties{
private:
    Q_DISABLE_COPY(QNStyleClass)

    /**
     * @brief class name
     */
    QString name;

public:
    /**
     * @brief QNStyleClass constructor
     * @param aName the name for this class
     */
    QNStyleClass(const QString& aName);

    /**
     * @brief QNStyleClass destructor
     */
    ~QNStyleClass();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNStyleClass& aOther) const;

    /**
     * @brief retrieves class name
     * @return the name of this class
     */
    QString getName() const;

    /**
     * @brief updates class name
     * @param aName the new name
     */
    void setName(const QString& aName);
};

#endif // QNSTYLECLASS_H
