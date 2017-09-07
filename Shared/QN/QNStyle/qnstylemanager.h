/**
 ** @file qnstylemanager.h
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

#ifndef QNSTYLEMANAGER_H
#define QNSTYLEMANAGER_H

#include <QDir>
#include "qnglobalexport.h"
#include "qnstyleinterface.h"
#include "qnstyleclass.h"
#include "qnstyletypes.h"

/**
 * @brief The QNStyleManagerInterface class models the manager of all style plugins
 * @note only one style plugin at a time can be used
 */
class QNStyleManagerInterface{
public:
    virtual ~QNStyleManagerInterface() {}

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
     * @brief resets the style and disables the current plugin
     * @return the error object resulting of this operation
     */
    virtual QNError resetStyle() = 0;

    /**
     * @brief creates a new style sheet and set the current style plugin
     * @param aFile the file to be created
     * @param aStyleId the ID for the new style
     * @param aStyleVersion the version for the new style
     * @return the error object resulting of the file creation
     */
    virtual QNError newStyle(const QString& aFile,
                             const QString& aStyleId = "style1",
                             const QString& aStyleVersion = "1.0") = 0;

    /**
     * @brief reads the file and loads style classes in main memory
     * @param aFile the file to be read
     * @return the error object resulting of the loading operation
     */
    virtual QNError load(const QString& aFile) = 0;

    /**
     * @brief writes all style classes onto the given file in a proper format
     * @param aFile the destination file
     * @return the error object resulting of the storing operation
     */
    virtual QNError store(const QString& aFile) = 0;

    /**
     * @brief updates item's font properties
     * @param aItem the item to be painted
     * @return the error object resulting of the updating operation
     */
    virtual QNError updateFont(QGraphicsItemPtr aItem) = 0;

    /**
     * @brief paint extension to support style properties
     * @param aItem the item to be painted
     * @param aPainter the painter object
     * @param aOption item's style options options
     * @param aWidget NOT USED
     * @return the error object resulting of the painting operation
     */
    virtual QNError paint(QGraphicsItemPtr aItem,
                          QPainterPtr aPainter,
                          QStyleOptionGraphicsItemConstPtr aOption,
                          QWidgetPtr aWidget) = 0;

    /**
     * @brief retrieves a live reference to the style object
     * @return the reference to the current style object
     * @note if no style is present, null is returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    virtual QNStylePtr getStyle() const = 0;
};

/**
 * @brief The QNStyleManager class implements the manager of all style plugins
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNStyleManager: public QNStyleManagerInterface{
private:
    Q_DISABLE_COPY(QNStyleManager)

    /**
     * @brief persistency plugins instances
     */
    ScopedQNStyleList workers;

    /**
     * @brief currently active plugin
     */
    QNStyleInterfacePtr current;

    /**
     * @brief style object
     */
    ScopedQNStyle style;

public:
    /**
     * @brief QNStyleManager constructor
     * @param aPluginsDir the diretory where plugins can be found
     */
    QNStyleManager(const QDir& aPluginsDir);

    /**
     * @brief QNStyleManager destructor
     */
    ~QNStyleManager();

    /**
     * @brief supportedFormats custom implementation
     * @see QNStyleManagerInterface
     */
    QRegExpList supportedFormats() Q_DECL_OVERRIDE;

    /**
     * @brief isSupported custom implementation
     * @see QNStyleManagerInterface
     */
    bool isSupported(const QString& aFile) Q_DECL_OVERRIDE;

    /**
     * @brief resetStyle custom implementation
     * @see QNStyleManagerInterface
     */
    QNError resetStyle() Q_DECL_OVERRIDE;

    /**
     * @brief newStyle custom implementation
     * @see QNStyleManagerInterface
     */
    QNError newStyle(const QString& aFile,
                     const QString& aStyleId = "style1",
                     const QString& aStyleVersion = "1.0") Q_DECL_OVERRIDE;

    /**
     * @brief load custom implementation
     * @see QNStyleManagerInterface
     */
    QNError load(const QString& aFile) Q_DECL_OVERRIDE;

    /**
     * @brief store custom implementation
     * @see QNStyleManagerInterface
     */
    QNError store(const QString& aFile) Q_DECL_OVERRIDE;

    /**
     * @brief updateFont custom implementation
     * @see QNStyleManagerInterface
     */
    QNError updateFont(QGraphicsItemPtr aItem) Q_DECL_OVERRIDE;

    /**
     * @brief paint custom implementation
     * @see QNStyleManagerInterface
     */
    QNError paint(QGraphicsItemPtr aItem,
                  QPainterPtr aPainter,
                  QStyleOptionGraphicsItemConstPtr aOption,
                  QWidgetPtr aWidget) Q_DECL_OVERRIDE;

    /**
     * @brief getStyle custom implementation
     * @see QNStyleManagerInterface
     */
    QNStylePtr getStyle() const Q_DECL_OVERRIDE;
};

#endif // QNSTYLEMANAGER_H
