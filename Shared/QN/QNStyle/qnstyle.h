/**
 ** @file qnstyle.h
 ** @author Angelo Prudentino
 ** @date 2016-12-29
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

#ifndef QNSTYLE_H
#define QNSTYLE_H

#include "qnstyletypes.h"


/**
 * @brief The QNStyle class models a user-defined style for the network
 * @note this class is not copyable
 */
class QNStyle {
private:
    Q_DISABLE_COPY(QNStyle)

    /**
     * @brief style id
     */
    QString id;

    /**
     * @brief style version
     */
    QString version;

    /**
     * @brief map of style classes
     */
    ScopedQNStyleClassMap classes;

public:
    /**
     * @brief QNStyle constructor
     * @param aId style id
     * @param aVersion style version
     */
    QNStyle(const QString& aId, const QString& aVersion);

    /**
     * @brief QNStyle destructor
     */
    ~QNStyle();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNStyle& aOther) const;

    /**
     * @brief retrieves the style's id
     * @return the id of this style object
     */
    QString getId() const;

    /**
     * @brief updates the style's id
     * @param aValue the new id
     * @note if aValue is empty, the request will be ignored
     */
    void setId(const QString& aValue);

    /**
     * @brief retrieves the style's version
     * @return the version of this style object
     */
    QString getVersion() const;

    /**
     * @brief updates the style's version
     * @param aValue the new version
     * @note if aValue is empty, the request will be ignored
     */
    void setVersion(const QString& aValue);

    /**
     * @brief retrieves a style class by name
     * @param aName the name of the required class
     * @return the pointer to the required class
     * @note if no class corresponds to the given name, null will be returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNStyleClassPtr getClass(const QString& aName) const;

    /**
     * @brief retrieves the list of all classes' names
     * @return the list of all classes' names
     */
    QStringList getAllClassesNames() const;

    /**
     * @brief adds a style class
     * @param aClass the class to be added
     * @return the added class or null if no class has been added
     * @note if aClass is null, the request will be ignored
     * @warning if aClass is already in the map, the two instances will be fused
     * and the aClass will be deleted. The reference to the valid class will be returned
     */
    QNStyleClassPtr addClass(QNStyleClassPtr aClass);

    /**
     * @brief removes and deletes a class given its name
     * @param aName the name of the class to be removed
     * @note if no class corresponds to the given name, the request will be ignored
     * @warning the memory space associated to the class will be cleaned,
     * so further uses of the removed class will cause a segmentation fault
     */
    void removeClass(const QString& aName);

    /**
     * @brief updates a class changing its name
     * @param aOldName the old name of the class
     * @param aNewName the new name of the class
     * @return the moved class or null if no class has been found
     * @note if no class corresponds to the given name, the request will be ignored
     * @note if old and/or new class name are empty, the request will be ignored
     */
    QNStyleClassPtr moveClass(const QString& aOldName, const QString& aNewName);
};

#endif // QNSTYLE_H
