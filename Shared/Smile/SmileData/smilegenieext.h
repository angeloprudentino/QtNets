/**
 ** @file smilegenieext.h
 ** @author Angelo Prudentino
 ** @date 2016-11-15
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

#ifndef SMILEGENIEEXT_H
#define SMILEGENIEEXT_H

#include <QMetaType>
#include "smiletypes.h"
#include "printable.h"

/**
 * @brief The GenieFNF enum enumerates possible values for the faultnameformat attribute
 */
enum GenieFNF{
    node,
    nodestate,
    user,
    inherit
};
Q_DECLARE_METATYPE(GenieFNF)

/**
 * @brief The GenieDAF class models the genie's diagautoformat attribute
 */
class GenieDAF: public Printable {
private:
    /**
     * @brief genie's diagautoformat target
     */
    QString target = "";

    /**
     * @brief genie's diagautoformat targetnr
     */
    QString targetnr = "";

    /**
     * @brief genie's diagautoformat observation
     */
    QString observation = "";

    /**
     * @brief genie's diagautoformat observationnr
     */
    QString observationnr = "";

    /**
     * @brief genie's diagautoformat auxiliary
     */
    QString auxiliary = "";

public:
    /**
     * @brief GenieDAF default constructor
     */
    GenieDAF();

    /**
     * @brief retrieves the genie's diagautoformat target
     * @return the genie's diagautoformat target
     */
    QString getTarget() const;

    /**
     * @brief retrieves the genie's diagautoformat targetnr
     * @return the genie's diagautoformat targetnr
     */
    QString getTargetnr() const;

    /**
     * @brief retrieves the genie's diagautoformat observation
     * @return the genie's diagautoformat observation
     */
    QString getObservation() const;

    /**
     * @brief retrieves the genie's diagautoformat observationnr
     * @return the genie's diagautoformat observationnrn
     */
    QString getObservationnr() const;

    /**
     * @brief retrieves the genie's diagautoformat auxiliary
     * @return the genie's diagautoformat auxiliary
     */
    QString getAuxiliary() const;

    /**
     * @brief updates the genie's diagautoformat target
     * @param value the new genie's diagautoformat target
     */
    void setTarget(const QString& value);

    /**
     * @brief updates the genie's diagautoformat targetnr
     * @param value the new genie's diagautoformat targetnr
     */
    void setTargetnr(const QString& value);

    /**
     * @brief updates the genie's diagautoformat observation
     * @param value the new genie's diagautoformat observation
     */
    void setObservation(const QString& value);

    /**
     * @brief updates the genie's diagautoformat observationnr
     * @param value the new genie's diagautoformat observationnr
     */
    void setObservationnr(const QString& value);

    /**
     * @brief updates the genie's diagautoformat auxiliary
     * @param value the new genie's diagautoformat auxiliary
     */
    void setAuxiliary(const QString& value);

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
    bool operator==(const GenieDAF& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const GenieDAF& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const GenieDAF& aOther) const;
};
Q_DECLARE_METATYPE(GenieDAF)


/**
 * @brief The SmileGenieExt class models genie extensions for smile networks
 */
class SmileGenieExt: public Printable {
private:
    /**
     * @brief genie's name
     */
    QString name = "";

    /**
     * @brief genie's version
     */
    QString version = "";

    /**
     * @brief genie's comment
     */
    QString comment = "";

    /**
     * @brief genie's faultnameformat
     */
    GenieFNF faultnameformat = GenieFNF::user;

    /**
     * @brief genie's diagautoformat
     */
    ScopedGenieDAF diagautoformat;

public:
    /**
     * @brief SmileGenieExt default constructor
     */
    SmileGenieExt();

    /**
     * @brief SmileGenieExt copy constructor
     * @param aOther the SmileGenieExt instance to be copied
     */
    SmileGenieExt(const SmileGenieExt& aOther);

    /**
     * @brief SmileGenieExt assignment operator
     * @param aOther the SmileGenieExt instance to be copied
     * @return a copy of the passed object
     */
    const SmileGenieExt& operator=(const SmileGenieExt& aOther);

    /**
     * @brief SmileGenieExt destructor
     */
    ~SmileGenieExt();

    /**
     * @brief retrieves the genie's name
     * @return the genie's name
     */
    QString getName() const;

    /**
     * @brief retrieves the genie's version
     * @return the genie's version
     */
    QString getVersion() const;

    /**
     * @brief retrieves the genie's comment
     * @return the genie's comment
     */
    QString getComment() const;

    /**
     * @brief retrieves the genie's faultnameformat
     * @return the genie's faultnameformat
     */
    GenieFNF getFaultnameformat() const;

    /**
     * @brief retrieves the genie's diagautoformat
     * @return the genie's diagautoformat (live reference)
     * @note if the diagautoformat does not exist, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    GenieDAFPtr getDiagautoformat() const;

    /**
     * @brief updates the genie's name
     * @param value the new value for the genie's name
     */
    void setName(const QString& value);

    /**
     * @brief updates the genie's version
     * @param value the new value for the genie's version
     */
    void setVersion(const QString& value);

    /**
     * @brief updates the genie's comment
     * @param value the new value for the genie's comment
     */
    void setComment(const QString& value);

    /**
     * @brief updates the genie's faultnameformat
     * @param value the new value for the genie's faultnameformat
     */
    void setFaultnameformat(const GenieFNF& value);

    /**
     * @brief updates the genie's diagautoformat
     * @param value the new value for the genie's diagautoformat
     */
    void setDiagautoformat(GenieDAFPtr value);

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
    bool operator==(const SmileGenieExt& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const SmileGenieExt& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const SmileGenieExt& aOther) const;
};
Q_DECLARE_METATYPE(SmileGenieExt)

#endif // SMILEGENIEEXT_H
