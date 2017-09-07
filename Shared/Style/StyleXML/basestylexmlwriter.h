/**
 ** @file basestylexmlwriter.h
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

#ifndef BASESTYLEXMLWRITER_H
#define BASESTYLEXMLWRITER_H

#include <QString>
#include <QXmlStreamWriter>
#include "logger.h"
#include "qnerror.h"
#include "qnstyletypes.h"


/**
 * @brief The BaseStyleXmlWriter class models an XML writer for the custom xqsf style format
 * @note this class is not copyable
 */
class BaseStyleXmlWriter{
private:
    Q_DISABLE_COPY(BaseStyleXmlWriter)

    /**
     * @brief the path of the xml document to be stored
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
     * @brief writes down style class's background sub-element
     * @param aWriter the xml writer instance
     * @param aClass the style class reference from which take data
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parseBackground() for information about the xml schema
     */
    QNError writeBackground(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass);

    /**
     * @brief writes down style class's outline sub-element
     * @param aWriter the xml writer instance
     * @param aClass the style class reference from which take data
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parseOutline() for information about the xml schema
     */
    QNError writeOutline(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass);

    /**
     * @brief writes down style class's font sub-element
     * @param aWriter the xml writer instance
     * @param aClass the style class reference from which take data
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parseFont() for information about the xml schema
     */
    QNError writeFont(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass);

    /**
     * @brief writes down style class's shape sub-element
     * @param aWriter the xml writer instance
     * @param aClass the style class reference from which take data
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parseShape() for information about the xml schema
     */
    QNError writeShape(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass);

    /**
     * @brief writes down style class's arrow sub-element
     * @param aWriter the xml writer instance
     * @param aClass the style class reference from which take data
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parseArrow() for information about the xml schema
     */
    QNError writeArrow(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass);

    /**
     * @brief writes down the style classes
     * @param aWriter the xml writer instance
     * @param aStyle the style reference from which take data
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parseStyleClasses() for information about the xml schema
     */
    QNError writeStyleClasses(QXmlStreamWriter& aWriter, const QNStylePtr aStyle);

public:
    /**
     * @brief BaseStyleXmlWriter constructor
     * @param aStyleDoc the path of the xml document to be stored
     * @param aStrictMode the error management's severity (optional, false by default)
     */
    BaseStyleXmlWriter(const QString& aStyleDoc, const bool aStrictMode = false);

    /**
     * @brief writes the style down to the output xml file
     * @param aStyle the style object to be populated
     * @return the error object resulting of the writing operation
     * @see BaseStyleXmlParser#parse() for information about the xml schema
     */
    QNError write(const ScopedQNStyle& aStyle);
};

#endif // BASESTYLEXMLWRITER_H
