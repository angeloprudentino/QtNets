/**
 ** @file basestylexmlparser.h
 ** @author Angelo Prudentino
 ** @date 2016-12-29
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

#ifndef BASESTYLEXMLPARSER_H
#define BASESTYLEXMLPARSER_H


#include <QString>
#include <QUrl>
#include <QSourceLocation>
#include <QAbstractMessageHandler>
#include <QXmlStreamReader>
#include <QXmlSchemaValidator>
#include <QXmlSchema>
#include "logger.h"
#include "qnerror.h"
#include "qnstyletypes.h"


/**
 * @brief The BaseStyleMessageHandler class manages all messages coming from the XML validation and parsing
 * framework
 *
 * the purpose is to redirect all those messages to log file according to their severity level
 * @see QAbstractMessageHandler
 */
class BaseStyleMessageHandler: public QAbstractMessageHandler{
private:
    /**
     * @brief pointer to a logger instance
     */
    LoggerPtr log = nullptr;

public:
    /**
     * @brief BaseStyleMessageHandler default constructor
     * @param aLog logger instance
     */
    BaseStyleMessageHandler(LoggerPtr aLog);

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
 * @brief The BaseStyleXmlParser class models an XML parser for the custom xqsf style format
 * @note this class is not copyable
 */
class BaseStyleXmlParser{
private:
    Q_DISABLE_COPY(BaseStyleXmlParser)

    /**
     * @brief the path of the xml document to be parsed
     */
    QString styleDoc;

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
     * @brief parses style class's background sub-element
     * @param aXml the xml document instance
     * @param aStyleClass the current style class
     * @note aStyle must be already allocated
     *
     * here is the formal description for the background element:
     * @code{.xml}
     * <xs:simpleType name="colorType">
     *      <xs:restriction base="xs:string">
     *           <xs:pattern value="[0-9|a-f|A-F]{6}" />
     *      </xs:restriction>
     * </xs:simpleType>
     *
     * <xs:simpleType name="brushType">
     *      <xs:restriction base="xs:string">
     *            <xs:enumeration value="noBrush"/>
     *            <xs:enumeration value="solid"/>
     *            <xs:enumeration value="dense1"/>
     *            <xs:enumeration value="dense2"/>
     *            <xs:enumeration value="dense3"/>
     *            <xs:enumeration value="dense4"/>
     *            <xs:enumeration value="dense5"/>
     *            <xs:enumeration value="dense6"/>
     *            <xs:enumeration value="dense7"/>
     *            <xs:enumeration value="horizontal"/>
     *            <xs:enumeration value="vertiacal"/>
     *            <xs:enumeration value="cross"/>
     *            <xs:enumeration value="backDiagonal"/>
     *            <xs:enumeration value="forwardDiagonal"/>
     *            <xs:enumeration value="crossDiagonal"/>
     *            <xs:enumeration value="linearGradient"/>
     *            <xs:enumeration value="radialGradient"/>
     *        </xs:restriction>
     * </xs:simpleType>
     *
     * <xs:complexType name="backgroundType">
     *      <xs:attribute name="color" type="colorType" use="optional" default="ffffff"/>
     *      <xs:attribute name="brush" type="brushType" use="optional" default="solid"/>
     * </xs:complexType>
     * @endcode
     */
    void parseBackground(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass);

    /**
     * @brief parses style class's outline sub-element
     * @param aXml the xml document instance
     * @param aStyleClass the current style class
     * @note aStyle must be already allocated
     *
     * here is the formal description for the outline element:
     * @code{.xml}
     * <xs:simpleType name="colorType">
     *      <xs:restriction base="xs:string">
     *           <xs:pattern value="[0-9|a-f|A-F]{6}" />
     *      </xs:restriction>
     * </xs:simpleType>
     *
     * <xs:complexType name="outlineType">
     *      <xs:attribute name="color" type="colorType" use="optional" default="000000"/>
     *      <xs:attribute name="size" type="xs:integer" use="optional" default="1" />
     *      <xs:attribute name="line" use="optional" default="solidLine">
     *           <xs:simpleType>
     *                <xs:restriction base="xs:string">
     *                      <xs:enumeration value="noPen"/>
     *                      <xs:enumeration value="solidLine"/>
     *                      <xs:enumeration value="dashLine"/>
     *                      <xs:enumeration value="dotLine"/>
     *                      <xs:enumeration value="dashDotLine"/>
     *                      <xs:enumeration value="dashDotDotLine"/>
     *                  </xs:restriction>
     *           </xs:simpleType>
     *      </xs:attribute>
     *      <xs:attribute name="cap" use="optional" default="flatCap">
     *           <xs:simpleType>
     *                <xs:restriction base="xs:string">
     *                      <xs:enumeration value="flatCap"/>
     *                      <xs:enumeration value="squareCap"/>
     *                      <xs:enumeration value="roundCap"/>
     *                  </xs:restriction>
     *           </xs:simpleType>
     *      </xs:attribute>
     *      <xs:attribute name="join" use="optional" default="miterJoin">
     *           <xs:simpleType>
     *                <xs:restriction base="xs:string">
     *                      <xs:enumeration value="miterJoin"/>
     *                      <xs:enumeration value="bevelJoin"/>
     *                      <xs:enumeration value="roundJoin"/>
     *                  </xs:restriction>
     *           </xs:simpleType>
     *      </xs:attribute>
     * </xs:complexType>
     * @endcode
     */
    void parseOutline(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass);

    /**
     * @brief parses style class's font sub-element
     * @param aXml the xml document instance
     * @param aStyleClass the current style class
     * @note aStyle must be already allocated
     *
     * here is the formal description for the font element:
     * @code{.xml}
     * <xs:simpleType name="colorType">
     *      <xs:restriction base="xs:string">
     *           <xs:pattern value="[0-9|a-f|A-F]{6}" />
     *      </xs:restriction>
     * </xs:simpleType>
     *
     * <xs:complexType name="fontType">
     *      <xs:attribute name="name" type="xs:string" use="optional" default="Times"/>
     *      <xs:attribute name="size" type="xs:integer" use="optional" default="8"/>
     *      <xs:attribute name="color" type="colorType" use="optional" default="000000"/>
     *      <xs:attribute name="bold" type="xs:boolean" use="optional" default="false"/>
     *      <xs:attribute name="italic" type="xs:boolean" use="optional" default="false"/>
     *      <xs:attribute name="align" use="optional" default="left">
     *           <xs:simpleType>
     *                <xs:restriction base="xs:string">
     *                      <xs:enumeration value="left" />
     *                      <xs:enumeration value="right" />
     *                      <xs:enumeration value="center" />
     *                  </xs:restriction>
     *           </xs:simpleType>
     *      </xs:attribute>
     * </xs:complexType>
     * @endcode
     */
    void parseFont(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass);

    /**
     * @brief parses style class's shape sub-element
     * @param aXml the xml document instance
     * @param aStyleClass the current style class
     * @note aStyle must be already allocated
     * @note the shape element is considered only for nodes
     *
     * here is the formal description for the shape element:
     * @code{.xml}
     * <xs:complexType name="shapeType">
     *      <xs:attribute name="name" use="optional" default="rectagle">
     *           <xs:simpleType>
     *                <xs:restriction base="xs:string">
     *                      <xs:enumeration value="square"/>
     *                      <xs:enumeration value="roundedSquare"/>
     *                      <xs:enumeration value="rectagle"/>
     *                      <xs:enumeration value="roundedRect"/>
     *                      <xs:enumeration value="rhombus"/>
     *                      <xs:enumeration value="triangle"/>
     *                      <xs:enumeration value="reverseTriangle"/>
     *                      <xs:enumeration value="ellipse"/>
     *                      <xs:enumeration value="circle"/>
     *                      <xs:enumeration value="hexagon"/>
     *                  </xs:restriction>
     *           </xs:simpleType>
     *      </xs:attribute>
     * </xs:complexType>
     * @endcode
     */
    void parseShape(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass);

    /**
     * @brief parses style class's arrow sub-element
     * @param aXml the xml document instance
     * @param aStyleClass the current style class
     * @note aStyle must be already allocated
     * @note the arrow element is considered only for edges
     *
     * here is the formal description for the arrow element:
     * @code{.xml}
     * <xs:complexType name="arrowType">
     *      <xs:attribute name="name" use="optional" default="normal">
     *           <xs:simpleType>
     *                <xs:restriction base="xs:string">
     *                      <xs:enumeration value="none"/>
     *                      <xs:enumeration value="empty"/>
     *                      <xs:enumeration value="normal"/>
     *                      <xs:enumeration value="circular"/>
     *                      <xs:enumeration value="diamond"/>
     *                  </xs:restriction>
     *           </xs:simpleType>
     *      </xs:attribute>
     * </xs:complexType>
     * @endcode
     */
    void parseArrow(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass);

    /**
     * @brief parses style class elements's
     * @param aXml the xml document instance
     * @param aStyle the current style object instance
     * @note aStyle must be already allocated
     *
     * here is the formal description for the root style element:
     * @code{.xml}
     * <xs:element name="style">
     *      <xs:complexType>
     *           <xs:element name="class" minOccurs="0" maxOccurs="unbounded">
     *                <xs:complexType>
     *                     <xs:sequence>
     *                          <xs:element name="background" type="backgroundType" minOccurs="0"/>
     *                          <xs:element name="outline" type="outlineType" minOccurs="0"/>
     *                          <xs:element name="font" type="fontType" minOccurs="0"/>
     *                          <xs:element name="shape" type="shapeType" minOccurs="0"/>
     *                          <xs:element name="arrow" type="arrowType" minOccurs="0"/>
     *                     </xs:sequence>
     *                     <xs:attribute name="name" type="xs:token" use="required" />
     *                </xs:complexType>
     *           </xs:element>
     *           <xs:attribute name="id" type="xs:token" use="required" />
     *           <xs:attribute name="version" use="required" />
     *      </xs:complexType>
     * </xs:element>
     * @endcode
     */
    void parseStyleClasses(QXmlStreamReader& aXml, QNStylePtr aStyle);

public:
    /**
     * @brief BaseStyleXmlParser constructor
     * @param aStyleDoc the path of the xml document to be parsed
     * @param aStrictMode the error management's severity (optional, false by default)
     */
    BaseStyleXmlParser(const QString& aStyleDoc, const bool aStrictMode = false);

    /**
     * @brief parses the xml document and fills the map of style classes
     * @param aStyle the style object to be populated
     * @return the error object resulting of the parsing operation
     * @note aStyle will be constructed inside this method; if already pointing to something,
     * it will be cleared first
     */
    QNError parse(ScopedQNStyle& aStyle);

    /**
     * @brief checks if the xml document satisfies rules expressed in the schema file
     * @param aSchema the location of the smile schema file
     * @return the error object resulting of the schema validation
     *
     * @deprecated this method presents memory leaks due to the dependency on
     * QXmlSchema and QXmlSchemaValidator classes
     *
     * @see QXmlSchema
     * @see QXmlSchemaValidator
     */
    QNError validate(const QString& aSchema);
};

#endif // BASESTYLEXMLPARSER_H
