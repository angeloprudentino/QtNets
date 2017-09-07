/**
 ** @file qntypes.h
 ** @author Angelo Prudentino
 ** @date 2016-11-09
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

#ifndef QNTYPES_H
#define QNTYPES_H

#include <QString>
#include <QScopedPointer>
#include <QPair>
#include <QMap>
#include <QList>
#include <QVariant>
#include <QMetaType>
#include <QtGlobal>


#define DEFAULT_RETRY 100

/**
 * @brief The QNFilterStrategy enum enumerates all possible filtering strategies
 * during seedge/filter operations
 */
enum QNFilterStrategy{
    AND ///all conditions must be satisfied
    ,OR ///at least one condition must be satisfied
};


class QNStylable;
class QNCommonIcon;
class QNNetwork;
class QNNode;
class QNPosition;
class QNEdge;
class QNFont;
class QNIcon;
class QNNodeIcon;
class QNModel;
class QNTextBox;
class QNNetworkModel;
class QNEdgeStyle;

/**
 * @brief The DoubleList class models a list of doubles
 */
class DoubleList: public QList<double>{
public:
    /**
     * @brief DoubleList constructor
     */
    DoubleList(){
        if(!QMetaType::hasRegisteredComparators<DoubleList>())
            QMetaType::registerComparators<DoubleList>();
    }

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const DoubleList& aOther) const {
        if(this->size() != aOther.size())
            return false;

        int size = this->size();
        for(int i=0; i<size; i++){
            double mine = this->at(i);
            double other = aOther.at(i);
            double dif = qAbs(mine - other);
            if(!(dif <= 0.1e-9 || dif > 0))
                return false;
        }

        return true;
    }
};
Q_DECLARE_METATYPE(DoubleList)

/**
 * @brief The IntList class models a list of integers
 */
class IntList: public QList<int>{
public:
    /**
     * @brief IntList constructor
     */
    IntList(){
        if(!QMetaType::hasRegisteredComparators<IntList>())
            QMetaType::registerComparators<IntList>();
    }

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const IntList& aOther) const {
        if(this->size() != aOther.size())
            return false;

        int size = this->size();
        for(int i=0; i<size; i++){
            int tmp = this->at(i);
            if(!aOther.contains(tmp))
                return false;
        }

        return true;
    }
};
Q_DECLARE_METATYPE(IntList)


/**
 * @brief Represent a pointer to a generic object
 */
typedef QObject* QObjectPtr;

/**
 * @brief Represents a pointer to a list of strings
 * @see QStringList
 */
typedef QStringList* QStringListPtr;

/**
 * @brief Represents a unique pointer to a list of strings
 * @see QStringList
 */
typedef QScopedPointer<QStringList> ScopedStringList;

/**
 * @brief Represents a property of a generic kind
 * @see QVariant
 */
typedef QPair<QString, QVariant> QNProperty;

/**
 * @brief Represents a list of QN properties
 * @see QNProperty
 */
typedef QList<QNProperty> QNPropertyList;

/**
 * @brief Represents a map with strings as keys and QVariants as values
 */
typedef QMap<QString, QVariant> QNVariantMap;

/**
 * @brief Represents a unique pointer to a map with strings as keys and QVariants as values
 * @see QNVariantMap
 */
typedef QScopedPointer<QNVariantMap> ScopedQNVariantMap;

/**
 * @brief Represents a pointer to an icon or a textbox object
 */
typedef QNCommonIcon* QNCommonIconPtr;

/**
 * @brief Represents a pointer to a stylable object
 * @see QNStylable
 */
typedef QNStylable* QNStylablePtr;

/**
 * @brief Represents a list of stylable pointers
 * @see QNStylablePtr
 */
typedef QList<QNStylablePtr> QNStylableList;
/**

 * @brief Represents a pointer to network object instance
 * @see QNNetwork
 */
typedef QNNetwork* QNNetworkPtr;

/**
 * @brief Represents a unique pointer to a QNNetwork instance
 * @see QNNetwork
 */
typedef QScopedPointer<QNNetwork> ScopedQNNetwork;

/**
 * @brief Represents a pointer to a node object instance
 * @see QNNode
 */
typedef QNNode* QNNodePtr;

/**
 * @brief Represents a list of nodes pointers
 * @see QNNodePtr
 */
typedef QList<QNNodePtr> QNNodeList;

/**
 * @brief Represents a pointer to a list of nodes pointers
 * @see QNNodeList
 */
typedef QNNodeList* QNNodeListPtr;

/**
 * @brief Represents a unique pointer to a list of nodes pointer
 * @see QNNodeList
 */
typedef QScopedPointer<QNNodeList> ScopedQNNodeList;

/**
 * @brief Represents a map of nodes pointers with strings as keys
 * @see QNNodePtr
 */
typedef QMap<QString, QNNodePtr> QNNodeMap;

/**
 * @brief Represents a unique pointer to a map of nodes pointers with strings as keys
 * @see QNNodeMap
 */
typedef QScopedPointer<QNNodeMap> ScopedQNNodeMap;

/**
 * @brief Represents a pointer to an edge object instance
 * @see QNEdge
 */
typedef QNEdge* QNEdgePtr;

/**
 * @brief Represents a list of edges pointers
 * @see QNEdgePtr
 */
typedef QList<QNEdgePtr> QNEdgeList;

/**
 * @brief Represents a pointer to a list of edges pointers
 * @see QNEdgeList
 */
typedef QNEdgeList* QNEdgeListPtr;

/**
 * @brief Represents a unique pointer to a list of edges pointer
 * @see QNEdgeList
 */
typedef QScopedPointer<QNEdgeList> ScopedQNEdgeList;

/**
 * @brief Represents a map of edges pointers with strings as keys
 * @see QNEdgePtr
 */
typedef QMap<QString, QNEdgePtr> QNEdgeMap;

/**
 * @brief Represents a unique pointer to a map of edges with strings as keys
 * @see QNEdgeMap
 */
typedef QScopedPointer<QNEdgeMap> ScopedQNEdgeMap;

/**
 * @brief Represents a pointer to an icon position instance
 * @see QNPosition
 */
typedef QNPosition* QNPositionPtr;

/**
 * @brief Represents a unique pointer to an icon position instance
 * @see QNPosition
 */
typedef QScopedPointer<QNPosition> ScopedQNPosition;

/**
 * @brief Represents a pointer to an icon font instance
 * @see QNFont
 */
typedef QNFont* QNFontPtr;

/**
 * @brief Represents a unique pointer to a icon font instance
 * @see QNFont
 */
typedef QScopedPointer<QNFont> ScopedQNFont;

/**
 * @brief Represents a pointer to an icon object instance
 * @see QNIcon
 */
typedef QNIcon* QNIconPtr;

/**
 * @brief Represents a list of icon objects
 * @see QNIconPtr
 */
typedef QList<QNIconPtr> QNIconList;

/**
 * @brief Represents a pointer to a list of icon objects
 * @see QNIconList
 */
typedef QNIconList* QNIconListPtr;

/**
 * @brief Represents a unique pointer to a list of icon objects
 * @see QNIconList
 */
typedef QScopedPointer<QNIconList> ScopedQNIconList;

/**
 * @brief Represents a map of icon objects with strings as keys
 * @see QString
 * @see QNIconPtr
 */
typedef QMap<QString, QNIconPtr> QNIconMap;

/**
 * @brief Represents a unique pointer to a map of icon objects with strings as keys
 * @see QNIconMap
 */
typedef QScopedPointer<QNIconMap> ScopedQNIconMap;

/**
 * @brief Represents a pointer to an node icon object instance
 * @see QNNodeIcon
 */
typedef QNNodeIcon* QNNodeIconPtr;

/**
 * @brief Represents a pointer to an model object instance
 * @see QNModel
 */
typedef QNModel* QNModelPtr;

/**
 * @brief Represents a pointer to a network model instance
 * @see QNNetworkModel
 */
typedef QNNetworkModel* QNNetworkModelPtr;

/**
 * @brief Represents a unique pointer to a network model instance
 * @see QNNetworkModel
 */
typedef QScopedPointer<QNNetworkModel> ScopedQNNetworkModel;

/**
 * @brief Represents a pointer to a textbox object instance
 * @see QNTextBox
 */
typedef QNTextBox* QNTextBoxPtr;

/**
 * @brief Represents a list of textbox objects
 * @see QNTextBoxPtr
 */
typedef QList<QNTextBoxPtr> QNTextBoxList;

/**
 * @brief Represents a pointer to a list of textbox objects
 * @see QNTextBoxList
 */
typedef QNTextBoxList* QNTextBoxListPtr;

/**
 * @brief Represents a unique pointer to a list of textbox objects
 * @see QNTextBoxList
 */
typedef QScopedPointer<QNTextBoxList> ScopedQNTextBoxList;

/**
 * @brief Represents a pointer to an edge-style object instance
 * @see QNEdgeStyle
 */
typedef QNEdgeStyle* QNEdgeStylePtr;

/**
 * @brief Represents a list of edge-style objects
 * @see QNEdgeStylePtr
 */
typedef QList<QNEdgeStylePtr> QNEdgeStyleList;

/**
 * @brief Represents a pointer to a list of edge-style objects
 * @see QNEdgeStyleList
 */
typedef QNEdgeStyleList* QNEdgeStyleListPtr;

/**
 * @brief Represents a unique pointer to a list of edge-style objects
 * @see QNEdgeStyleList
 */
typedef QScopedPointer<QNEdgeStyleList> ScopedQNEdgeStyleList;

/**
 * @brief Represents a map of edge style pointers with strings as keys
 * @see QNEdgeStylePtr
 */
typedef QMap<QString, QNEdgeStylePtr> QNEdgeStyleMap;

/**
 * @brief Represents a unique pointer to a map of
 * edge style pointers with strings as keys
 * @see QNEdgeStyleMap
 */
typedef QScopedPointer<QNEdgeStyleMap> ScopedQNEdgeStyleMap;

#endif // QNTYPES_H
