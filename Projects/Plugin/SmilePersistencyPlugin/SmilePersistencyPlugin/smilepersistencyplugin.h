/**
 ** @file smilepersistencyplugin.h
 ** @author Angelo Prudentino
 ** @date 2016-11-09
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

#ifndef SMILEPERSISTENCYPLUGIN_H
#define SMILEPERSISTENCYPLUGIN_H

#include "smilepersistencyplugin_global.h"
#include "qnpersistenceinterface.h"

/**
 * @brief The SmilePersistencyPlugin class implements a persistency plugin for the smile xml format
 */
class SMILEPERSISTENCYPLUGINSHARED_EXPORT SmilePersistencyPlugin: public QObject, public QNPersistenceInterface{
private:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QNPersistenceInterface_iid)
    Q_INTERFACES(QNPersistenceInterface)

    /**
     * @brief supported files pattern
     */
    QRegExp supportedPattern;

public:
    /**
     * @brief SmilePersistencyPlugin constructor
     */
    SmilePersistencyPlugin();

    /**
     * @see QNPersistenceInterface
     */
    QRegExpList supportedFormats() Q_DECL_OVERRIDE;

    /**
     * @see QNPersistenceInterface
     */
    bool isSupported(const QString& aFile) Q_DECL_OVERRIDE;

    /**
     * @see QNPersistenceInterface
     */
    QNError load(const QString& aFile, ScopedQNNetwork& aNet) Q_DECL_OVERRIDE;

    /**
     * @see QNPersistenceInterface
     */
    QNError store(const QString& aFile, const ScopedQNNetwork& aNet) Q_DECL_OVERRIDE;
};

#endif // SMILEPERSISTENCYPLUGIN_H
