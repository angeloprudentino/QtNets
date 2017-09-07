/**
 ** @file smilexmlwriter.h
 ** @author Angelo Prudentino
 ** @date 2016-11-10
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

#ifndef SMILEXMLWRITER_H
#define SMILEXMLWRITER_H

#include <QXmlStreamWriter>
#include "logger.h"
#include "smiletypes.h"
#include "qnerror.h"
#include "qnnetwork.h"
#include "smilegenieext.h"

/**
 * @brief The SmileXmlWriter class models an XML writer for the smile format
 *
 * These xml documents will contain bayesian networks in SMILE format;
 * @note this class is not copyable
 */
class SmileXmlWriter {
private:
    Q_DISABLE_COPY(SmileXmlWriter)

    /**
     * @brief the path of the xml document to be written
     */
    QString smileDoc;

    /**
     * @brief logger instance
     */
    Logger log;

    /**
     * @brief determine error management's severity;
     * false means that non-critical errors are ignored.
     */
    bool strictMode = false;


    /**
     * @brief writes down the genie's node children
     * @param aWriter the xml writer instance
     * @param aTextbox the textbox object from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieTextbox() for information about the xml schema
     */
    QNError writeGenieTextbox(QXmlStreamWriter& aWriter, const QNTextBoxPtr aTextbox);

    /**
     * @brief writes down the genie's node tableComment children
     * @param aWriter the xml writer instance
     * @param aLinkList the list of link objects from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieLink() for information about the xml schema
     */
    QNError writeGenieLink(QXmlStreamWriter& aWriter, const GenieLinkList& aLinkList);

    /**
     * @brief writes down the genie's node states
     * @param aWriter the xml writer instance
     * @param aNode the node icon object from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieNode() for information about the xml schema
     */
    QNError writeGenieNodeStates(QXmlStreamWriter& aWriter, const QNNodeIconPtr aNode);

    /**
     * @brief writes down position elements
     * @param aWriter the xml writer instance
     * @param aPos the position element from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGeniePosition() for information about the xml schema
     */
    QNError writeGeniePosition(QXmlStreamWriter& aWriter, const QNPositionPtr aPos);

    /**
     * @brief writes down font elements
     * @param aWriter the xml writer instance
     * @param aFont the font element from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieFont() for information about the xml schema
     */
    QNError writeGenieFont(QXmlStreamWriter& aWriter, const QNFontPtr aFont);

    /**
     * @brief writes down the common properties of all genie's icons
     * @param aWriter the xml writer instance
     * @param aIcon the genie icon from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieIcon() for information about the xml schema
     */
    QNError writeGenieIcon(QXmlStreamWriter& aWriter, const QNIconPtr aIcon);

    /**
     * @brief writes down the genie's node children
     * @param aWriter the xml writer instance
     * @param aNode the node icon object from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieNode() for information about the xml schema
     */
    QNError writeGenieNode(QXmlStreamWriter& aWriter, const QNNodeIconPtr aNode);

    /**
     * @brief writes down the genie's model children
     * @param aWriter the xml writer instance
     * @param aModel the network's graphic extension model or a submodel from which take data
     * @return the error object resulting of the writing operation
     * @note this is a recursive method
     * @see SmileXmlParser#parseGenieModel() for information about the xml schema
     */
    QNError writeGenieModel(QXmlStreamWriter& aWriter, const QNModelPtr aModel);

    /**
     * @brief writes down the genie's archcomment children
     * @param aWriter the xml writer instance
     * @param aModel the network's graphic extension model from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieChildren() for information about the xml schema
     */
    QNError writeGenieArccomment(QXmlStreamWriter& aWriter, const QNNetworkModelPtr aModel);

    /**
     * @brief writes down the genie's diagautoformat child
     * @param aWriter the xml writer instance
     * @param aGenie the genie extensions object from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieDiagautoformat() for information about the xml schema
     */
    QNError writeGenieDiagautoformat(QXmlStreamWriter& aWriter, const SmileGenieExt& aGenie);

    /**
     * @brief writes down the genie's children elements
     * @param aWriter the xml writer instance
     * @param aGenie the genie extensions object from which take data
     * @param aModel the network's graphic extension model from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieChildren() for information about the xml schema
     */
    QNError writeGenieChildren(QXmlStreamWriter& aWriter, const SmileGenieExt& aGenie, const QNNetworkModelPtr aModel);

    /**
     * @brief writes down the genie's attributes
     * @param aWriter the xml writer instance
     * @param aGenie the genie extensions object from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseGenieAttributes() for information about the xml schema
     */
    QNError writeGenieAttributes(QXmlStreamWriter& aWriter, const SmileGenieExt& aGenie);

    /**
     * @brief writes down the smile's extensions
     * @param aWriter the xml writer instance
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileExtensions() for information about the xml schema
     */
    QNError writeSmileExtensions(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet);

    /**
     * @brief writes down common properties of smile node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseNodeProperties() for information about the xml schema
     */
    QNError writeNodeProperties(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down parents of smile node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseNodeParents() for information about the xml schema
     */
    QNError writeNodeParents(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down states of smile node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseNodeStates() for information about the xml schema
     */
    QNError writeNodeStates(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down common attributes of smile node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseNodeAttributes() for information about the xml schema
     */
    QNError writeNodeAttributes(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile cpt node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeCpt() for information about the xml schema
     */
    QNError writeSmileNodeCpt(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile noisyadder node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeNoisyadder() for information about the xml schema
     */
    QNError writeSmileNodeNoisyadder(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile noisymax node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeNoisymax() for information about the xml schema
     */
    QNError writeSmileNodeNoisymax(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile deterministic node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeDeterministic() for information about the xml schema
     */
    QNError writeSmileNodeDeterministic(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile decision node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeDecision() for information about the xml schema
     */
    QNError writeSmileNodeDecision(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile utility node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeUtility() for information about the xml schema
     */
    QNError writeSmileNodeUtility(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down smile mau node sub-elements
     * @param aWriter the xml writer instance
     * @param aNode the node reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodeMau() for information about the xml schema
     */
    QNError writeSmileNodeMau(QXmlStreamWriter& aWriter, const QNNodePtr aNode);

    /**
     * @brief writes down the smile's nodes sub-elements
     * @param aWriter the xml writer instance
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileNodes() for information about the xml schema
     */
    QNError writeSmileNodes(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet);

    /**
     * @brief writes down the smile's observationcost sub-elements
     * @param aWriter the xml writer instance
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileObscost() for information about the xml schema
     */
    QNError writeSmileObscost(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet);

    /**
     * @brief writes down the smile's algparams sub-element
     * @param aWriter the xml writer instance
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileAlgparams() for information about the xml schema
     */
    QNError writeSmileAlgparams(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet);

    /**
     * @brief writes down the smile's properties sub-element
     * @param aWriter the xml writer instance
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileProperties() for information about the xml schema
     */
    QNError writeSmileProperties(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet);

    /**
     * @brief writes down the smile's sub-elements
     * @param aWriter the xml writer instance
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parseSmileChildren() for information about the xml schema
     */
    QNError writeSmileChildren(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet);

public:
    /**
     * @brief SmileXmlWriter constructor
     * @param aSmileDoc the path of the xml document to be written
     * @param aStrictMode the error management's severity (optional, false by default)
     */
    SmileXmlWriter(const QString& aSmileDoc, const bool aStrictMode = false);

    /**
     * @brief writes the network down to the output xml file
     * @param aNet the network reference from which take data
     * @return the error object resulting of the writing operation
     * @see SmileXmlParser#parse() for information about the xml schema
     */
    QNError write(const ScopedQNNetwork& aNet);
};

#endif // SMILEXMLWRITER_H
