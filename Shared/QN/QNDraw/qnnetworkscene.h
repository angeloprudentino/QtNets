/**
 ** @file qnnetworkscene.h
 ** @author Angelo Prudentino
 ** @date 2016-12-08
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

#ifndef QNNETWORKSCENE_H
#define QNNETWORKSCENE_H

#include "qnglobalexport.h"
#include "qndrawtypes.h"
#include "qnstyletypes.h"
#include "qntypes.h"
#include "qnerror.h"

/**
 * @brief The QNNetworkScene class models the scene in which the network will be rendered
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNNetworkScene: public QGraphicsScene{
    Q_OBJECT

private:
    Q_DISABLE_COPY(QNNetworkScene)

    /**
     * @brief The Mode enum enumerates possible operative modes
     */
    enum Mode{
        Navigation ///drag-drop items in the scene
        ,InsertEdge ///insert new edges
    };

    /**
     * @brief current operative mode
     */
    Mode mode;

    /**
     * @brief temporary edge
     */
    QGraphicsLineItemPtr tempLine;

    /**
     * @brief take track of the last mouse-click position
     */
    QPointF mousePos;

    /**
     * @brief the contextual menu invoked when no item is selected
     */
    QMenuPtr sceneCtxMenu;

    /**
     * @brief the contextual menu invoked when only one node item is selected
     */
    QMenuPtr oneNodeSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when many node items are selected
     */
    QMenuPtr manyNodesSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when only one model item is selected
     */
    QMenuPtr oneModelSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when many model items are selected
     */
    QMenuPtr manyModelsSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when only one textbox item is selected
     */
    QMenuPtr oneTextboxSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when many textbox items are selected
     */
    QMenuPtr manyTextboxesSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when only one edge item is selected
     */
    QMenuPtr oneEdgeSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when many edge items are selected
     */
    QMenuPtr manyEdgesSelectedCtxMenu;

    /**
     * @brief the contextual menu invoked when many items of different kinds are selected
     */
    QMenuPtr manySelectedCtxMenu;

    /**
     * @brief the corresponding network
     */
    QNNetworkModelPtr network;

    /**
     * @brief the list of contained items
     */
    ScopedGraphicsItemList itemList;

    /**
     * @brief style manager reference
     */
    QNStyleManagerPtr styleMgr;

    /**
     * @brief determines if the given point in inside a model and returns it
     * @param aPoint the point to be tested
     * @param aItem the (optional) item to be filtered
     * @return the parent model item
     */
    QNModelItemPtr determineParent(QPointF aPoint, QGraphicsItemPtr aItem = 0);

    /**
     * @brief adds a graphic item to the list
     * @param aItem the item to be added
     */
    void addItemToList(QGraphicsItemPtr aItem);

    /**
     * @brief retrieves the node item associated to the given id
     * @param aId the node id to be seedgeed
     * @return the corresponding node item
     * @note if no item is found, the returned pointer will be null
     */
    QNNodeItemPtr getNode(QString aId);

    /**
     * @brief adds a node to scene and renders it
     * @param aNode the node to be added
     * @param aParent the parent item for the added node
     */
    void addNode(QNNodeIconPtr aNode, QGraphicsItemPtr aParent = 0);

    /**
     * @brief removes the given node from the scene
     * @param aNodeItem the node to be removed
     */
    void removeNode(QNNodeItemPtr aNodeItem);

    /**
     * @brief adds an edge to scene and renders it
     * @param aEdge the edge to be added
     */
    void addEdge(QNEdgeStylePtr aEdge);

    /**
     * @brief removes the given edge from the scene
     * @param aEdgeItem the edge to be removed
     */
    void removeEdge(QNEdgeItemPtr aEdgeItem);

    /**
     * @brief adds an model to scene and renders it
     * @param aModel the model to be added
     * @param aParent the parent item for the added model
     * @note null value for aModel indicates the whole network
     */
    void addModel(QNModelPtr aModel = 0, QGraphicsItemPtr aParent = 0);

    /**
     * @brief removes the given model from the scene
     * @param aModelItem the model to be removed
     */
    void removeModel(QNModelItemPtr aModelItem);

    /**
     * @brief adds an textbox to scene and renders it
     * @param aTextbox the textbox to be added
     * @param aParent the parent item for the added textbox
     */
    void addTextbox(QNTextBoxPtr aTextbox, QGraphicsItemPtr aParent = 0);

    /**
     * @brief removes the given textbox from the scene
     * @param aTextbox the textbox to be removed
     */
    void removeTextbox(QNTextBoxItemPtr aTextbox);

protected:
    /**
     * @brief mousePressEvent custom implementation
     * @param aEvent the delivered event
     */
    void mousePressEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseMoveEvent custom implementation
     * @param aEvent the delivered event
     */
    void mouseMoveEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief mouseReleaseEvent custom implementation
     * @param aEvent the delivered event
     * @note this method emits #itemsSelected() signal
     */
    void mouseReleaseEvent(QGraphicsSceneMouseEventPtr aEvent) Q_DECL_OVERRIDE;

    /**
     * @brief contextMenuEvent custom implementation
     * @param aEvent the delivered event
     */
    void contextMenuEvent(QGraphicsSceneContextMenuEventPtr aEvent) Q_DECL_OVERRIDE;

public slots:
    /**
     * @brief deletes all selected items in the scene
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void deleteItems();

    /**
     * @brief brings to front all selected items in the scene
     */
    void bringToFront();

    /**
     * @brief sends to back all selected items in the scene
     */
    void sendToBack();

    /**
     * @brief insert a new empty node into the scene
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void newNode();

    /**
     * @brief insert a new empty model into the scene
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void newModel();

    /**
     * @brief insert a new empty textbox into the scene
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void newTextbox();

    /**
     * @brief sets the new edge mode for this scene
     */
    void newEdgeMode();

    /**
     * @brief builds a cycle among all the selected nodes;
     * @note if other items are selected, those are ignored
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void newCycle();

    /**
     * @brief selects all items in the scene
     * @note this method emits #itemsSelected() signal
     */
    void selectAll();

    /**
     * @brief toggles the showContent properties of all selected items
     * @note selected items that does not support this functionality, will be ignored
     */
    void toggleShowContent();

    /**
     * @brief hides the content of all the selected items
     * @note selected items that does not support this functionality, will be ignored
     */
    void hideAllContents();

    /**
     * @brief hides the content of all the selected items
     * @note selected items that does not support this functionality, will be ignored
     */
    void showAllContents();

public:
    /**
     * @brief QNNetworkScene constructor
     * @param aStyleMgr the reference to the style manager
     * @param aParent the parent object
     */
    QNNetworkScene(QNStyleManagerPtr aStyleMgr = 0, QObjectPtr aParent = 0);

    /**
     * @brief QNNetworkScene destructor
     */
    ~QNNetworkScene();

    /**
     * @brief creates a new edge between the given two nodes
     * @param aSource the source node
     * @param aDest the destination node
     * @note aSource and aDest must be valid and different nodes
     * to get the edge inserted
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void newEdge(QNNodeItemPtr aSource, QNNodeItemPtr aDest);

    /**
     * @brief determines if an item can be moved inside/outside a model
     * and does it
     * @note this method emits #modified() and #itemsSelected() signals
     */
    void manageMoveInside();

    /**
     * @brief determines which items are currently selected and send the
     * proper signals accordingly
     * @note this method emits #itemsSelected() signal
     */
    void manageSelection();

    /**
     * @brief decides which ctxMenu should be invoked and invokes it
     * @param aEvent the event to be processed
     * @return true if the event has been managed, false otherwhise
     */
    bool manageCtxMenuClick(QGraphicsSceneContextMenuEventPtr aEvent);

    /**
     * @brief updates the default scene's contextual menu
     * @param aValue the new contextual menu
     */
    void updateSceneCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * only one node is selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateOneNodeSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * many nodes are selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateManyNodesSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * only one model is selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateOneModelSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * many models are selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateManyModelsSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * only one textbox is selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateOneTextboxSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * many textboxes are selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateManyTextboxesSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * only one edge is selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateOneEdgeSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * many edges are selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateManyEdgesSelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the default contextual menu to be invoked when
     * many different items are selected
     * @param aValue the new contextual menu
     * @note if aValue is null, the default menu will be used
     */
    void updateManySelectedCtxMenu(QMenuPtr aValue);

    /**
     * @brief updates the network reference
     * @param aValue the new network reference
     * @warning passing null as network reference will disable the all scene
     */
    void setNetwork(QNNetworkModelPtr aValue);

    /**
     * @brief retrieves the last mouse click that caused the appearence of
     * a contextual menu in the scene
     * @return the last mouse cursor position
     */
    QPointF getMousePos() const;

    /**
     * @brief set a mouse position to be used as last mouse clicked position
     * @param aValue the position to be set
     * @warning use this method only for test purposes
     */
    void setMousePos(const QPointF& aValue);

signals:
    /**
     * @brief signals that the network has been modified
     */
    void modified();

    /**
     * @brief signals every chenge reguarding the active or selected items
     * @param aNodeList the list of selected nodes
     * @param aModelList the list of selected models
     * @param aTextboxList the list of selected textboxes
     * @param aEdgeList the list of selected edges
     */
    void itemsSelected(QNNodeItemList aNodeList, QNModelItemList aModelList,
                      QNTextBoxItemList aTextboxList, QNEdgeItemList aEdgeList);
};

#endif // QNNETWORKSCENE_H
