/**
 ** @file smilealgparams.h
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

#ifndef SMILEALGPARAMS_H
#define SMILEALGPARAMS_H

#include <QMetaType>
#include "smiletypes.h"
#include "printable.h"

/**
 * @brief The SmileEpis class models the smile epis element
 *
 * @see SmileXmlParser#parseSmileAlgparams() for the formal description
 */
class SmileEpis: public Printable{
private:
    /**
     * @brief proplen
     */
    int proplen = 0;
    /**
     * @brief numStates1
     */
    int numStates1 = 0;
    /**
     * @brief numStates2
     */
    int numStates2 = 0;
    /**
     * @brief numStates3
     */
    int numStates3 = 0;
    /**
     * @brief eps1
     */
    double eps1 = 0;
    /**
     * @brief eps2
     */
    double eps2 = 0;
    /**
     * @brief eps3
     */
    double eps3 = 0;
    /**
     * @brief eps4
     */
    double eps4 = 0;

public:
    /**
     * @brief SmileEpis constructor
     */
    SmileEpis();

    /**
     * @brief retrieves the proplen value
     * @return the proplen value
     */
    int getProplen() const;

    /**
     * @brief retrieves the numState1 value
     * @return the numState1 value
     */
    int getNumStates1() const;

    /**
     * @brief retrieves the numState2 value
     * @return the numState2 value
     */
    int getNumStates2() const;

    /**
     * @brief retrieves the numState3 value
     * @return the numState3 value
     */
    int getNumStates3() const;

    /**
     * @brief retrieves the eps1 value
     * @return the eps1 value
     */
    double getEps1() const;

    /**
     * @brief retrieves the eps2 value
     * @return the eps2 value
     */
    double getEps2() const;

    /**
     * @brief retrieves the eps3 value
     * @return the eps3 value
     */
    double getEps3() const;

    /**
     * @brief retrieves the eps4 value
     * @return the eps4 value
     */
    double getEps4() const;

    /**
     * @brief updates proplenS property
     * @param value the value to set as proplen
     */
    void setProplen(int value);

    /**
     * @brief updates numStates1 property
     * @param value the value to set as numStates1
     */
    void setNumStates1(int value);

    /**
     * @brief updates numStates2 property
     * @param value the value to set as numStates2
     */
    void setNumStates2(int value);

    /**
     * @brief updates numStates3 property
     * @param value the value to set as numStates3
     */
    void setNumStates3(int value);

    /**
     * @brief updates eps1 property
     * @param value the value to set as eps1
     */
    void setEps1(double value);

    /**
     * @brief updates eps2 property
     * @param value the value to set as eps2
     */
    void setEps2(double value);

    /**
     * @brief updates eps3 property
     * @param value the value to set as eps3
     */
    void setEps3(double value);

    /**
     * @brief updates eps4 property
     * @param value the value to set as eps4
     */
    void setEps4(double value);

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
    bool operator==(const SmileEpis& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const SmileEpis& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const SmileEpis& aOther) const;
};
Q_DECLARE_METATYPE(SmileEpis)


/**
 * @brief The SmileAlgparams class models the smile algparams element
 *
 * @see SmileXmlParser#parseSmileAlgparams() for the formal description
 */
class SmileAlgparams: public Printable {
private:
    /**
     * @brief the epis child element
     */
    ScopedSmileEpis epis;

public:
    /**
     * @brief SmileAlgparams default constructor
     */
    SmileAlgparams();

    /**
     * @brief SmileAlgparams destructor
     */
    ~SmileAlgparams();

    /**
     * @brief SmileAlgparams copy constructor
     * @param aOther the instance to copy from
     */
    SmileAlgparams(const SmileAlgparams& aOther);

    /**
     * @brief SmileAlgparams assignment operator
     * @param aOther the instance to copy from
     * @return a new object that is a copy of the given parameter
     */
    const SmileAlgparams& operator=(const SmileAlgparams& aOther);

    /**
     * @brief returns the epis child element
     * @return the epis child element (live reference)
     * @note if the epis does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    SmileEpisPtr getEpis();

    /**
     * @brief stores the epis sub-element on this algparams
     * @param value the value to set as epis
     */
    void setEpis(ScopedSmileEpis& value);

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
    bool operator==(const SmileAlgparams& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const SmileAlgparams& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const SmileAlgparams& aOther) const;
};
Q_DECLARE_METATYPE(SmileAlgparams)

#endif // SMILEALGPARAMS_H
