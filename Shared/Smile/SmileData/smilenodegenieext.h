/**
 ** @file smilenodegenieext.h
 ** @author Angelo Prudentino
 ** @date 2016-11-16
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

#ifndef GENIETABLECOMMENT_H
#define GENIETABLECOMMENT_H

#include <QMetaType>
#include <QString>
#include "smiletypes.h"
#include "printable.h"

/**
 * @brief The GenieTableComment class models the genie's node tableComment element
 */
class GenieTableComment: public Printable {
private:
    /**
     * @brief tableComment's column
     */
    int col = 0;

    /**
     * @brief tableComment's row
     */
    int row = 0;

public:
    /**
     * @brief GenieTableComment default constructor
     */
    GenieTableComment();

    /**
     * @brief retrieves the tableComment's column
     * @return the tableComment's column
     */
    int getCol() const;

    /**
     * @brief retrieves the tableComment's row
     * @return the tableComment's row
     */
    int getRow() const;

    /**
     * @brief updates the tableComment's column
     * @param value the new tableComment's column value
     */
    void setCol(int value);

    /**
     * @brief updates the tableComment's row
     * @param value the new tableComment's row value
     */
    void setRow(int value);

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
    bool operator==(const GenieTableComment& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const GenieTableComment& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const GenieTableComment& aOther) const;
};
Q_DECLARE_METATYPE(GenieTableComment)

/**
 * @brief The GenieTableCommentList class models a list of genie table comment objects
 * @see GenieTableComment
 */
class GenieTableCommentList: public QList<GenieTableComment>, public Printable{
public:
    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    QString toString() override;
};
Q_DECLARE_METATYPE(GenieTableCommentList)


/**
 * @brief The GenieLink class models the genie's node link element
 */
class GenieLink: public Printable {
private:
    /**
     * @brief link's title
     */
    QString title = "";

    /**
     * @brief link's path
     */
    QString path = "";

public:
    /**
     * @brief GenieLink default constructor
     */
    GenieLink();

    /**
     * @brief retrieves the link's title
     * @return the link's title
     */
    QString getTitle() const;

    /**
     * @brief retrieves the link's path
     * @return the link's path
     */
    QString getPath() const;

    /**
     * @brief updates the link's title
     * @param value the new link's title value
     */
    void setTitle(const QString& value);

    /**
     * @brief updates the link's path
     * @param value the new link's path value
     */
    void setPath(const QString& value);

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
    bool operator==(const GenieLink& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const GenieLink& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const GenieLink& aOther) const;
};
Q_DECLARE_METATYPE(GenieLink)

/**
 * @brief The GenieLinkList class models a list of genie link objects
 * @see GenieLink
 */
class GenieLinkList: public QList<GenieLink>, public Printable{
public:
    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    QString toString() override;
};
Q_DECLARE_METATYPE(GenieLinkList)

/**
 * @brief The GenieNodeState class models a state associated to genie node
 */
class GenieNodeState: public Printable{
private:
    /**
     * @brief node state's id
     */
    QString id;

    /**
     * @brief node state's faultname property
     */
    QString faultname;

    /**
     * @brief node state's fix property
     */
    QString fix;

    /**
     * @brief node state's optional comment
     */
    QString comment;

    /**
     * @brief node state's list of links
     */
    ScopedGenieLinkList links;

public:
    /**
     * @brief QNGenieNodeState constructor
     */
    GenieNodeState();

    /**
     * @brief GenieNodeState copy constructor
     * @param aOther the genie node state object to be copied
     */
    GenieNodeState(const GenieNodeState& aOther);

    /**
     * @brief GenieNodeState assignment constructor
     * @param aOther the genie node state object to be copied
     * @return the copy of the passed object
     */
    GenieNodeState& operator=(const GenieNodeState& aOther);

    /**
     * GenieNodeState destructor
     */
    ~GenieNodeState();

    /**
     * @brief retrieves the node state's id
     * @return the node state's id
     */
    QString getId() const;

    /**
     * @brief retrieves the node state's faultname
     * @return the node state's faultname
     */
    QString getFaultname() const;

    /**
     * @brief retrieves the node state's fix
     * @return the node state's fix
     */
    QString getFix() const;

    /**
     * @brief retrieves the node state's comment
     * @return the node state's comment
     */
    QString getComment() const;

    /**
     * @brief retrieves the node state's link list
     * @return the node state's link list
     * @note if the node state has no links, the returned pointer will be null.
     * @warning do not delete the returned pointer;
     * that could cause a segmentation fault later on in the program execution.
     */
    GenieLinkListPtr getLinks() const;

    /**
     * @brief updates the node state's id
     * @param value the new id value
     */
    void setId(const QString& value);

    /**
     * @brief updates the node state's faultname
     * @param value the new faultname value
     */
    void setFaultname(const QString& value);

    /**
     * @brief updates the node state's fix
     * @param value the new fix value
     */
    void setFix(const QString& value);

    /**
     * @brief updates the node state's comment
     * @param value the new comment value
     */
    void setComment(const QString& value);

    /**
     * @brief updates the node state's list of links
     * @param aLinks the new list of links
     * @warning after the call, aLinks becomes owned by this object, which controls its lifcycle
     */
    void setLinks(GenieLinkListPtr aLinks);

    /**
     * @brief adds a link to the list
     * @param aLink the link to be added
     */
    void addLink(const GenieLink& aLink);

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
    bool operator==(const GenieNodeState& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const GenieNodeState& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const GenieNodeState& aOther) const;
};
Q_DECLARE_METATYPE(GenieNodeState)

/**
 * @brief The GenieNodeStateList class models a list of genie node's states objects
 * @see GenieNodeState
 */
class GenieNodeStateList: public QList<GenieNodeState>, public Printable{
public:
    /**
     * @brief produces a textual description of the entire object instance
     * @return a textual description for this instance
     */
    QString toString() override;
};
Q_DECLARE_METATYPE(GenieNodeStateList)

/**
 * @brief The GenieBarchart class models a barchart associated to genie node
 */
class GenieBarchart: public Printable{
private:
    /**
     * @brief barchart's active state
     */
    bool active;

    /**
     * @brief barchart's width
     */
    int width;

    /**
     * @brief barchart's height
     */
    int height;

public:
    /**
     * @brief GenieBarchart constructor
     */
    GenieBarchart();

    /**
     * @brief retrieves the barchart active state
     * @return the barchart active state
     */
    bool isActive() const;

    /**
     * @brief retrieves the barchart width
     * @return the barchart width
     */
    int getWidth() const;

    /**
     * @brief retrieves the barchart height
     * @return the barchart height
     */
    int getHeight() const;

    /**
     * @brief updates the barchart active state
     * @param value the new barchart active state
     */
    void setActive(bool value);

    /**
     * @brief updates the barchart width
     * @param value the new barchart width
     */
    void setWidth(int value);

    /**
     * @brief updates the barchart height
     * @param value the new barchart height
     */
    void setHeight(int value);

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
    bool operator==(const GenieBarchart& aOther) const;

    /**
     * @brief operator!= implementation
     * @param aOther the object to be compared with
     * @return true if this is NOT equal to aOther, false otherwise
     */
    bool operator!=(const GenieBarchart& aOther) const;

    /**
     * @brief operator< implementation
     * @param aOther the object to be compared with
     * @return true if this is lower to aOther, false otherwise
     */
    bool operator<(const GenieBarchart& aOther) const;
};
Q_DECLARE_METATYPE(GenieBarchart)

#endif // GENIETABLECOMMENT_H
