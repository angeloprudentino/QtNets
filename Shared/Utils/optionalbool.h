/**
 ** @file optionalbool.h
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

#ifndef OPTIONALBOOL_H
#define OPTIONALBOOL_H

#include "qnglobalexport.h"

/**
 * @brief The OptionalBool class represents an optional boolean value
 *
 * This object could be true, false or invalid (meaning no value has been assigned yet)
 */
class QTNETS_EXPORT OptionalBool{
private:
    /**
     * @brief stores object validity
     */
    bool valid = false;

    /**
     * @brief stores object value
     */
    bool value = false;

public:
    /**
     * @brief OptionalBool default constructor
     *
     * The object is now invalid
     */
    OptionalBool();

    /**
     * @brief OptionalBool value setting constructor
     * @param aValue the value assigned to the object
     *
     * The object is now valid
     */
    OptionalBool(const bool aValue);

    /**
     * @brief OptionalBool copy constructor
     * @param aNewValue the object to copy from
     */
    OptionalBool(const OptionalBool& aNewValue);

    /**
     * @brief OptionalBool assignemt operator
     * @param aNewValue the object to copy from
     * @return a newly created OptionalBool based on the object passed as parameter
     */
    OptionalBool& operator=(const OptionalBool& aNewValue);

    /**
     * @brief returns the validity of this instance
     * @return true if a value has been assigned
     */
    bool isValid() const;

    /**
     * @brief returns the instance's value
     * @return the object's value
     */
    operator bool() const;

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const OptionalBool& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const OptionalBool& aOther) const;
};

#endif // OPTIONALBOOL_H
