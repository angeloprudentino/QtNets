/**
 ** @file basestylexmlparser.cpp
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

#include <QUrl>
#include <QFile>
#include <QRegExp>
#include <QVariant>
#include <QRegExp>
#include "basestylexmlparser.h"
#include "basestyleconst.h"
#include "basestyletypes.h"
#include "qnstyle.h"
#include "qnstyleclass.h"


/*
 *
 * BaseStyleMessageHandler
 *
 */
BaseStyleMessageHandler::BaseStyleMessageHandler(LoggerPtr aLog): QAbstractMessageHandler() {
    this->log = aLog;
}

void BaseStyleMessageHandler::handleMessage(QtMsgType type, const QString &description,
                                            const QUrl &identifier, const QSourceLocation &sourceLocation){

    if(this->log != nullptr){
        QString msg = QString("Error (%1) at line %2, column %3: %4").
                      arg(identifier.toString()).arg(sourceLocation.line()).arg(sourceLocation.column()).
                      arg(QString(description).remove(QRegExp("<[^>]*>")));

        switch (type) {
            case QtDebugMsg:
            case QtInfoMsg:
                log->d("validate", msg);
                break;

            case QtWarningMsg:
                log->w("validate", msg);
                break;

            case QtCriticalMsg:
            case QtFatalMsg:
                log->e("validate", msg);
                break;

            default:
                log->e("validate", "default case -> " + msg);
                break;
        }
    }
}

/*
 *
 * BaseStyleXmlParser
 *
 */
void BaseStyleXmlParser::parseBackground(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseBackground", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == STYLE_CLASS_BACKGROUND){
                //color
                QString color = aXml.attributes().value(STYLE_COLOR).toString();
                if(color.isEmpty()){
                    color = STYLE_DEFAULT_BG_COLOR;
                    this->log.w("parseBackground", "background color (default): " + color);
                }
                else{
                    this->log.d("parseBackground", "background color: " + color);
                    QRegExp colorRegExp(COLOR_REGEXP);
                    if(!colorRegExp.exactMatch(color)){
                        if(this->strictMode){
                            QString err = "background color is not valid";
                            this->log.e("parseBackground", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            color = STYLE_DEFAULT_BG_COLOR;
                            this->log.w("parseBackground", "background color (default): " + color);
                        }
                    }
                }

                //brush
                QString brush = aXml.attributes().value(STYLE_CLASS_BACKGROUND_BRUSH).toString();
                BSBrushStyle bs;
                if(brush.isEmpty()){
                    bs = BSBrushStyle::solid;
                    this->log.w("parseBackground", "background brush (default): solid");
                }
                else{
                    this->log.d("parseBackground", "background brush: " + brush);
                    if(brush == "transparent")
                        bs = BSBrushStyle::transparent;
                    else if(brush == "solid")
                        bs = BSBrushStyle::solid;
                    else if(brush == "dense1")
                        bs = BSBrushStyle::dense1;
                    else if(brush == "dense2")
                        bs = BSBrushStyle::dense2;
                    else if(brush == "dense3")
                        bs = BSBrushStyle::dense3;
                    else if(brush == "dense4")
                        bs = BSBrushStyle::dense4;
                    else if(brush == "dense5")
                        bs = BSBrushStyle::dense5;
                    else if(brush == "dense6")
                        bs = BSBrushStyle::dense6;
                    else if(brush == "dense7")
                        bs = BSBrushStyle::dense7;
                    else if(brush == "horizontal")
                        bs = BSBrushStyle::horizontal;
                    else if(brush == "vertiacal")
                        bs = BSBrushStyle::vertiacal;
                    else if(brush == "cross")
                        bs = BSBrushStyle::cross;
                    else if(brush == "backDiagonal")
                        bs = BSBrushStyle::backDiagonal;
                    else if(brush == "forwardDiagonal")
                        bs = BSBrushStyle::forwardDiagonal;
                    else if(brush == "crossDiagonal")
                        bs = BSBrushStyle::crossDiagonal;
                    else if(brush == "linearGradient")
                        bs = BSBrushStyle::linearGradient;
                    else if(brush == "radialGradient")
                        bs = BSBrushStyle::radialGradient;
                    else {
                        if(this->strictMode){
                            QString err = "background brush is not valid";
                            this->log.e("parseBackground", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            bs = BSBrushStyle::solid;
                            this->log.w("parseBackground", "background brush (default): solid");
                        }
                    }
                }

                QVariant qv; qv.setValue(color);
                aStyleClass->setProperty(STYLE_CLASS_BACKGROUND_COLOR, qv);
                qv.setValue(bs);
                aStyleClass->setProperty(STYLE_CLASS_BACKGROUND_BRUSH, qv);
                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseBackground", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == STYLE_CLASS)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void BaseStyleXmlParser::parseOutline(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseOutline", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == STYLE_CLASS_OUTLINE){
                //color
                QString color = aXml.attributes().value(STYLE_COLOR).toString();
                if(color.isEmpty()){
                    color = STYLE_DEFAULT_FG_COLOR;
                    this->log.w("parseOutline", "outline color (default): " + color);
                }
                else{
                    this->log.d("parseOutline", "outline color: " + color);
                    QRegExp colorRegExp(COLOR_REGEXP);
                    if(!colorRegExp.exactMatch(color)){
                        if(this->strictMode){
                            QString err = "outline color is not valid";
                            this->log.e("parseOutline", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            color = STYLE_DEFAULT_FG_COLOR;
                            this->log.w("parseOutline", "outline color (default): " + color);
                        }
                    }
                }


                //width
                QString width = aXml.attributes().value(STYLE_CLASS_OUTLINE_WIDTH).toString();
                int w;
                if(width.isEmpty()){
                    w = STYLE_DEFAULT_WIDTH;
                    width = STYLE_DEFAULT_WIDTH;
                    this->log.w("parseOutline", "outline width (default): " + width);
                }
                else{
                    this->log.d("parseOutline", "outline width: " + width);
                    bool ok;
                    w = width.toInt(&ok);
                    if(!ok){
                        if(this->strictMode){
                            QString err = "outline width is not a valid integer";
                            this->log.e("parseOutline", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            w = STYLE_DEFAULT_WIDTH;
                            width = STYLE_DEFAULT_WIDTH;
                            this->log.w("parseOutline", "outline width (default): " + width);
                        }
                    }
                }
                if(w < 0){
                    w = STYLE_DEFAULT_WIDTH;
                    width = STYLE_DEFAULT_WIDTH;
                    this->log.w("parseOutline", "outline width (default): " + width);
                }

                //line
                QString line = aXml.attributes().value(STYLE_CLASS_OUTLINE_LINE).toString();
                BSLineStyle ls;
                if(line.isEmpty()){
                    ls = BSLineStyle::solidLine;
                    this->log.w("parseOutline", "outline line (default): solidLine");
                }
                else{
                    this->log.d("parseOutline", "outline line: " + line);
                    if(line == "noLine")
                        ls = BSLineStyle::noLine;
                    else if(line == "solidLine")
                        ls = BSLineStyle::solidLine;
                    else if(line == "dashLine")
                        ls = BSLineStyle::dashLine;
                    else if(line == "dotLine")
                        ls = BSLineStyle::dotLine;
                    else if(line == "dashDotLine")
                        ls = BSLineStyle::dashDotLine;
                    else if(line == "dashDotDotLine")
                        ls = BSLineStyle::dashDotDotLine;
                    else {
                        if(this->strictMode){
                            QString err = "outline line is not valid";
                            this->log.e("parseOutline", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            ls = BSLineStyle::solidLine;
                            this->log.w("parseOutline", "outline line (default): solidLine");
                        }
                    }
                }

                //cap
                QString cap = aXml.attributes().value(STYLE_CLASS_OUTLINE_CAP).toString();
                BSCapStyle cs;
                if(cap.isEmpty()){
                    cs = BSCapStyle::flatCap;
                    this->log.w("parseOutline", "outline cap (default): flatCap");
                }
                else{
                    this->log.d("parseOutline", "outline cap: " + cap);
                    if(cap == "flatCap")
                        cs = BSCapStyle::flatCap;
                    else if(cap == "squareCap")
                        cs = BSCapStyle::squareCap;
                    else if(cap == "roundCap")
                        cs = BSCapStyle::roundCap;
                    else {
                        if(this->strictMode){
                            QString err = "outline cap is not valid";
                            this->log.e("parseOutline", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            cs = BSCapStyle::flatCap;
                            this->log.w("parseOutline", "outline cap (default): flatCap");
                        }
                    }
                }

                //join
                QString join = aXml.attributes().value(STYLE_CLASS_OUTLINE_JOIN).toString();
                BSJoinStyle js;
                if(join.isEmpty()){
                    js = BSJoinStyle::miterJoin;
                    this->log.w("parseOutline", "outline join (default): miterJoin");
                }
                else{
                    this->log.d("parseOutline", "outline join: " + join);
                    if(join == "miterJoin")
                        js = BSJoinStyle::miterJoin;
                    else if(join == "bevelJoin")
                        js = BSJoinStyle::bevelJoin;
                    else if(join == "roundJoin")
                        js = BSJoinStyle::roundJoin;
                    else {
                        if(this->strictMode){
                            QString err = "outline join is not valid";
                            this->log.e("parseOutline", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            js = BSJoinStyle::miterJoin;
                            this->log.w("parseOutline", "outline join (default): miterJoin");
                        }
                    }
                }

                QVariant qv; qv.setValue(color);
                aStyleClass->setProperty(STYLE_CLASS_OUTLINE_COLOR, qv);
                qv.setValue(w);
                aStyleClass->setProperty(STYLE_CLASS_OUTLINE_WIDTH, qv);
                qv.setValue(ls);
                aStyleClass->setProperty(STYLE_CLASS_OUTLINE_LINE, qv);
                qv.setValue(cs);
                aStyleClass->setProperty(STYLE_CLASS_OUTLINE_CAP, qv);
                qv.setValue(js);
                aStyleClass->setProperty(STYLE_CLASS_OUTLINE_JOIN, qv);

                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseOutline", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == STYLE_CLASS)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void BaseStyleXmlParser::parseFont(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseFont", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == STYLE_CLASS_FONT){
                //name
                QString name = aXml.attributes().value(STYLE_NAME).toString();
                if(name.isEmpty()){
                    name = STYLE_DEFAULT_FONT_NAME;
                    this->log.w("parseFont", "font name (default): " + name);
                }
                else
                    this->log.d("parseFont", "font name: " + name);

                //size
                QString size = aXml.attributes().value(STYLE_CLASS_FONT_SIZE).toString();
                int s;
                if(size.isEmpty()){
                    s = STYLE_DEFAULT_FONT_SIZE;
                    size = STYLE_DEFAULT_FONT_SIZE;
                    this->log.w("parseFont", "font size (default): " + size);
                }
                else{
                    this->log.d("parseFont", "font size: " + size);
                    bool ok;
                    s = size.toInt(&ok);
                    if(!ok){
                        if(this->strictMode){
                            QString err = "font size is not a valid integer";
                            this->log.e("parseFont", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            s = STYLE_DEFAULT_FONT_SIZE;
                            size = STYLE_DEFAULT_FONT_SIZE;
                            this->log.w("parseFont", "font size (default): " + size);
                        }
                    }
                }
                if(s < 0){
                    s = STYLE_DEFAULT_FONT_SIZE;
                    size = STYLE_DEFAULT_FONT_SIZE;
                    this->log.w("parseFont", "font size (default): " + size);
                }

                //color
                QString color = aXml.attributes().value(STYLE_COLOR).toString();
                if(color.isEmpty()){
                    color = STYLE_DEFAULT_FONT_COLOR;
                    this->log.w("parseFont", "font color (default): " + color);
                }
                else{
                    this->log.d("parseFont", "font color: " + color);
                    QRegExp colorRegExp(COLOR_REGEXP);
                    if(!colorRegExp.exactMatch(color)){
                        if(this->strictMode){
                            QString err = "font color is not valid";
                            this->log.e("parseFont", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            color = STYLE_DEFAULT_FONT_COLOR;
                            this->log.w("parseFont", "font color (default): " + color);
                        }
                    }
                }

                //bold
                QString bold = aXml.attributes().value(STYLE_CLASS_FONT_BOLD).toString();
                if(!bold.isEmpty())
                    this->log.d("parseFont", "font bold: " + bold);
                bool b = (bold == "true")? true : false;
                this->log.d("parseFont", QString("font bold (final): %1").arg((b)? "true" : "false"));

                //italic
                QString italic = aXml.attributes().value(STYLE_CLASS_FONT_ITALIC).toString();
                if(!italic.isEmpty())
                    this->log.d("parseFont", "font italic: " + italic);
                bool i = (italic == "true")? true : false;
                this->log.d("parseFont", QString("font italic (final): %1").arg((i)? "true" : "false"));


                //align
                QString align = aXml.attributes().value(STYLE_CLASS_FONT_ALIGN).toString();
                BSTextAlign a;
                if(align.isEmpty()){
                    a = BSTextAlign::textLeft;
                    this->log.w("parseFont", "font align (default): left");
                }
                else{
                    this->log.d("parseFont", "font align: " + align);
                    if(align == "left")
                        a = BSTextAlign::textLeft;
                    else if(align == "right")
                        a = BSTextAlign::textRight;
                    else if(align == "center")
                        a = BSTextAlign::textCenter;
                    else {
                        if(this->strictMode){
                            QString err = "font align is not valid";
                            this->log.e("parseFont", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            a = BSTextAlign::textLeft;
                            this->log.w("parseFont", "font align (default): left");
                        }
                    }
                }

                QVariant qv; qv.setValue(name);
                aStyleClass->setProperty(STYLE_CLASS_FONT_NAME, qv);
                qv.setValue(s);
                aStyleClass->setProperty(STYLE_CLASS_FONT_SIZE, qv);
                qv.setValue(color);
                aStyleClass->setProperty(STYLE_CLASS_FONT_COLOR, qv);
                qv.setValue(b);
                aStyleClass->setProperty(STYLE_CLASS_FONT_BOLD, qv);
                qv.setValue(i);
                aStyleClass->setProperty(STYLE_CLASS_FONT_ITALIC, qv);
                qv.setValue(a);
                aStyleClass->setProperty(STYLE_CLASS_FONT_ALIGN, qv);

                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseOutline", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == STYLE_CLASS)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void BaseStyleXmlParser::parseShape(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseShape", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == STYLE_CLASS_SHAPE){
                //name
                QString name = aXml.attributes().value(STYLE_NAME).toString();
                BSNodeShape shape;
                if(name.isEmpty()){
                    shape = BSNodeShape::rectagle;
                    this->log.w("parseShape", "shape name (default): rectangle");
                }
                else{
                    this->log.d("parseShape", "shape name: " + name);
                    if(name == "square")
                        shape = BSNodeShape::square;
                    else if(name == "roundedSquare")
                        shape = BSNodeShape::roundedSquare;
                    else if(name == "rectagle")
                        shape = BSNodeShape::rectagle;
                    else if(name == "roundedRect")
                        shape = BSNodeShape::roundedRect;
                    else if(name == "rhombus")
                        shape = BSNodeShape::rhombus;
                    else if(name == "triangle")
                        shape = BSNodeShape::triangle;
                    else if(name == "reverseTriangle")
                        shape = BSNodeShape::reverseTriangle;
                    else if(name == "ellipse")
                        shape = BSNodeShape::ellipse;
                    else if(name == "circle")
                        shape = BSNodeShape::circle;
                    else if(name == "hexagon")
                        shape = BSNodeShape::hexagon;
                    else if(name == "image")
                        shape = BSNodeShape::image;
                    else {
                        if(this->strictMode){
                            QString err = "shape name is not valid";
                            this->log.e("parseShape", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            shape = BSNodeShape::rectagle;
                            this->log.w("parseShape", "shape name (default): rectangle");
                        }
                    }
                }

                QVariant qv; qv.setValue(shape);
                aStyleClass->setProperty(STYLE_CLASS_SHAPE, qv);

                //image path
                QString path = aXml.attributes().value(STYLE_CLASS_SHAPE_PATH).toString();
                aStyleClass->setProperty(STYLE_CLASS_SHAPE_PATH, path);

                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseOutline", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == STYLE_CLASS)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void BaseStyleXmlParser::parseArrow(QXmlStreamReader& aXml, QNStyleClassPtr aStyleClass){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseArrow", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == STYLE_CLASS_ARROW){
                //name
                QString name = aXml.attributes().value(STYLE_NAME).toString();
                BSArrowStyle arrow;
                if(name.isEmpty()){
                    arrow = BSArrowStyle::full;
                    this->log.w("parseArrow", "arrow name (default): full");
                }
                else{
                    this->log.d("parseArrow", "arrow name: " + name);
                    if(name == "none")
                        arrow = BSArrowStyle::none;
                    else if(name == "full")
                        arrow = BSArrowStyle::full;
                    else if(name == "empty")
                        arrow = BSArrowStyle::empty;
                    else if(name == "circular")
                        arrow = BSArrowStyle::circular;
                    else if(name == "diamond")
                        arrow = BSArrowStyle::diamond;
                    else {
                        if(this->strictMode){
                            QString err = "arrow name is not valid";
                            this->log.e("parseArrow", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            arrow = BSArrowStyle::full;
                            this->log.w("parseArrow", "arrow name (default): full");
                        }
                    }
                }

                QVariant qv; qv.setValue(arrow);
                aStyleClass->setProperty(STYLE_CLASS_ARROW, qv);

                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseOutline", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == STYLE_CLASS)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void BaseStyleXmlParser::parseStyleClasses(QXmlStreamReader& aXml, QNStylePtr aStyle){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseStyleChildren", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == STYLE_CLASS){
                //style class name
                QString name = aXml.attributes().value(STYLE_NAME).toString();
                if(name.isEmpty()){
                    QString msg = QString("a style class must have a non-empty name");
                    this->log.e("parseStyleChildren", msg);
                    aXml.raiseError(msg);
                    return;
                }
                this->log.d("parseStyleChildren", "class name: " + name);
                QNStyleClassPtr styleClass = new QNStyleClass(name);

                aXml.readNext();

                //background
                this->parseBackground(aXml, styleClass);

                //outline
                this->parseOutline(aXml, styleClass);

                //font
                this->parseFont(aXml, styleClass);

                //shape
                this->parseShape(aXml, styleClass);

                //arrow
                this->parseArrow(aXml, styleClass);

                if(aStyle->addClass(styleClass) == nullptr){
                    QString msg = QString("impossible to add %1 style class").arg(name);
                    this->log.e("parseStyleChildren", msg);
                    aXml.raiseError(msg);
                    return;
                }
            }
            else{
                this->log.e("parseStyleChildren", "Invalid style child start: " + aXml.name().toString());
                aXml.raiseError("Invalid style child start");
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseStyleChildren", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == STYLE)
                    return;
            }

            //skip token
            aXml.readNext();
        }
    }
}

BaseStyleXmlParser::BaseStyleXmlParser(const QString& aStyleDoc, const bool aStrictMode): log("baseStlye", "BaseStyleXmlParser"){
    this->styleDoc = aStyleDoc;
    this->strictMode = aStrictMode;
}

QNError BaseStyleXmlParser::parse(ScopedQNStyle& aStyle){
    QFile file(this->styleDoc);
    file.open(QIODevice::ReadOnly);
    QXmlStreamReader xml(&file);

    //start parsing
    xml.readNext();
    while(!xml.atEnd() && !xml.isStartElement()){
        this->log.d("parse", QString("%1").arg(xml.tokenString()));
        xml.readNext();
    }

    if(!xml.atEnd()){
        this->log.d("parse", QString("%1 tag start").arg(xml.name().toString()));
        if(xml.name() == STYLE){
            //style id - required
            QString id = xml.attributes().value(STYLE_ID).toString();
            if(id.isEmpty()){
                xml.raiseError("style's id must be not null");
            }
            else{
                this->log.d("parse", "style id: " + id);

                //style version
                QString version = xml.attributes().value(STYLE_VERSION).toString();
                if(version.isEmpty()){
                    xml.raiseError("style's version must be not null");
                }
                else{
                    this->log.d("parse", "style version: " + version);

                    aStyle.reset(new QNStyle(id, version));
                    if (!xml.hasError()){
                        //parse style's children
                        xml.readNext();
                        this->parseStyleClasses(xml, aStyle.data());
                    }
                }
            }
        }
        else{
            xml.raiseError(QString("Root element must be %1").arg(STYLE));
        }
    }
    file.close();

    if (xml.hasError()){
        QString err = QString("Failed to parse file %1: %2").arg(file.fileName()).arg(xml.errorString());
        this->log.e("parse", err);
        aStyle.reset();
        return QNError(STYLE_READ_ERROR, err);
    }
    else if (file.error() != QFile::NoError){
        QString err = QString("Cannot read file %1: %2").arg(file.fileName()).arg(file.errorString());
        this->log.e("parse", err);
        return QNError(STYLE_READ_ERROR, err);
    }

    return QNError();
}

QNError BaseStyleXmlParser::validate(const QString& aSchema){
    QFile schemaFile (aSchema);
    schemaFile.open(QIODevice::ReadOnly);
    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(this->styleDoc);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        BaseStyleMessageHandler h(&(this->log));
        validator.setMessageHandler(&h);
        bool valid = validator.validate(&file, QUrl::fromLocalFile(file.fileName()));
        schemaFile.close();
        file.close();

        if (valid)
            return QNError();
        else
            return QNError(STYLE_READ_ERROR, this->styleDoc + " does not match the schema file; see log for further details.");
    }
    else{
        schemaFile.close();
        QString msg = QString("%1 schema file is not valid.").arg(aSchema);
        this->log.e("validate", msg);
        return QNError(STYLE_READ_ERROR, msg);
    }
}
