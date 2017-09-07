/**
 ** @file qnstylable.h
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

#ifndef QNSTYLABLE_H
#define QNSTYLABLE_H

#include "qnglobalexport.h"
#include "qntypes.h"


/**
 * @brief The QNStylable class models a set of objects that can be customized by an external plugin
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNStylable {
private:
    Q_DISABLE_COPY(QNStylable)

    /**
     * @brief textbox' style classes
     */
    ScopedStringList classes;

public:

    /**
     * @brief QNStylable constructor
     */
    QNStylable();

    /**
     * @brief QNStylable destructor
     */
    virtual ~QNStylable();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNStylable& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNStylable& aOther) const;

    /**
     * @brief retrieves the list of all classes associated to this object
     * @return the list of all classes associated to this object
     * @note if no classes are associated, null will be returned
     */
    QStringListPtr getClasses() const;

    /**
     * @brief tests if this object is associated to the given class
     * @param aClass the class to be checked
     * @return true if this object is associated to the given class; false otherwise
     */
    bool hasClass(const QString& aClass) const;

    /**
     * @brief removes all classesfrom this object
     */
    void resetClasses();

    /**
     * @brief initializes this object's classes list with the given one
     * @param aClass the class to be used
     * @note previously added classes will be lost
     * @note if aClass is empty, the request will be ignored
     */
    void initClasses(const QString& aClass);

    /**
     * @brief associates a new class to this object
     * @param aClass the class to be added
     * @note if aClass is empty, the request will be ignored
     * @note the class will be added only if not already present
     */
    void addClass(const QString& aClass);

    /**
     * @brief removes the given class from this object
     * @param aClass the class to be removed
     * @note if aClass is empty, the request will be ignored
     */
    void remClass(const QString& aClass);
};
#endif // QNSTYLABLE_H
