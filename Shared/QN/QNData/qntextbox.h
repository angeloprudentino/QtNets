/**
 ** @file qntextbox.h
 ** @author Angelo Prudentino
 ** @date 2016-11-17
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

#ifndef QNTEXTBOX_H
#define QNTEXTBOX_H

#include "qntypes.h"
#include "qnposition.h"
#include "qnfont.h"
#include "qncommonicon.h"
#include "qnstylable.h"
#include "qnglobalexport.h"

/**
 * @brief The QNTextBox class models a textbox in the plot or in one of its submodels
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNTextBox: public QNCommonIcon, public QNStylable {
private:
    Q_DISABLE_COPY(QNTextBox)

    /**
     * @brief reference to the container model
     */
    QNModelPtr container;

    /**
     * @brief textbox's caption
     */
    QString caption;

public:
    /**
     * @brief QNTextBox constructor
     * @param aContainer the reference to the container model
     * @warning passing null pointer as parameter to this constructor could
     * cause a segmentation fault later on in the program execution
     */
    QNTextBox(QNModelPtr aContainer);

    /**
      * @brief QNTextBox destructor
      */
    ~QNTextBox();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNTextBox& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNTextBox& aOther) const;

    /**
     * @brief retrieves the reference to the container model
     * @return the reference to the container model
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNModelPtr getContainer() const;

    /**
     * @brief retrieves the textbox's caption
     * @return the textbox's caption
     */
    QString getCaption() const;

    /**
     * @brief updates the textbox's container reference
     * @param aValue the new textbox's container
     * @note if aValue is null, the request will be ignored
     */
    void setContainer(QNModelPtr aValue);

    /**
     * @brief updates the textbox's caption
     * @param aValue the new the textbox's caption
     */
    void setCaption(const QString& aValue);

    /**
     * @brief returns the list of all properties' keys
     * @return the list of all properties' keys
     */
    QStringList getAllKeys() const Q_DECL_OVERRIDE;

    /**
     * @brief retrieves properties by name
     * @param aKey the name of the required property
     * @return the required property
     * @note if the property is not found, the returned variant object is invalid
     * @see QVariant#isValid()
     */
    QVariant getProperty(const QString& aKey) const Q_DECL_OVERRIDE;

    /**
     * @brief stores the provided property using the provided key
     * @param aKey the property's key
     * @param aValue the property's value
     * @note if aKey is empty, the request will be ignored
     * @result the error object resulting from this operation
     */
    QNError setProperty(const QString& aKey, const QVariant& aValue) Q_DECL_OVERRIDE;
};

#endif // QNTEXTBOX_H
