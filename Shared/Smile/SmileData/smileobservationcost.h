/**
 ** @file smileobservationcost.h
 ** @author Angelo Prudentino
 ** @date 2016-10-28
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

#ifndef SMILEOBSERVATIONCOST_H
#define SMILEOBSERVATIONCOST_H

#include <QMetaType>
#include "smiletypes.h"
#include "printable.h"
#include "qntypes.h"

/**
 * @brief The SmileObscostNode class models the smile observationcost's node element
 *
 * @see SmileXmlParser#parseSmileObscost() for the formal description
 */
class SmileObscostNode: public Printable {
private:
    /**
     * @brief node's id
     */
    QString id = "";

    /**
     * @brief node's parents list
     */
    QStringList parents;

    /**
     * @brief node's costs list
     */
    DoubleList costs;

public:
    /**
     * @brief SmileObscostNode constructor
     * @param aId the node's id
     */
    SmileObscostNode(const QString& aId = "");

    /**
     * @brief returns node's id
     * @return node's id
     */
    QString getId() const;

    /**
     * @brief returns the list of parents
     * @return the list of parents
     */
    QStringList getParents() const;

    /**
     * @brief add a parent to the list
     * @param aParent the parent to be added
     */
    void addParent(const QString& aParent);

    /**
     * @brief returns the list of costs
     * @return the list of costs
     */
    DoubleList getCosts() const;

    /**
     * @brief set a list of costs
     * @param aCost the cost to be added
     */
    void addCost(const double aCost);

    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    QString toString() override;

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const SmileObscostNode& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const SmileObscostNode& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const SmileObscostNode& aOther) const;
};
Q_DECLARE_METATYPE(SmileObscostNode)


/**
 * @brief The SmileObservationcost class models the smile observationcost element
 *
 * @see SmileXmlParser#parseSmileObscost() for the formal description
 */
class SmileObservationcost: public Printable{
private:
    /**
     * @brief the list of observationcost's nodes
     */
    ScopedSmileObscostNodeList nodes;

public:
    /**
     * @brief SmileObservationcost default constructor
     */
    SmileObservationcost();

    /**
     * @brief SmileObservationcost destructor
     */
    ~SmileObservationcost();

    /**
     * @brief SmileObservationcost copy constructor
     * @param aOther the instance to copy from
     */
    SmileObservationcost(const SmileObservationcost& aOther);

    /**
     * @brief SmileObservationcost assignment operator
     * @param aOther the instance to copy from
     * @return a new object that is a copy of the given parameter
     */
    const SmileObservationcost& operator=(const SmileObservationcost& aOther);

    /**
     * @brief adds a node to the list
     * @param aNode the node to be added
     */
    void addNode(SmileObscostNode& aNode);

    /**
     * @brief returns a live list of nodes
     * @return a live list of nodes;
     * @note if there are no nodes, null will be returned
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    SmileObscostNodeListPtr getNodes() const;

    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    QString toString() override;

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const SmileObservationcost& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const SmileObservationcost& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const SmileObservationcost& aOther) const;
};
Q_DECLARE_METATYPE(SmileObservationcost)

#endif // SMILEOBSERVATIONCOST_H
