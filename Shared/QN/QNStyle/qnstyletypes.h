/**
 ** @file qnstyletypes.h
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

#ifndef QNSTYLETYPES_H
#define QNSTYLETYPES_H

#include <QObject>
#include <QScopedPointer>
#include <QList>
#include <QRegExp>
#include <QMap>


class QNStyle;
class QNStyleClass;
class QNStyleInterface;
class QNStyleManagerInterface;


/**
 * @brief Represents a pointer to a style object
 * @see QNStyle
 */
typedef QNStyle* QNStylePtr;

/**
 * @brief Represents a unique pointer to a style object
 * @see QNStyle
 */
typedef QScopedPointer<QNStyle> ScopedQNStyle;

/**
 * @brief Represents a pointer to a style class
 * @see QNStyleClass
 */
typedef QNStyleClass* QNStyleClassPtr;

/**
 * @brief Represents a list of style classes
 * @see QNStyleClassPtr
 */
typedef QList<QNStyleClassPtr> QNStyleClassList;

/**
 * @brief Represents a map with strings as keys and style classes as values
 * @see QNStyleClassPtr
 */
typedef QMap<QString, QNStyleClassPtr> QNStyleClassMap;

/**
 * @brief Represents a pointer to a map of style classes
 * @see QNStyleClassMap
 */
typedef QNStyleClassMap* QNStyleClassMapPtr;

/**
 * @brief Represents a unique pointer to a QNStyleClassMap
 * @see QNStyleClassMap
 */
typedef QScopedPointer<QNStyleClassMap> ScopedQNStyleClassMap;

/**
 * @brief Represents a pointer to a style plugin
 * @see QNStyleInterface
 */
typedef QNStyleInterface* QNStyleInterfacePtr;

/**
 * @brief Represents a list of style plugins
 * @see QNStyleInterfacePtr
 */
typedef QList<QNStyleInterfacePtr> QNStyleList;

/**
 * @brief Represents a unique pointer to a list of style plugins
 * @see QNStyleList
 */
typedef QScopedPointer<QNStyleList> ScopedQNStyleList;

/**
 * @brief Represents a pointer to a style manager instance
 * @see QNStyleManagerInterface
 */
typedef QNStyleManagerInterface* QNStyleManagerPtr;

/**
 * @brief Represent a list of regular expressions
 * @see QRegExp
 */
typedef QList<QRegExp> QRegExpList;

#endif // QNSTYLETYPES_H
