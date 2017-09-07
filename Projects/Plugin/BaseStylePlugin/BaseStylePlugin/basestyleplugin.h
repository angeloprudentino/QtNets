/**
 ** @file basestyleplugin.h
 ** @author Angelo Prudentino
 ** @date 2017-01-11
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

#ifndef BASESTYLEPLUGIN_H
#define BASESTYLEPLUGIN_H

#include "basestyleplugin_global.h"
#include "qnstyleinterface.h"
#include "basestyletypes.h"

/**
 * @brief The BaseStylePlugin class implements a basic style plugin
 */
class BASESTYLEPLUGINSHARED_EXPORT BaseStylePlugin: public QObject, public QNStyleInterface{
private:
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QNStyleInterface_iid)
    Q_INTERFACES(QNStyleInterface)

    /**
     * @brief supported files pattern
     */
    QRegExp supportedPattern;

public:
    /**
     * @brief BaseStylePlugin constructor
     */
    BaseStylePlugin();

    /**
     * @brief supportedFormats custom implementation
     * @see QNStyleInterface
     */
    QRegExpList supportedFormats() Q_DECL_OVERRIDE;

    /**
     * @brief isSupported custom implementation
     * @see QNStyleInterface
     */
    bool isSupported(const QString& aFile) Q_DECL_OVERRIDE;

    /**
     * @brief load custom implementation
     * @see QNStyleInterface
     */
    QNError load(const QString& aFile, ScopedQNStyle& aStyle) Q_DECL_OVERRIDE;

    /**
     * @brief store custom implementation
     * @see QNStyleInterface
     */
    QNError store(const QString& aFile, const ScopedQNStyle& aStyle) Q_DECL_OVERRIDE;

    /**
     * @brief updateFont custom implementation
     * @see QNStyleInterface
     */
    QNError updateFont(QNStylePtr aStyle, QGraphicsItemPtr aItem) Q_DECL_OVERRIDE;

    /**
     * @brief paint custom implementation
     * @see QNStyleInterface
     */
    QNError paint(QNStylePtr aStyle, QGraphicsItemPtr aItem,
                  QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget) Q_DECL_OVERRIDE;
};

#endif // BASESTYLEPLUGIN_H
