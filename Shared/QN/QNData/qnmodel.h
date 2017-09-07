/**
 ** @file qnmodel.h
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

#ifndef QNMODEL_H
#define QNMODEL_H

#include "qntypes.h"
#include "qnicon.h"
#include "qntextbox.h"
#include "qnglobalexport.h"
#include "utility.h"
#include "qnerror.h"

/**
 * @brief The QNModel class models a sub-model (containing nodes and texboxes)
 * in the global plot
 * @note this class is not copyable
 */
class QTNETS_EXPORT QNModel : public QNIcon{
private:
    Q_DISABLE_COPY(QNModel)

    /**
     * @brief the model's winodow position
     */
    ScopedQNPosition window;

    /**
     * @brief the map of icons contained in this model
     */
    ScopedQNIconMap icons;

    /**
     * @brief the list of textboxes belonging to this model
     */
    ScopedQNTextBoxList textboxes;

    /**
     * @brief retrieves the top-level container model
     * @return the top-level container model
     * @note if this model has no container, it means it is the top-level container
     */
    QNModelPtr getTopContainer();

    /**
     * @brief compare this model's icons with the given ones
     * @param aOther the object to be compared with
     * @return true if icons are the same, false otherwise
     */
    bool compareIcons(const QNModel& aOther) const;

    /**
     * @brief compare this model's textboxes with the given ones
     * @param aOther the object to be compared with
     * @return true if textboxes are the same, false otherwise
     */
    bool compareTextboxes(const QNModel& aOther) const;

public:
    /**
     * @brief QNModel default constructor
     * @param aContainer teh reference to the container model
     * @note the main model has null container
     */
    QNModel(QNModelPtr aContainer);

    /**
      * @brief QNModel destructor
      */
    virtual ~QNModel();

    /**
     * @brief operator== implementation
     * @param aOther the object to be compared with
     * @return true if this is equal to aOther, false otherwise
     */
    bool operator==(const QNModel& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const QNModel& aOther) const;

    /**
     * @brief updates the model's id only if it is unique
     * @param aValue the new value for this icon's id
     * @return the error object resulting from this operation
     * @note if aValue is empty, the request will be ignored
     */
    QNError setId(const QString& aValue);

    /**
     * @brief initialize the model's id with a random unique value
     * @param aLen the lenght of the random string to be generated (default 20)
     * @return the error object resulting from this operation
     */
    QNError initId(const uint aLen = DEFAULT_ID_LEN);

    /**
     * @brief retrieves the models's window position in the plot
     * @return the models's window position in the plot (live reference)
     * @note if the model's window position does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNPositionPtr getWindow() const;

    /**
     * @brief returns the list of all icons' keys
     * @return the list of all icons' keys
     */
    QStringList getAllIconsKeys() const;

    /**
     * @brief retrieves the icon given its unique id
     * @param aIconId the id of the desired node
     * @return the icon reference
     * @note if the icon does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNIconPtr getIcon(const QString& aIconId) const;

    /**
     * @brief retrieves an icon given its id searching also in the submodels
     * @param aId the id to search for
     * @return the desired icon or nullptr if not found
     */
    QNIconPtr getIconG(const QString& aId) const;

    /**
     * @brief adds an icon to the icons map
     * @param aIcon the icon to be added
     * @note if aIcon is null, the request will be ignored
     */
    void addIcon(QNIconPtr aIcon);

    /**
     * @brief removes the given icon from the map
     * @param aIcon the icon to be removed
     * @return true if the icon has been removed and deleted
     * @note if aIcon is null, the request will be ignored
     */
    bool removeIcon(QNIconPtr aIcon);

    /**
     * @brief moves an icon from this model to the given one
     * @param aIcon the icon to be moved
     * @param aTo the model where the icon will be moved
     * @return true if the icon has been moved
     * @note if aIcon or aTo is null, the request will be ignored
     */
    bool moveIcon(QNIconPtr aIcon, QNModelPtr aTo);

    /**
     * @brief retrieves a live reference to the list of contained textboxes
     * @return the live reference to the list of contained textboxes
     * @note if the model conatins no textboxes, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    QNTextBoxListPtr getTextboxes() const;

    /**
     * @brief updates the models's window position in the plot
     * @param value the new models's window position in the plot
     * @note if value is null, the request will be ignored
     */
    void setWindow(QNPositionPtr value);

    /**
     * @brief adds a textbox to the list
     * @param aTextBox the textbox to be added
     * @note if aTextBox is null, the request will be ignored
     */
    void addTextBox(QNTextBoxPtr aTextBox);

    /**
     * @brief removes the given textbox from the list
     * @param aTextBox the textbox to be removed
     * @return true if the textbox has been removed and deleted
     * @note if aTextBox is null, the request will be ignored
     */
    bool removeTextBox(QNTextBoxPtr aTextBox);

    /**
     * @brief moves a textbox from this model to the given one
     * @param aTextBox the textbox to be moved
     * @param aTo the model where the textbox will be moved
     * @return true if the textbox has been moved
     * @note if aTextBox or aTo is null, the request will be ignored
     */
    bool moveTextBox(QNTextBoxPtr aTextBox, QNModelPtr aTo);
};

#endif // QNMODEL_H
