/**
 ** @file qnpersistencetypes.h
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

#ifndef QNPERSISTENCETYPES_H
#define QNPERSISTENCETYPES_H

#include <QScopedPointer>
#include <QList>
#include <QRegExp>

class QNPersistenceInterface;
class QNPersistenceManager;


/**
 * @brief Represents a pointer to a persistency plugin
 * @see QNPersistenceInterface
 */
typedef QNPersistenceInterface* QNPersistenceInterfacePtr;

/**
 * @brief Represents a list of persistency plugins
 * @see QNPersistenceInterfacePtr
 */
typedef QList<QNPersistenceInterfacePtr> QNPersistenceList;

/**
 * @brief Represents a unique pointer to a list of persistency plugins
 * @see QNPersistenceList
 */
typedef QScopedPointer<QNPersistenceList> ScopedQNPersistenceList;

/**
 * @brief Represents a pointer to a persistency manager instance
 * @see QNPersistenceManager
 */
typedef QNPersistenceManager* QNPersistenceManagerPtr;

/**
 * @brief Represent a list of regular expressions
 * @see QRegExp
 */
typedef QList<QRegExp> QRegExpList;

#endif // QNPERSISTENCETYPES_H
