/**
 ** @file qnerror.h
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

#ifndef QNERROR_H
#define QNERROR_H

#include <QString>
#include "qnglobalexport.h"

#define SUCCESS 0
#define NET_FORMAT_NOT_SUPPORTED -1
#define NET_READ_ERROR -2
#define NET_WRITE_ERROR -3
#define STYLE_FORMAT_NOT_SUPPORTED -4
#define STYLE_READ_ERROR -5
#define STYLE_WRITE_ERROR -6
#define INVALID_STYLE_STATE -7
#define DUPLICATED_ID -8
#define NO_DATA -9
#define INVALID_REQ -10

/**
 * @brief The QNError class models an error occured while parsing or writing
 * a smile xml document
 */
class QTNETS_EXPORT QNError {
private:
    /**
     * @brief error state
     */
    bool err;

    /**
     * @brief error code
     */
    int code;

    /**
     * @brief error detail
     */
    QString detail;

public:
    /**
     * @brief QNError default constructor
     * @note use this constructor to set no error
     */
    QNError();

    /**
     * @brief QNError error constructor
     * @param aCode the code of the error
     * @param aDetail the detail of the error
     * @note use this constructor to set a specific error
     */
    QNError(const int aCode, const QString& aDetail);

    /**
     * @brief allows to use this object easily in conditional constructs
     */
    operator bool() const;

    /**
     * @brief retrieves the error code
     * @return the error code
     */
    int getCode() const;

    /**
     * @brief retrieves the error detail
     * @return the error detail
     */
    QString getDetail() const;
};

#endif // QNERROR_H
