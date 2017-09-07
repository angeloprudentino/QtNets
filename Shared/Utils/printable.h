/**
 ** @file printable.h
 ** @author Angelo Prudentino
 ** @date 2016-12-06
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

#ifndef PRINTABLE_H
#define PRINTABLE_H

/**
 * @brief The Printable class models a generic object that can be printed out
 */
class Printable{
public:
    /**
     * @brief Printable destructor
     */
    virtual ~Printable() {}

    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    virtual QString toString() = 0;
};

#endif // PRINTABLE_H
