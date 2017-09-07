/**
 ** @file qnpersistenceinterface.h
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

#ifndef QNPERSISTENCEINTERFACE_H
#define QNPERSISTENCEINTERFACE_H

#include "qntypes.h"
#include "qnpersistencetypes.h"
#include "qnerror.h"

/**
 * @brief The QNPersistenceInterface class represents the interface that a class must
 * implement to manage persistency issues
 */
class QNPersistenceInterface{
public:
    /**
     * @brief QNPersistenceInterface destructor
     */
    virtual ~QNPersistenceInterface() {}

    /**
     * @brief retrieves the list of supported file formats
     * @return the list of supported file formats
     * @note each element of the returned list will be of the "*.ext" form
     */
    virtual QRegExpList supportedFormats() = 0;

    /**
     * @brief tells if if the given file is supported
     * @param aFile the file name to be checked
     * @return true if the file format is supported
     */
    virtual bool isSupported(const QString& aFile) = 0;

    /**
     * @brief read the file and populates the ScopedQNNetwork data structure
     * with the contained graph
     * @param aFile the file to be read
     * @param aNet the data structure to be populated
     * @return the error object resulting of the loading operation
     * @note aNet will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     */
    virtual QNError load(const QString& aFile, ScopedQNNetwork& aNet) = 0;

    /**
     * @brief writes the content of the ScopedQNNetwork data structure onto the given
     * file in a proper format
     * @param aFile the destination file
     * @param aNet the data structure to be persisted on file
     * @return the error object resulting of the storing operation
     */
    virtual QNError store(const QString& aFile, const ScopedQNNetwork& aNet) = 0;
};

#define QNPersistenceInterface_iid "QtNetsPersistence.QNPersistenceInterface"
Q_DECLARE_INTERFACE(QNPersistenceInterface, QNPersistenceInterface_iid)

#endif // QNPERSISTENCEINTERFACE_H
