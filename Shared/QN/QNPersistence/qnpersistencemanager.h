/**
 ** @file qnpersistencemanager.h
 ** @author Angelo Prudentino
 ** @date 2016-12-06
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

#ifndef QNPERSISTENCEMANAGER_H
#define QNPERSISTENCEMANAGER_H

#include "qnpersistenceinterface.h"
#include <QDir>

/**
 * @brief The QNPersistenceManager class models the manager of all persistency plugins
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNPersistenceManager: public QNPersistenceInterface{
private:
    Q_DISABLE_COPY(QNPersistenceManager)

    /**
     * @brief persistency plugins instances
     */
    ScopedQNPersistenceList workers;

public:
    /**
     * @brief QNPersistenceManager constructor
     * @param aPluginsDir the diretory where plugins can be found
     */
    QNPersistenceManager(const QDir& aPluginsDir);

    /**
     * @brief QNPersistenceManager destructor
     */
    ~QNPersistenceManager();

    /**
     * @brief supportedFormats custom implementation
     * @see QNPersistenceInterface
     */
    QRegExpList supportedFormats() Q_DECL_OVERRIDE;

    /**
     * @brief isSupported custom implementation
     * @see QNPersistenceInterface
     */
    bool isSupported(const QString& aFile) Q_DECL_OVERRIDE;

    /**
     * @brief load custom implementation
     * @see QNPersistenceInterface
     */
    QNError load(const QString& aFile, ScopedQNNetwork& aNet) Q_DECL_OVERRIDE;

    /**
     * @brief store custom implementation
     * @see QNPersistenceInterface
     */
    QNError store(const QString& aFile, const ScopedQNNetwork& aNet) Q_DECL_OVERRIDE;
};

#endif // QNPERSISTENCEMANAGER_H
