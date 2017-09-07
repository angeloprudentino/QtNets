/**
 ** @file qnstyleinterface.h
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

#ifndef QNSTYLEINTERFACE_H
#define QNSTYLEINTERFACE_H

#include "qntypes.h"
#include "qndrawtypes.h"
#include "qnstyletypes.h"
#include "qnerror.h"


/**
 * @brief The QNStyleInterface class represents the interface that a class must
 * implement to manage style issues
 */
class QNStyleInterface{
public:
    /**
     * @brief QNStyleInterface destructor
     */
    virtual ~QNStyleInterface() {}

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
     * @brief reads the file and loads style classes in main memory
     * @param aFile the file to be read
     * @param aStyle the style object to be populated
     * @return the error object resulting of the loading operation
     * @note aStyle will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     */
    virtual QNError load(const QString& aFile, ScopedQNStyle& aStyle) = 0;

    /**
     * @brief writes all style classes onto the given file in a proper format
     * @param aFile the destination file
     * @param aStyle the style object to be stored on file
     * @return the error object resulting of the storing operation
     */
    virtual QNError store(const QString& aFile, const ScopedQNStyle& aStyle) = 0;

    /**
     * @brief updates item's font properties
     * @param aStyle the style object to be used during painting
     * @param aItem the item to be painted
     * @return the error object resulting of the updating operation
     */
    virtual QNError updateFont(QNStylePtr aStyle, QGraphicsItemPtr aItem) = 0;

    /**
     * @brief paint extension to support style properties
     * @param aStyle the style object to be used during painting
     * @param aItem the item to be painted
     * @param aPainter the painter object
     * @param aOption item's style options options
     * @param aWidget NOT USED
     * @return the error object resulting of the painting operation
     */
    virtual QNError paint(QNStylePtr aStyle, QGraphicsItemPtr aItem, QPainterPtr aPainter,
                       QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget = Q_NULLPTR) = 0;
};

#define QNStyleInterface_iid "QtNetsStyle.QNStyleInterface"
Q_DECLARE_INTERFACE(QNStyleInterface, QNStyleInterface_iid)

#endif // QNSTYLEINTERFACE_H
