/**
 ** @file basestylexmlwriter.cpp
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

#include <QFile>
#include "basestylexmlwriter.h"
#include "basestyleconst.h"
#include "basestyletypes.h"
#include "qnstyle.h"
#include "qnstyleclass.h"

QNError BaseStyleXmlWriter::writeBackground(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass){
    aWriter.writeStartElement(STYLE_CLASS_BACKGROUND);

    //color
    QString color = aClass->getProperty(STYLE_CLASS_BACKGROUND_COLOR).toString();
    if(color.isEmpty()){
        if(this->strictMode){
            QString msg = "class bg color must be not empty";
            this->log.e("writeBackground", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            color = STYLE_DEFAULT_BG_COLOR;
        }
    }
    this->log.d("writeBackground", "class bg color: " + color);
    aWriter.writeAttribute(STYLE_COLOR, color);

    //brush
    QVariant qv = aClass->getProperty(STYLE_CLASS_BACKGROUND_BRUSH);
    QString brush;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class bg brush must be valid";
            this->log.e("writeBackground", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeBackground", "invalid class bg brush; fallback to default");
            brush = "solid";
        }
    }
    else{
        BSBrushStyle b = qv.value<BSBrushStyle>();
        switch (b) {
            case BSBrushStyle::transparent:
                brush = "transparent";
                break;
            case BSBrushStyle::solid:
                brush = "solid";
                break;
            case BSBrushStyle::dense1:
                brush = "dense1";
                break;
            case BSBrushStyle::dense2:
                brush = "dense2";
                break;
            case BSBrushStyle::dense3:
                brush = "dense3";
                break;
            case BSBrushStyle::dense4:
                brush = "dense4";
                break;
            case BSBrushStyle::dense5:
                brush = "dense5";
                break;
            case BSBrushStyle::dense6:
                brush = "dense6";
                break;
            case BSBrushStyle::dense7:
                brush = "dense7";
                break;
            case BSBrushStyle::horizontal:
                brush = "horizontal";
                break;
            case BSBrushStyle::vertiacal:
                brush = "vertiacal";
                break;
            case BSBrushStyle::cross:
                brush = "cross";
                break;
            case BSBrushStyle::backDiagonal:
                brush = "backDiagonal";
                break;
            case BSBrushStyle::forwardDiagonal:
                brush = "forwardDiagonal";
                break;
            case BSBrushStyle::crossDiagonal:
                brush = "crossDiagonal";
                break;
            case BSBrushStyle::linearGradient:
                brush = "linearGradient";
                break;
            case BSBrushStyle::radialGradient:
                brush = "radialGradient";
                break;
            default:
                brush = "solid";
                break;
        }
    }
    this->log.d("writeBackground", "class bg brush: " + brush);
    aWriter.writeAttribute(STYLE_CLASS_BACKGROUND_BRUSH, brush);

    aWriter.writeEndElement();
    return QNError();
}

QNError BaseStyleXmlWriter::writeOutline(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass){
    aWriter.writeStartElement(STYLE_CLASS_OUTLINE);

    //color
    QString color = aClass->getProperty(STYLE_CLASS_OUTLINE_COLOR).toString();
    if(color.isEmpty()){
        if(this->strictMode){
            QString msg = "class outline color must be not empty";
            this->log.e("writeOutline", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            color = STYLE_DEFAULT_FG_COLOR;
        }
    }
    this->log.d("writeOutline", "class outline color: " + color);
    aWriter.writeAttribute(STYLE_COLOR, color);

    //width
    bool ok;
    int w = aClass->getProperty(STYLE_CLASS_OUTLINE_WIDTH).toInt(&ok);
    if(!ok){
        if(this->strictMode){
            QString msg = "class outline width is invalid";
            this->log.e("writeOutline", msg);
            return QNError(STYLE_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeOutline", "invalid class outline width; fallback to default");
            w = STYLE_DEFAULT_WIDTH;
        }
    }
    this->log.d("writeOutline", "class outline width: " + QString::number(w));
    aWriter.writeAttribute(STYLE_CLASS_OUTLINE_WIDTH, QString::number(w));

    //line
    QVariant qv = aClass->getProperty(STYLE_CLASS_OUTLINE_LINE);
    QString line;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class outline style must be valid";
            this->log.e("writeOutline", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeOutline", "invalid class outline style; fallback to default");
            line = "solidLine";
        }
    }
    else{
        BSLineStyle l = qv.value<BSLineStyle>();
        switch (l) {
            case BSLineStyle::noLine:
                line = "noLine";
                break;
            case BSLineStyle::solidLine:
                line = "solidLine";
                break;
            case BSLineStyle::dashLine:
                line = "dashLine";
                break;
            case BSLineStyle::dotLine:
                line = "dotLine";
                break;
            case BSLineStyle::dashDotLine:
                line = "dashDotLine";
                break;
            case BSLineStyle::dashDotDotLine:
                line = "dashDotDotLine";
                break;
            default:
                line = "solidLine";
                break;
        }
    }
    this->log.d("writeOutline", "class outline style: " + line);
    aWriter.writeAttribute(STYLE_CLASS_OUTLINE_LINE, line);

    //cap
    qv = aClass->getProperty(STYLE_CLASS_OUTLINE_CAP);
    QString cap;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class outline cap must be valid";
            this->log.e("writeOutline", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeOutline", "invalid class outline cap; fallback to default");
            cap = "flatCap";
        }
    }
    else{
        BSCapStyle c = qv.value<BSCapStyle>();
        switch (c) {
            case BSCapStyle::flatCap:
                cap = "flatCap";
                break;
            case BSCapStyle::squareCap:
                cap = "squareCap";
                break;
            case BSCapStyle::roundCap:
                cap = "roundCap";
                break;
            default:
                cap = "flatCap";
                break;
        }
    }
    this->log.d("writeOutline", "class outline cap: " + cap);
    aWriter.writeAttribute(STYLE_CLASS_OUTLINE_CAP, cap);

    //join
    qv = aClass->getProperty(STYLE_CLASS_OUTLINE_JOIN);
    QString join;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class outline join must be valid";
            this->log.e("writeOutline", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeOutline", "invalid class outline join; fallback to default");
            join = "miterJoin";
        }
    }
    else{
        BSJoinStyle j = qv.value<BSJoinStyle>();
        switch (j) {
            case BSJoinStyle::miterJoin:
                join = "miterJoin";
                break;
            case BSJoinStyle::bevelJoin:
                join = "bevelJoin";
                break;
            case BSJoinStyle::roundJoin:
                join = "roundJoin";
                break;
            default:
                join = "miterJoin";
                break;
        }
    }
    this->log.d("writeOutline", "class outline join: " + join);
    aWriter.writeAttribute(STYLE_CLASS_OUTLINE_JOIN, join);

    aWriter.writeEndElement();
    return QNError();
}

QNError BaseStyleXmlWriter::writeFont(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass){
    aWriter.writeStartElement(STYLE_CLASS_FONT);

    //name
    QString name = aClass->getProperty(STYLE_CLASS_FONT_NAME).toString();
    if(name.isEmpty()){
        if(this->strictMode){
            QString msg = "class font name must be not empty";
            this->log.e("writeFont", msg);
            return QNError(STYLE_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeFont", "invalid class font name; fallback to default");
            name = STYLE_DEFAULT_FONT_NAME;
        }
    }
    this->log.d("writeFont", "class font name: " + name);
    aWriter.writeAttribute(STYLE_NAME, name);

    //size
    bool ok;
    int size = aClass->getProperty(STYLE_CLASS_FONT_SIZE).toInt(&ok);
    if(!ok){
        if(this->strictMode){
            QString msg = "class font size is invalid";
            this->log.e("writeFont", msg);
            return QNError(STYLE_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeFont", "invalid class font size; fallback to default");
            size = STYLE_DEFAULT_FONT_SIZE;
        }
    }
    this->log.d("writeFont", "class font size: " + QString::number(size));
    aWriter.writeAttribute(STYLE_CLASS_FONT_SIZE, QString::number(size));

    //color
    QString color = aClass->getProperty(STYLE_CLASS_FONT_COLOR).toString();
    if(color.isEmpty()){
        if(this->strictMode){
            QString msg = "class font color must be not empty";
            this->log.e("writeFont", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            color = STYLE_DEFAULT_FONT_COLOR;
        }
    }
    this->log.d("writeFont", "class font color: " + color);
    aWriter.writeAttribute(STYLE_COLOR, color);

    //bold
    bool bold = aClass->getProperty(STYLE_CLASS_FONT_BOLD).toBool();
    QString boldStr = (bold)? "true": "false";
    this->log.d("writeFont", "class font bold: " + boldStr);
    aWriter.writeAttribute(STYLE_CLASS_FONT_BOLD, boldStr);

    //italic
    bool italic = aClass->getProperty(STYLE_CLASS_FONT_ITALIC).toBool();
    QString italicStr = (italic)? "true": "false";
    this->log.d("writeFont", "class font italic: " + italicStr);
    aWriter.writeAttribute(STYLE_CLASS_FONT_ITALIC, italicStr);

    //align
    QVariant qv = aClass->getProperty(STYLE_CLASS_FONT_ALIGN);
    QString align;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class font align must be valid";
            this->log.e("writeFont", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeFont", "invalid class font align; fallback to default");
            align = "left";
        }
    }
    else{
        BSTextAlign a = qv.value<BSTextAlign>();
        switch (a) {
            case BSTextAlign::textLeft:
                align = "left";
                break;
            case BSTextAlign::textCenter:
                align = "center";
                break;
            case BSTextAlign::textRight:
                align = "right";
                break;
            default:
                align = "left";
                break;
        }
    }
    this->log.d("writeFont", "class font align: " + align);
    aWriter.writeAttribute(STYLE_CLASS_FONT_ALIGN, align);

    aWriter.writeEndElement();
    return QNError();
}

QNError BaseStyleXmlWriter::writeShape(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass){
    aWriter.writeStartElement(STYLE_CLASS_SHAPE);

    //shape name
    QVariant qv = aClass->getProperty(STYLE_CLASS_SHAPE);
    QString name;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class node shape must be valid";
            this->log.e("writeShape", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeShape", "invalid class node shape; fallback to default");
            name = "rectagle";
        }
    }
    else{
        BSNodeShape n = qv.value<BSNodeShape>();
        switch (n) {
            case BSNodeShape::square:
                name = "square";
                break;
            case BSNodeShape::roundedSquare:
                name = "roundedSquare";
                break;
            case BSNodeShape::rectagle:
                name = "rectagle";
                break;
            case BSNodeShape::roundedRect:
                name = "roundedRect";
                break;
            case BSNodeShape::rhombus:
                name = "rhombus";
                break;
            case BSNodeShape::triangle:
                name = "triangle";
                break;
            case BSNodeShape::reverseTriangle:
                name = "reverseTriangle";
                break;
            case BSNodeShape::ellipse:
                name = "ellipse";
                break;
            case BSNodeShape::circle:
                name = "circle";
                break;
            case BSNodeShape::hexagon:
                name = "hexagon";
                break;
            case BSNodeShape::image:
                name = "image";
                break;
            default:
                name = "rectagle";
                break;
        }
    }
    this->log.d("writeShape", "class node shape: " + name);
    aWriter.writeAttribute(STYLE_NAME, name);

    //image path
    qv = aClass->getProperty(STYLE_CLASS_SHAPE_PATH);
    QString path = qv.toString();
    if(!path.isEmpty()){
        this->log.d("writeShape", "class node image: " + path);
        aWriter.writeAttribute(STYLE_CLASS_SHAPE_PATH, path);
    }

    aWriter.writeEndElement();
    return QNError();
}

QNError BaseStyleXmlWriter::writeArrow(QXmlStreamWriter& aWriter, const QNStyleClassPtr aClass){
    aWriter.writeStartElement(STYLE_CLASS_ARROW);

    //shape name
    QVariant qv = aClass->getProperty(STYLE_CLASS_ARROW);
    QString arrow;
    if(!qv.isValid()){
        if(this->strictMode){
            QString msg = "class edge arrow must be valid";
            this->log.e("writeArrow", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.w("writeArrow", "invalid class edge arrow; fallback to default");
            arrow = "full";
        }
    }
    else{
        BSArrowStyle a = qv.value<BSArrowStyle>();
        switch (a) {
            case BSArrowStyle::none:
                arrow = "none";
                break;
            case BSArrowStyle::empty:
                arrow = "empty";
                break;
            case BSArrowStyle::full:
                arrow = "full";
                break;
            case BSArrowStyle::circular:
                arrow = "circular";
                break;
            case BSArrowStyle::diamond:
                arrow = "diamond";
                break;
            default:
                arrow = "full";
                break;
        }
    }
    this->log.d("writeArrow", "class edge arrow: " + arrow);
    aWriter.writeAttribute(STYLE_NAME, arrow);

    aWriter.writeEndElement();
    return QNError();
}

QNError BaseStyleXmlWriter::writeStyleClasses(QXmlStreamWriter& aWriter, const QNStylePtr aStyle){
    QNError err;

    QStringList names = aStyle->getAllClassesNames();
    foreach (QString n, names) {
        QNStyleClassPtr c = aStyle->getClass(n);
        if(c == nullptr){
            if(this->strictMode){
                QString msg = QString("%1 style class is null.").arg(n);
                this->log.e("writeStyleClasses", msg);
                return QNError(STYLE_WRITE_ERROR, msg);
            }
            else{
                QString msg = QString("%1 style class is null. Skip it").arg(n);
                this->log.w("writeStyleClasses", msg);
                continue;
            }
        }
        aWriter.writeStartElement(STYLE_CLASS);

        //class content
        this->log.d("writeStyleClasses", "class name: " + n);
        aWriter.writeAttribute(STYLE_NAME, n);
        err = this->writeBackground(aWriter, c);
        if(err) break;
        err = this->writeOutline(aWriter, c);
        if(err) break;
        err = this->writeFont(aWriter, c);
        if(err) break;
        err = this->writeShape(aWriter, c);
        if(err) break;
        err = this->writeArrow(aWriter, c);
        if(err) break;

        aWriter.writeEndElement();
    }

    return err;
}

BaseStyleXmlWriter::BaseStyleXmlWriter(const QString& aStyleDoc, const bool aStrictMode): log("baseStlye", "BaseStyleXmlWriter"){
    this->styleDoc = aStyleDoc;
    this->strictMode = aStrictMode;
}

QNError BaseStyleXmlWriter::write(const ScopedQNStyle& aStyle){
    if(aStyle.isNull()){
        QString msg("this style is empty.");
        this->log.e("write", msg);
        return QNError(STYLE_WRITE_ERROR, msg);
    }

    QFile file(this->styleDoc);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(4);

    //start writing doc
    writer.setCodec("ISO 8859-1");
    writer.writeStartDocument();

    //style root element
    writer.writeStartElement(STYLE);

    //style id
    QString id = aStyle->getId();
    if(id.isEmpty()){
        file.close();
        QString msg("this style has no id.");
        this->log.e("write", msg);
        return QNError(STYLE_WRITE_ERROR, msg);
    }
    this->log.d("write", "style id: " + id);
    writer.writeAttribute(STYLE_ID, id);

    //style version
    QString version = aStyle->getVersion();
    if(version.isEmpty()){
        file.close();
        QString msg("this style has no version.");
        this->log.e("write", msg);
        return QNError(STYLE_WRITE_ERROR, msg);
    }
    this->log.d("write", "style version: " + version);
    writer.writeAttribute(STYLE_VERSION, version);

    //style classes
    QNError err =  this->writeStyleClasses(writer, aStyle.data());
    if(err){
        file.close();
        return err;
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
    return QNError();
}

