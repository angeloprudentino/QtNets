/**
 ** @file smilenode.h
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

#ifndef SMILENODE_H
#define SMILENODE_H

#include <QMetaType>
#include <QString>
#include <QList>
#include "optionalbool.h"
#include "printable.h"

#define MIN_STATE_NUM 2

/**
 * @brief The DiagType enum
 */
enum DiagType{
    target,
    observation,
    auxiliary
};
Q_DECLARE_METATYPE(DiagType)


/**
 * @brief The SmileNodeState class models the smile node's state element
 *
 * @see SmileXmlParser#parseNodeStates() for the formal description
 */
class SmileNodeState: public Printable{
private:
    /**
     * @brief id for this state
     */
    QString id = "";

    /**
     * @brief label for this state
     */
    QString label = "";

    /**
     * @brief target for this state
     * @note this property is optional
     */
    OptionalBool target;

    /**
     * @brief default for this state
     * @note this property is optional
     */
    OptionalBool _default;

    /**
     * @brief fault for this state
     * @note this property is optional
     */
    OptionalBool fault;

public:
    /**
     * @brief SmileNodeState default constructor
     */
    SmileNodeState();

    /**
     * @brief retrievs the id of this state
     * @return the id of this state
     */
    QString getId() const;

    /**
     * @brief retrievs the label of this state
     * @return the label of this state
     */
    QString getLabel() const;

    /**
     * @brief retrievs the target of this state
     * @return the target of this state
     */
    OptionalBool getTarget() const;

    /**
     * @brief retrievs the default of this state
     * @return the default of this state
     */
    OptionalBool getDefault() const;

    /**
     * @brief retrievs the fault of this state
     * @return the fault of this state
     */
    OptionalBool getFault() const;

    /**
     * @brief updates the id for this state
     * @param aValue the value to set as id
     */
    void setId(const QString& aValue);

    /**
     * @brief updates the label for this state
     * @param aValue the value to set as label
     */
    void setLabel(const QString& aValue);

    /**
     * @brief updates the target for this state
     * @param aValue the value to set as target
     */
    void setTarget(const bool aValue);

    /**
     * @brief updates the default for this state
     * @param aValue the value to set as default
     */
    void setDefault(const bool aValue);

    /**
     * @brief updates the fault for this state
     * @param aValue the value to set as fault
     */
    void setFault(const bool aValue);

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
    bool operator==(const SmileNodeState& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const SmileNodeState& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const SmileNodeState& aOther) const;
};
Q_DECLARE_METATYPE(SmileNodeState)

/**
 * @brief The SmileNodeStateList class models a list of node's states objects
 * @see SmileNodeState
 */
class SmileNodeStateList: public QList<SmileNodeState>, public Printable{
public:
    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    QString toString() override;
};
Q_DECLARE_METATYPE(SmileNodeStateList)

#endif // SMILENODE_H
