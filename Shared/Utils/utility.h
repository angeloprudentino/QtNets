/**
 ** @file utility.h
 ** @author Angelo Prudentino
 ** @date 2016-10-26
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

#ifndef UTILITY_H
#define UTILITY_H

#include <QString>

#define DEFAULT_ID_LEN 20


/**
 * @brief generates a random string
 * @param aLen the lenght of the random string to be generated (default 20)
 * @param aChars the list of characters that can be used
 * (if empty, all characters and numbers are used)
 * @return the generated random string
 */
const QString genRandString(const uint aLen = DEFAULT_ID_LEN, const QString& aChars = "");

#endif // UTILITY_H
