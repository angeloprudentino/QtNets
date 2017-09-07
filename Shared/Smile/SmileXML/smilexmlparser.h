/**
 ** @file smilexmlparser.h
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

#ifndef SMILEXMLPARSER_H
#define SMILEXMLPARSER_H

#include <QString>
#include <QUrl>
#include <QSourceLocation>
#include <QAbstractMessageHandler>
#include <QXmlStreamReader>
#include <QXmlSchemaValidator>
#include <QXmlSchema>
#include "smilegenieext.h"
#include "logger.h"
#include "qnnetwork.h"
#include "qnnode.h"
#include "qnedge.h"
#include "qnnetworkmodel.h"
#include "smilenodegenieext.h"
#include "smiletypes.h"
#include "qnerror.h"


/**
 * @brief The SmileMessageHandler class manages all messages coming from the XML validation and parsing
 * framework
 *
 * the purpose is to redirect all those messages to log file according to their severity level
 * @see QAbstractMessageHandler
 */
class SmileMessageHandler: public QAbstractMessageHandler{
private:
    /**
     * @brief pointer to a logger instance
     */
    LoggerPtr log = nullptr;
public:
    /**
     * @brief SmileMessageHandler default constructor
     * @param aLog logger instance
     */
    SmileMessageHandler(LoggerPtr aLog);

    /**
     * @brief handleMessage is the method that process every message coming from the XML engine
     * @param type message severity level
     * @param description message detail
     * @param identifier the url of the item generating the message
     * @param sourceLocation the exact location of the message
     * @see QAbstractMessageHandler#handleMessage
     */
    void handleMessage(QtMsgType type, const QString &description,
                       const QUrl &identifier = QUrl(), const QSourceLocation &sourceLocation = QSourceLocation());
};

/**
 * @brief The SmileXmlParser class models an XML parser for the smile format
 *
 * These xml documents contain bayesian networks in SMILE format;
 * it is also responsible of populating an in memory structure that represents the network
 * @note this class is not copyable
 */
class SmileXmlParser{
private:
    Q_DISABLE_COPY(SmileXmlParser)

    /**
     * @brief the path of the xml document to be parsed
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
     * @brief checks edges correctness and removes invalid ones
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     */
    void checkEdges(QNNetworkPtr aNet);

    /**
     * @brief parseGenieTextbox
     * @param aXml the xml document instance
     * @param aTextbox the textbox object to be populated
     * @note aTextbox must be already allocated
     *
     *  here is the formal description for the textbox element:
     *  @code{.xml}
     *  <xs:element name="textbox">
     *       <xs:complexType>
     *            <xs:sequence>
     *                 <xs:element name="caption" type="xs:string" />
     *                 <xs:element name="font" type="alignedfont" />
     *                 <xs:element ref="position" />
     *                 <xs:element name="comment" type="xs:string" minOccurs="0" maxOccurs="1" />
     *            </xs:sequence>
     *       </xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     * @see parseGeniePosition
     * @see parseGenieFont
     */
    void parseGenieTextbox(QXmlStreamReader& aXml, QNTextBoxPtr aTextbox);

    /**
     * @brief parses a genie tableComment element
     * @param aXml the xml document instance
     * @param aTableComm the table comment object to be populated
     *
     * here is the formal description for the genie tableComment element:
     * @code{.xml}
     * <xs:complexType name="tableComment" mixed="true">
     *     <xs:attribute name="col" type="xs:integer" use="required" />
     *     <xs:attribute name="row" type="xs:integer" use="required" />
     * </xs:complexType>
     * @endcode
     */
    void parseGenieTablecomment(QXmlStreamReader& aXml, GenieTableComment& aTableComm);

    /**
     * @brief parses a genie link element
     * @param aXml the xml document instance
     * @param aLinkList the list of links to be populated
     * @note aLinkList will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     * @note if an error occurs, aLinkList will be null
     *
     * here is the formal description for the genie link element:
     * @code{.xml}
     * <xs:element name="link">
     *     <xs:complexType>
     *         <xs:attribute name="title" type="xs:string" use="required" />
     *         <xs:attribute name="path" type="xs:string" use="required" />
     *     </xs:complexType>
     * </xs:element>
     * @endcode
     */
    void parseGenieLink(QXmlStreamReader& aXml, GenieLinkListPtr* aLinkList);

    /**
     * @brief parses a genie node state element
     * @param aXml the xml document instance
     * @param aNodeState the genie node state to be populated
     * @see parseGenieNode
     */
    void parseGenieNodeState(QXmlStreamReader& aXml, GenieNodeState& aNodeState);

    /**
     * @brief parses the specific properties of node objects
     * @param aXml the xml document instance
     * @param aNode the node icon object to be populated
     * @note aNode must be already allocated
     *
     * here is the formal description for the genie node element:
     * @code{.xml}
     * <xs:element name="node">
     *      <xs:complexType>
     *           <xs:complexContent>
     *                <xs:extension base="icon">
     *                     <xs:sequence>
     *                          <xs:element name="state" minOccurs="0" maxOccurs="unbounded">
     *                               <xs:complexType>
     *                                    <xs:sequence>
     *                                         <xs:element name="fix" type="xs:string" minOccurs="0" maxOccurs="1" />
     *                                         <xs:element name="comment" type="xs:string" minOccurs="0" maxOccurs="1" />
     *                                         <xs:element ref="link" minOccurs="0" maxOccurs="unbounded" />
     *                                    </xs:sequence>
     *                                    <xs:attribute name="id" type="xs:token" use="required" />
     *                                    <xs:attribute name="faultname" type="xs:string" use="optional" />
     *                               </xs:complexType>
     *                          </xs:element>
     *                          <xs:element name="barchart" minOccurs="0" maxOccurs="1" >
     *                               <xs:complexType>
     *                                    <xs:attribute name="active" type="xs:boolean" use="required" />
     *                                    <xs:attribute name="width" type="xs:integer" use="required" />
     *                                    <xs:attribute name="height" type="xs:integer" use="required" />
     *                               </xs:complexType>
     *                          </xs:element>
     *                          <xs:element name="question" type="xs:string" minOccurs="0" maxOccurs="1" />
     *                          <xs:element ref="link" minOccurs="0" maxOccurs="unbounded" />
     *                          <xs:element name="defcomment" type="tableComment" minOccurs="0" maxOccurs="unbounded" />
     *                          <xs:element name="costcomment" type="tableComment" minOccurs="0" maxOccurs="unbounded" />
     *                     </xs:sequence>
     *                     <xs:attribute ref="faultnameformat" use="optional" default="inherit" />
     *                </xs:extension>
     *           </xs:complexContent>
     *      </xs:complexType>
     * </xs:element>
     * @endcode
     *
     * @see parseGenieIcon
     * @see parseGenieLink
     * @see parseGenieTablecomment
     */
    void parseGenieNode(QXmlStreamReader& aXml, QNNodeIconPtr aNode);

    /**
     * @brief parses the genie font sub-element
     * @param aXml the xml document instance
     * @param aFont the font object to be populated
     * @note aFont will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     * @note if an error occurs, aFont will be null
     *
     * here is the formal description for the genie font and alignedfont elements:
     * @code{.xml}
     * <xs:complexType name="fontType">
     *      <xs:attribute ref="color" use="required" />
     *      <xs:attribute name="name" type="xs:string" use="required" />
     *      <xs:attribute name="size" type="xs:integer" use="required" />
     *      <xs:attribute name="bold" type="xs:boolean" use="optional" default="false" />
     *      <xs:attribute name="italic" type="xs:boolean" use="optional" default="false" />
     * </xs:complexType>
     * <xs:element name="font" type="fontType" />
     *
     * <xs:complexType name="alignedfont">
     *      <xs:complexContent>
     *           <xs:extension base="fontType">
     *                <xs:attribute name="align" use="optional" default="left">
     *                     <xs:simpleType>
     *                          <xs:restriction base="xs:string">
     *                                <xs:enumeration value="left" />
     *                                <xs:enumeration value="right" />
     *                                <xs:enumeration value="center" />
     *                            </xs:restriction>
     *                     </xs:simpleType>
     *                </xs:attribute>
     *           </xs:extension>
     *      </xs:complexContent>
     * </xs:complexType>
     * @endcode
     */
    void parseGenieFont(QXmlStreamReader& aXml, QNFontPtr* aFont);

    /**
     * @brief parses the genie position element
     * @param aXml the xml document instance
     * @param aPos the position object to be populated
     * @note aPos will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     * @note if an error occurs, aPos will be null
     *
     * here is the formal description for the genie position element:
     * @code{.xml}
     * <xs:simpleType name="position">
     *      <xs:restriction base="intList">
     *           <xs:length value="4" />
     *      </xs:restriction>
     * </xs:simpleType>
     * <xs:element name="position" type="position" />
     * @endcode
     */
    void parseGeniePosition(QXmlStreamReader& aXml, QNPositionPtr* aPos);

    /**
     * @brief parses the common properties of node, submodel and textbox objects
     * @param aXml the xml document instance
     * @param aIcon the generic icon object to be populated
     * @note aIcon must be already allocated
     *
     * here is the formal description for the genie icon element:
     * @code{.xml}
     * <xs:simpleType name="colorType">
     *      <xs:restriction base="xs:string">
     *           <xs:pattern value="[0-9|a-f|A-F]{6}" />
     *      </xs:restriction>
     * </xs:simpleType>
     * <xs:attribute name="color" type="colorType" />
     *
     * <xs:complexType name="icon">
     *      <xs:sequence>
     *           <xs:element name="name" type="xs:string" />
     *           <xs:element name="interior">
     *                <xs:complexType>
     *                     <xs:attribute ref="color" use="required" />
     *                </xs:complexType>
     *           </xs:element>
     *           <xs:element name="outline">
     *                <xs:complexType>
     *                     <xs:attribute ref="color" use="required" />
     *                     <xs:attribute name="width" type="xs:integer" use="optional" default="1" />
     *                </xs:complexType>
     *           </xs:element>
     *           <xs:element ref="font" />
     *           <xs:element ref="position" />
     *           <xs:element name="comment" type="xs:string" minOccurs="0" maxOccurs="1" />
     *      </xs:sequence>
     *      <xs:attribute name="id" type="xs:token" use="required" />
     * </xs:complexType>
     * @endcode
     *
     * @see parseGeniePosition
     * @see parseGenieFont
     */
    void parseGenieIcon(QXmlStreamReader& aXml, QNIconPtr aIcon);

    /**
     * @brief parses genie's model (node, submodel, textbox) sub-element
     * @param aXml the xml document instance
     * @param aNetModel the network's graphic extension model to be populated
     * @param aModel the network's graphic extension model or a submodel to be populated
     * @note aNetModel must be already allocated
     * @note aModel must be already allocated
     *
     * here is the formal description for the genie submodel element:
     * @code{.xml}
     * <xs:element name="submodel">
     *      <xs:complexType>
     *           <xs:complexContent>
     *                <xs:extension base="icon">
     *                     <xs:sequence>
     *                          <xs:element name="window" type="position" minOccurs="0" maxOccurs="1"/>
     *                          <xs:choice minOccurs="0" maxOccurs="unbounded">
     *                               <xs:element ref="node" />
     *                               <xs:element ref="submodel" />
     *                               <xs:element ref="textbox" />
     *                          </xs:choice>
     *                     </xs:sequence>
     *                </xs:extension>
     *           </xs:complexContent>
     *      </xs:complexType>
     * </xs:element>
     * @endcode
     *
     * @see parseSmileExtensions
     * @see parseGeniePosition
     * @note this is a recursive method
     */
    void parseGenieModel(QXmlStreamReader& aXml, QNNetworkModelPtr aNetModel, QNModelPtr aModel);

    /**
     * @brief parses genie's arccomment sub-element
     * @param aXml the xml document instance
     * @param aModel the network's graphic extension model to be populated
     * @note aModel must be already allocated
     * @see parseSmileExtensions
     */
    void parseGenieArccomment(QXmlStreamReader& aXml, QNNetworkModelPtr aModel);

    /**
     * @brief parses genie's diagautoformat sub-element
     * @param aXml the xml document instance
     * @param aGenie the genie extension object to be populated
     * @see parseSmileExtensions
     */
    void parseGenieDiagautoformat(QXmlStreamReader& aXml, SmileGenieExt& aGenie);

    /**
     * @brief parses genie's sub-elements
     * @param aXml the xml document instance
     * @param aGenie the genie extension object to be populated
     * @param aModel the network's graphic extension model to be populated
     * @note aModel must be already allocated
     * @see parseSmileExtensions
     */
    void parseGenieChildren(QXmlStreamReader& aXml, SmileGenieExt& aGenie, QNNetworkModelPtr aModel);

    /**
     * @brief parses genie's attributes
     * @param aXml the xml document instance
     * @param aGenie the genie extension object to be populated
     * @see parseSmileExtensions
     */
    void parseGenieAttributes(QXmlStreamReader& aXml, SmileGenieExt& aGenie);

    /**
     * @brief parses smile (genie) graphic extensions
     * @param aXml the xml document instance
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     *
     * here is the formal description for the extensions element:
     * @code{.xml}
     * <xs:simpleType name="colorType">
     *      <xs:restriction base="xs:string">
     *           <xs:pattern value="[0-9|a-f|A-F]{6}" />
     *      </xs:restriction>
     * </xs:simpleType>
     *
     * <xs:attribute name="faultnameformat">
     *      <xs:simpleType>
     *           <xs:restriction base="xs:string">
     *                <xs:enumeration value="node" />
     *                <xs:enumeration value="nodestate" />
     *                <xs:enumeration value="user" />
     *                <xs:enumeration value="inherit" />
     *           </xs:restriction>
     *      </xs:simpleType>
     * </xs:attribute>
     *
     * <xs:element name="extensions">
     *      <xs:complexType>
     *           <xs:sequence>
     *                <xs:element name="genie">
     *                     <xs:complexType>
     *                          <xs:sequence>
     *                               <xs:element name="comment" type="xs:string" minOccurs="0" maxOccurs="1" />
     *                               <xs:element name="diagautoformat" minOccurs="0" maxOccurs="1">
     *                                    <xs:complexType>
     *                                         <xs:attribute name="target" type="colorType" use="required" />
     *                                         <xs:attribute name="targetnr" type="colorType" use="required" />
     *                                         <xs:attribute name="observation" type="colorType" use="required" />
     *                                         <xs:attribute name="observationnr" type="colorType" use="required" />
     *                                         <xs:attribute name="auxiliary" type="colorType" use="required" />
     *                                    </xs:complexType>
     *                               </xs:element>
     *                               <xs:choice minOccurs="0" maxOccurs="unbounded">
     *                                    <xs:element ref="node" />
     *                                    <xs:element ref="submodel" />
     *                                    <xs:element ref="textbox" />
     *                               </xs:choice>
     *                               <xs:element name="arccomment" minOccurs="0" maxOccurs="unbounded">
     *                                    <xs:complexType mixed="true">
     *                                         <xs:attribute name="parent" type="xs:token" use="required" />
     *                                         <xs:attribute name="child" type="xs:token" use="required" />
     *                                         <xs:attribute name="cost" type="xs:boolean" use="optional" default="false" />
     *                                    </xs:complexType>
     *                               </xs:element>
     *                          </xs:sequence>
     *                          <xs:attribute name="version" type="xs:string" use="required" />
     *                          <xs:attribute name="name" type="xs:string" use="required" />
     *                          <xs:attribute ref="faultnameformat" use="optional" default="user" />
     *                     </xs:complexType>
     *                </xs:element>
     *           </xs:sequence>
     *      </xs:complexType>
     * </xs:element>
     * @endcode
     */
    void parseSmileExtensions(QXmlStreamReader& aXml, QNNetworkPtr aNet);

    /**
     * @brief parses smile node's property sub-elements
     * @param aXml the xml document instance
     * @param aNode the smile node to which set properties
     * @note aNode must be already allocated
     *
     * here is the formal description for the property element:
     * @code{.xml}
     * <xs:element name="property">
     *     <xs:complexType>
     *        <xs:simpleContent>
     *            <xs:extension base="xs:string">
     *                <xs:attribute name="id" type="xs:token" use="required" />
     *            </xs:extension>
     *        </xs:simpleContent>
     *     </xs:complexType>
     * </xs:element>
     * @endcode
     */
    void parseNodeProperties(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses smile node's parents sub-element
     * @param aXml the xml document instance
     * @param aNode the smile node to which set parents
     * @note aNode must be already allocated
     *
     * here is the formal description for the parents element:
     * @code{.xml}
     * <xs:simpleType name="idList">
     *	  <xs:list itemType="xs:token" />
     * </xs:simpleType>
     *
     * <xs:element name="parents" type="idList" />
     * @endcode
     */
    void parseNodeParents(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses smile node's state sub-elements
     * @param aXml the xml document instance
     * @param aNode the smile node to which set attributes
     * @note aNode must be already allocated
     *
     * here is the formal description for the state element:
     * @code{.xml}
     * <xs:attributeGroup name="stateAttributes">
     *     <xs:attribute name="id" type="xs:token" use="required" />
     *     <xs:attribute name="label" type="xs:token" use="optional" />
     *     <xs:attribute name="target" type="xs:boolean" use="optional" />
     *     <xs:attribute name="default" type="xs:boolean" use="optional" />
     *     <xs:attribute name="fault" type="xs:boolean" use="optional" />
     * </xs:attributeGroup>
     *
     * <xs:element name="state">
     *     <xs:complexType>
     * 	       <xs:attributeGroup ref="stateAttributes" />
     * 	   </xs:complexType>
     * </xs:element>
     * @endcode
     */
    void parseNodeStates(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses smile node's common attributes
     * @param aXml the xml document instance
     * @param aNode the smile node to which set attributes
     * @note aNode must be already allocated
     *
     * here is the formal description for the nodeAttributes attribute group:
     * @code{.xml}
     * <xs:attributeGroup name="nodeAttributes">
     *     <xs:attribute name="id" type="xs:token" use="required" />
     *     <xs:attribute name="target" type="xs:boolean" use="optional" />
     *     <xs:attribute name="ranked" type="xs:boolean" use="optional" default="false" />
     *     <xs:attribute name="mandatory" type="xs:boolean" use="optional" default="false" />
     *     <xs:attribute name="diagtype" default="auxiliary" use="optional">
     *         <xs:simpleType>
     *             <xs:restriction base="xs:string">
     *                 <xs:enumeration value="target" />
     *                 <xs:enumeration value="observation" />
     *                 <xs:enumeration value="auxiliary" />
     *             </xs:restriction>
     *         </xs:simpleType>
     *     </xs:attribute>
     * </xs:attributeGroup>
     * @endcode
     */
    void parseNodeAttributes(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile cpt node
     * @param aXml the xml document instance
     * @param aNode the smile cpt node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the cpt node element:
     *  @code{.xml}
     *  <xs:simpleType name="doubleList">
     *  	<xs:list itemType="xs:double" />
     *  </xs:simpleType>
     *
     *  <xs:element name="cpt">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="state" minOccurs="2" maxOccurs="unbounded" />
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="probabilities" type="doubleList" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeCpt(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile noisyadder node
     * @param aXml the xml document instance
     * @param aNode the smile noisyadder node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the noisyadder node element:
     *  @code{.xml}
     *  <xs:simpleType name="intList">
     *  	<xs:list itemType="xs:integer" />
     *  </xs:simpleType>
     *
     *  <xs:simpleType name="doubleList">
     *  	<xs:list itemType="xs:double" />
     *  </xs:simpleType>
     *
     *  <xs:element name="noisyadder">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="state" minOccurs="2" maxOccurs="unbounded" />
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="dstates" type="intList" />
     *  			<xs:element name="weights" type="doubleList" />
     *  			<xs:element name="parameters" type="doubleList" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeNoisyadder(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile noisymax node
     * @param aXml the xml document instance
     * @param aNode the smile noisymax node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the noisymax node element:
     *  @code{.xml}
     *  <xs:simpleType name="intList">
     *  	<xs:list itemType="xs:integer" />
     *  </xs:simpleType>
     *
     *  <xs:simpleType name="doubleList">
     *  	<xs:list itemType="xs:double" />
     *  </xs:simpleType>
     *
     *  <xs:element name="noisymax">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="state" minOccurs="2" maxOccurs="unbounded" />
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="strengths" type="intList" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="parameters" type="doubleList" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeNoisymax(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile deterministic node
     * @param aXml the xml document instance
     * @param aNode the smile deterministic node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the deterministic node element:
     *  @code{.xml}
     *  <xs:simpleType name="idList">
     *     <xs:list itemType="xs:token" />
     *  </xs:simpleType>
     *
     *  <xs:element name="deterministic">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="state" minOccurs="2" maxOccurs="unbounded" />
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="resultingstates" type="idList" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeDeterministic(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile decision node
     * @param aXml the xml document instance
     * @param aNode the smile decision node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the decision node element:
     *  @code{.xml}
     *  <xs:element name="decision">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="state" minOccurs="2" maxOccurs="unbounded" />
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeDecision(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile utility node
     * @param aXml the xml document instance
     * @param aNode the smile utility node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the utility node element:
     *  @code{.xml}
     *  <xs:simpleType name="doubleList">
     *  	<xs:list itemType="xs:double" />
     *  </xs:simpleType>
     *
     *  <xs:element name="utility">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="utilities" type="doubleList" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeUtility(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses specific properties of a smile mau node
     * @param aXml the xml document instance
     * @param aNode the smile mau node to be populated
     * @note aNode must be already allocated
     *
     *  here is the formal description for the mau element:
     *  @code{.xml}
     *  <xs:simpleType name="doubleList">
     *  	<xs:list itemType="xs:double" />
     *  </xs:simpleType>
     *
     *  <xs:element name="mau">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *  			<xs:element name="weights" type="doubleList" />
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  		<xs:attributeGroup ref="nodeAttributes" />
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     *
     *  @see parseNodeParents();
     *  @see parseNodeAttributes();
     *  @see parseNodeProperties();
     */
    void parseSmileNodeMau(QXmlStreamReader& aXml, QNNodePtr aNode);

    /**
     * @brief parses smile element's nodes
     * @param aXml the xml document instance
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     *
     *  here is the formal description for the nodes element:
     *  @code{.xml}
     *  <xs:element name="nodes">
     *  	<xs:complexType>
     *  		<xs:choice minOccurs="0" maxOccurs="unbounded" >
     *  			<xs:element ref="cpt" />
     *  			<xs:element ref="noisymax" />
     *  			<xs:element ref="noisyadder" />
     *  			<xs:element ref="deterministic" />
     *  			<xs:element ref="decision" />
     *  			<xs:element ref="utility" />
     *  			<xs:element ref="mau" />
     *  		</xs:choice>
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     */
    void parseSmileNodes(QXmlStreamReader& aXml, QNNetworkPtr aNet);

    /**
     * @brief parses smile element's observationcost
     * @param aXml the xml document instance
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     *
     * here is the formal description for the observationcost element:
     * @code{.xml}
     * <xs:element name="observationcost">
     *		<xs:complexType>
     *			<xs:sequence>
     *				<xs:element name="node" minOccurs="0" maxOccurs="unbounded">
     *					<xs:complexType>
     *						<xs:sequence>
     *							<xs:element ref="parents" minOccurs="0" maxOccurs="1" />
     *							<xs:element name="cost" type="doubleList" />
     *						</xs:sequence>
     *						<xs:attribute name="id" type="xs:token" use="required" />
     *					</xs:complexType>
     *				</xs:element>
     *			</xs:sequence>
     *		</xs:complexType>
     * </xs:element>
     * @endcode
     */
    void parseSmileObscost(QXmlStreamReader& aXml, QNNetworkPtr aNet);

    /**
     * @brief parses smile element's algparams
     * @param aXml the xml document instance
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     *
     * here is the formal description for the algparams element:
     * @code{.xml}
     *	<xs:element name="algparams">
     *		<xs:complexType>
     *			<xs:all>
     *				<xs:element name="epis" minOccurs="0">
     *					<xs:complexType>
     *						<xs:attribute name="proplen" type="xs:integer" use="required" />
     *						<xs:attribute name="numstates1" type="xs:integer" use="required" />
     *						<xs:attribute name="numstates2" type="xs:integer" use="required" />
     *						<xs:attribute name="numstates3" type="xs:integer" use="required" />
     *						<xs:attribute name="eps1" type="xs:double" use="required" />
     *						<xs:attribute name="eps2" type="xs:double" use="required" />
     *						<xs:attribute name="eps3" type="xs:double" use="required" />
     *						<xs:attribute name="eps4" type="xs:double" use="required" />
     *					</xs:complexType>
     *				</xs:element>
     *			</xs:all>
     *		</xs:complexType>
     *	</xs:element>
     * @endcode
     */
    void parseSmileAlgparams(QXmlStreamReader& aXml, QNNetworkPtr aNet);

    /**
     * @brief parses smile element's properties
     * @param aXml the xml document instance
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     *
     *  here is the formal description for the properties element:
     *  @code{.xml}
     *  <xs:element name="property">
     *  	<xs:complexType>
     *  		<xs:simpleContent>
     *  			<xs:extension base="xs:string">
     *  				<xs:attribute name="id" type="xs:token" use="required" />
     *  			</xs:extension>
     *  		</xs:simpleContent>
     *  	</xs:complexType>
     *  </xs:element>
     *
     *  <xs:element name="properties">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="property" minOccurs="0" maxOccurs="unbounded" />
     *  		</xs:sequence>
     *  	</xs:complexType>
     *  </xs:element>
     *  @endcode
     */
    void parseSmileProperties(QXmlStreamReader& aXml, QNNetworkPtr aNet);

    /**
     * @brief parses smile element's childs
     * @param aXml the xml document instance
     * @param aNet the current network object instance
     * @note aNet must be already allocated
     *
     * here is the formal description for the root smile element:
     * @code{.xml}
     *  <xs:element name="smile">
     *  	<xs:complexType>
     *  		<xs:sequence>
     *  			<xs:element ref="properties" minOccurs="0" maxOccurs="1" />
     *  			<xs:element ref="algparams" minOccurs="0" maxOccurs="1" />
     *  			<xs:element ref="nodes" minOccurs="0" maxOccurs="1" />
     *  			<xs:element ref="observationcost" minOccurs="0" maxOccurs="1" />
     *  			<xs:element ref="extensions" minOccurs="0" maxOccurs="1" />
     *  		</xs:sequence>
     *  		<xs:attribute name="version" use="required" />
     *  		<xs:attribute name="id" type="xs:token" use="required" />
     *  		<xs:attribute name="numsamples" type="xs:integer" default="1000" />
     *  	</xs:complexType>
     *  </xs:element>
     * @endcode
     */
    void parseSmileChildren(QXmlStreamReader& aXml, QNNetworkPtr aNet);

public:
    /**
     * @brief SmileXmlParser constructor
     * @param aSmileDoc the path of the xml document to be parsed
     * @param aStrictMode the error management's severity (optional, false by default)
     */
    SmileXmlParser(const QString& aSmileDoc, const bool aStrictMode = false);

    /**
     * @brief parses the xml document and fills the in-memory smile graph representation
     * @param aNet reference to the whole network object
     * @return the error object resulting of the parsing operation
     * @note aNet will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     */
    QNError parse(ScopedQNNetwork& aNet);

    /**
     * @brief checks if the xml document satisfies rules expressed in the SMILE schema file
     * @param aSmileSchema the location of the smile schema file
     * @return the error object resulting of the schema validation
     *
     * @deprecated this method presents memory leaks due to the dependency on
     * QXmlSchema and QXmlSchemaValidator classes
     *
     * @see QXmlSchema
     * @see QXmlSchemaValidator
     */
    QNError validate(const QString& aSmileSchema);
};

#endif // SMILEXMLPARSER_H
