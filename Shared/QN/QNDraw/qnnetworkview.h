/**
 ** @file qnnetworkview.h
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

#ifndef QNNETWORKVIEW_H
#define QNNETWORKVIEW_H

#include "qnglobalexport.h"
#include "qndrawtypes.h"
#include <QObject>
#include <QGraphicsView>

#define DEFAULT_ZOOM_STEP 0.1
#define DEFAULT_ROTATE_STEP 10

/**
 * @brief The QNNetworkView class models the view in which the network's scene will be rendered
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNetworkView: public QGraphicsView{
    Q_OBJECT

private:
    Q_DISABLE_COPY(QNNetworkView)

    /**
     * @brief current zoom level
     */
    double zoomLevel = 0;

    /**
     * @brief current rotation
     */
    double rotateLevel = 0;

    /**
     * @brief updates the view's matrix
     */
    void updateMatrix();

protected:
#ifndef QT_NO_WHEELEVENT
    /**
     * @brief wheelEvent custom implementation
     * @param aEvent the event to be managed
     */
    void wheelEvent(QWheelEventPtr aEvent) Q_DECL_OVERRIDE;
#endif

public:
    /**
     * @brief QNNetworkView constructor
     * @param aScene the contained scene
     * @param aParent the parent of this view
     */
    QNNetworkView(QNNetworkScenePtr aScene, QWidgetPtr aParent = Q_NULLPTR);

public slots:
    /**
     * @brief restores zoom level to its default
     */
    void restoreZoom();

    /**
     * @brief increases the zoom level of a specified value
     * @param aValue zoom increase value
     */
    void zoomIn(const double aValue = DEFAULT_ZOOM_STEP);

    /**
     * @brief decreases the zoom level of a specified value
     * @param aValue zoom decrease value
     */
    void zoomOut(const double aValue = DEFAULT_ZOOM_STEP);

    /**
     * @brief restores orientation to its default
     */
    void restoreRotate();

    /**
     * @brief right-rotates the view of a specified value
     * @param aValue right rotation value
     */
    void rotateRight(const double aValue = DEFAULT_ROTATE_STEP);

    /**
     * @brief left-rotates the view of a specified value
     * @param aValue left rotation value
     */
    void rotateLeft(const double aValue = DEFAULT_ROTATE_STEP);
};

#endif // QNNETWORKVIEW_H
