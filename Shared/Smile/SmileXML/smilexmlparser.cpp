/**
 ** @file smilexmlparser.cpp
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

#include <QUrl>
#include <QFile>
#include <QRegExp>
#include <QVariant>
#include <QRegExp>
#include "smilexmlconst.h"
#include "smilenode.h"
#include "smilealgparams.h"
#include "smileobservationcost.h"
#include "smilexmlparser.h"
#include "qnnodeicon.h"
#include "qngraphicconst.h"

/*
 *
 * SmileMessageHandler
 *
 */
SmileMessageHandler::SmileMessageHandler(LoggerPtr aLog): QAbstractMessageHandler() {
    this->log = aLog;
}

void SmileMessageHandler::handleMessage(QtMsgType type, const QString &description,
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
 * SmileXmlParser
 *
 */
void SmileXmlParser::checkEdges(QNNetworkPtr aNet){
    if(aNet != nullptr){
        QStringList keys = aNet->getAllEdgesKeys();
        foreach (QString k, keys) {
            QNEdgePtr a = aNet->getEdge(k);
            QStringList ids = k.split(" - ");
            if(ids.length() != 2){
                this->log.w("checkEdges", k + " is not a valid edge id. Edge ignored.");
                aNet->removeEdge(a);
                continue;
            }

            //check src existence
            QString srcId = ids.at(0);
            QNNodePtr src = aNet->getNode(srcId);
            if(src == nullptr){
                this->log.w("checkEdges", srcId + " src node does not exist. Edge ignored.");
                aNet->removeEdge(a);
                continue;
            }
            a->setSrc(src);

            //check dst existence
            QString dstID = ids.at(1);
            QNNodePtr dst = aNet->getNode(dstID);
            if(dst == nullptr){
                this->log.w("checkEdges", dstID + " dst node does not exist. Edge ignored.");
                aNet->removeEdge(a);
                continue;
            }
            a->setDst(dst);

            //update nodes
            src->addSuccessor(dst);
            dst->addAncestor(src);
        }
    }
}

void SmileXmlParser::parseGenieTextbox(QXmlStreamReader& aXml, QNTextBoxPtr aTextbox){
    //show content - optional
    QString toShowString = aXml.attributes().value(SHOW_TAG).toString();
    bool toShow = (toShowString.isEmpty() || toShowString  == "true");
    aTextbox->setShowContent(toShow);
    if(toShow)
        this->log.d("parseGenieTextbox", "textbox is showing content");
    else
        this->log.d("parseGenieTextbox", "textbox is hiding content");

    aXml.readNext();

    //caption - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieTextbox", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_CAPTION){
                QString caption = aXml.readElementText();
                this->log.d("parseGenieTextbox", "textbox caption: " + caption);
                aTextbox->setCaption(caption);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_CAPTION).arg(aXml.name().toString());
                this->log.e("parseGenieTextbox", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseGenieTextbox", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //font - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieTextbox", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_FONT){
                QNFontPtr font = nullptr;
                this->parseGenieFont(aXml, &font);
                if(font == nullptr)
                    return; //there is an error in the font

                aTextbox->setFont(font);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_FONT).arg(aXml.name().toString());
                this->log.e("parseGenieTextbox", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieTextbox", QString("%1 tag end").arg(aXml.name().toString()));
            }
            //skip current token
            aXml.readNext();
        }
    }

    //position - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieTextbox", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_POS){
                QNPositionPtr pos = nullptr;
                this->parseGeniePosition(aXml, &pos);
                if(pos == nullptr)
                    return; //there is an error in the pos

                aTextbox->setPosition(pos);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_POS).arg(aXml.name().toString());
                this->log.e("parseGenieTextbox", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieTextbox", QString("%1 tag end").arg(aXml.name().toString()));
            }
            //skip current token
            aXml.readNext();
        }
    }

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieTextbox", QString("%1 tag start").arg(aXml.name().toString()));
            //comment - optional
            if(aXml.name() == SMILE_GENIE_COMMENT){
                QString comment = aXml.readElementText();
                this->log.d("parseGenieTextbox", "icon comment: " + comment);
                aTextbox->setComment(comment);
                aXml.readNext();
            }
            //classes - optional
            else if(aXml.name() == CLASSES_TAG){
                QString classes_str = aXml.readElementText();
                this->log.d("parseGenieTextbox", "classes: " + classes_str);
                QStringList l = classes_str.split(", ");
                foreach (QString c, l) {
                    aTextbox->addClass(c);
                }
                aXml.readNext();
            }
            //label - optional
            else if(aXml.name() == LABEL_TAG){
                int len = aXml.attributes().value(LABEL_LEN_TAG).toInt();
                QString prop = aXml.attributes().value(LABEL_PROP_TAG).toString();
                this->log.d("parseGenieTextbox", "label found: " + prop + ", " + len);
                aTextbox->setLabelLen(len);
                aTextbox->updatePropToShow(prop);

                aXml.readNext();
            }
            else
                break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieTextbox", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_TEXTBOX)
                    return; //end recursive call
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieTablecomment(QXmlStreamReader& aXml, GenieTableComment& aTableComm){
    //col - required
    QString col = aXml.attributes().value(SMILE_GENIE_COL).toString();
    if(col.isEmpty()){
        QString err = "genie tableComment's col is required";
        this->log.e("parseGenieTablecomment", err);
        aXml.raiseError(err);
        return;
    }
    bool ok;
    int c = col.toInt(&ok);
    if(!ok){
        QString err = "genie tableComment's col must be an integer";
        this->log.e("parseGenieTablecomment", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGenieTablecomment", "genie tableComment's col: " + col);
    aTableComm.setCol(c);

    //row - required
    QString row = aXml.attributes().value(SMILE_GENIE_ROW).toString();
    if(row.isEmpty()){
        QString err = "genie tableComment's row is required";
        this->log.e("parseGenieTablecomment", err);
        aXml.raiseError(err);
        return;
    }
    int r = row.toInt(&ok);
    if(!ok){
        QString err = "genie tableComment's row must be an integer";
        this->log.e("parseGenieTablecomment", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGenieTablecomment", "genie tableComment's row: " + row);
    aTableComm.setRow(r);
}

void SmileXmlParser::parseGenieLink(QXmlStreamReader& aXml, GenieLinkListPtr* aLinkList){
    if(*aLinkList != nullptr)
        delete *aLinkList;
    *aLinkList = nullptr;

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieLink", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_LINK){
                GenieLink link;

                //link title - required
                QString title = aXml.attributes().value(SMILE_GENIE_LINK_TITLE).toString();
                if(title.isEmpty() && this->strictMode){
                    QString err = "genie link title is required";
                    this->log.e("parseGenieLink", err);
                    aXml.raiseError(err);
                    if(*aLinkList != nullptr)
                        delete *aLinkList;
                    *aLinkList = nullptr;

                    return;
                }
                this->log.d("parseGenieLink", "genie link title: " + title);
                link.setTitle(title);

                //link path - required
                QString path = aXml.attributes().value(SMILE_GENIE_LINK_PATH).toString();
                if(path.isEmpty() && this->strictMode){
                    QString err = "genie link path is required";
                    this->log.e("parseGenieLink", err);
                    aXml.raiseError(err);
                    if(*aLinkList != nullptr)
                        delete *aLinkList;
                    *aLinkList = nullptr;

                    return;
                }
                this->log.d("parseGenieLink", "genie link path: " + path);
                link.setPath(path);

                //add link to the list
                if(*aLinkList == nullptr)
                    *aLinkList = new GenieLinkList();
                (*aLinkList)->push_back(link);
                aXml.readNext();
            }
            else{
                //link list is complete
                break;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieLink", QString("%1 tag end").arg(aXml.name().toString()));

                if((aXml.name() == SMILE_GENIE_NODE_STATE) || (aXml.name() == SMILE_GENIE_NODE))
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieNodeState(QXmlStreamReader& aXml, GenieNodeState& aNodeState){
    //id - required
    QString id = aXml.attributes().value(SMILE_ID_TAG).toString();
    if(id.isEmpty()){
        QString err = "genie node state's id is required";
        this->log.e("parseGenieNodeState", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGenieNodeState", "node state's id: " + id);
    aNodeState.setId(id);

    //faultname - optional
    QString faultname = aXml.attributes().value(SMILE_GENIE_NODE_FAULTNAME).toString();
    if(!faultname.isEmpty()){
        this->log.d("parseGenieNodeState", "node state's faultname: " + faultname);
        aNodeState.setFaultname(faultname);
    }

    aXml.readNext();

    //fix - optional
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNodeState", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_FIX){
                QString fix = aXml.readElementText();
                this->log.d("parseGenieNodeState", "node state's fix: " + fix);
                aNodeState.setFix(fix);
                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNodeState", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE_STATE)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }

    //comment - optional
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNodeState", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_COMMENT){
                QString comment = aXml.readElementText();
                this->log.d("parseGenieNodeState", "node state's comment: " + comment);
                aNodeState.setComment(comment);
                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNodeState", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE_STATE)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }

    //link - optional
    GenieLinkListPtr list = nullptr;
    this->parseGenieLink(aXml, &list);
    if(list != nullptr)
        aNodeState.setLinks(list);
}

void SmileXmlParser::parseGenieNode(QXmlStreamReader& aXml, QNNodeIconPtr aNode){
    //state - list
    GenieNodeStateList nl;
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNode", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_NODE_STATE){
                GenieNodeState n;
                this->parseGenieNodeState(aXml, n);
                if(aXml.hasError())
                    return; //error parsing state
                nl.push_back(n);
            }
            else{
                //no more states
                if(!nl.isEmpty()){
                    QVariant qv; qv.setValue(nl);
                    aNode->setProperty(SMILE_GENIE_NODE_STATE, qv);
                }
                break;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNode", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE){
                    //no more states
                    if(!nl.isEmpty()){
                        QVariant qv; qv.setValue(nl);
                        aNode->setProperty(SMILE_GENIE_NODE_STATE, qv);
                    }
                    return;
                }
            }

            //skip current token
            aXml.readNext();
        }
    }

    //barchart - optional
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNode", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_BARCHART){
                GenieBarchart bc;

                //active - required
                QString active = aXml.attributes().value(SMILE_GENIE_BARCHART_ACTIVE).toString();
                if(active.isEmpty() && this->strictMode){
                    QString err = "genie barchart's active is required";
                    this->log.e("parseGenieNode", err);
                    aXml.raiseError(err);
                    return;
                }
                this->log.d("parseGenieNode", "genie barchart's active: " + active);
                bc.setActive((active == "true") ? true : false);

                //width - required
                QString width = aXml.attributes().value(SMILE_GENIE_BARCHART_W).toString();
                if(width.isEmpty()){
                    QString err = "genie barchart's width is required";
                    this->log.e("parseGenieNode", err);
                    aXml.raiseError(err);
                    return;
                }
                bool ok;
                int w = width.toInt(&ok);
                if(!ok){
                    QString err = "genie barchart's width must be an integer";
                    this->log.e("parseGenieNode", err);
                    aXml.raiseError(err);
                    return;
                }
                this->log.d("parseGenieNode", "genie barchart's width: " + width);
                bc.setWidth(w);

                //height - required
                QString height = aXml.attributes().value(SMILE_GENIE_BARCHART_H).toString();
                if(height.isEmpty()){
                    QString err = "genie barchart's height is required";
                    this->log.e("parseGenieNode", err);
                    aXml.raiseError(err);
                    return;
                }
                int h = height.toInt(&ok);
                if(!ok){
                    QString err = "genie barchart's height must be an integer";
                    this->log.e("parseGenieNode", err);
                    aXml.raiseError(err);
                    return;
                }
                this->log.d("parseGenieNode", "genie barchart's height: " + height);
                bc.setHeight(h);

                QVariant qv; qv.setValue(bc);
                aNode->setProperty(SMILE_GENIE_BARCHART, qv);
                aXml.readNext();
            }
            else
                break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNode", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }

    //question - optional
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNode", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_QUESTION){
                QString question = aXml.readElementText();
                if(!question.isEmpty()){
                    this->log.d("parseGenieNode", "genie node's question: " + question);
                    aNode->setProperty(SMILE_GENIE_QUESTION, question);
                }
                aXml.readNext();
            }
            else
                break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNode", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }

    //link - list
    GenieLinkListPtr list = nullptr;
    this->parseGenieLink(aXml, &list);
    if(list != nullptr){
        //store a copy of the list and delete the original;
        //that's the way QVariant works
        QVariant qv; qv.setValue(*list);
        delete list;
        aNode->setProperty(SMILE_GENIE_LINK, qv);
    }

    //defcomment - list
    GenieTableCommentList dcList;
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNode", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_DEFCOMMENT){
                GenieTableComment tc;
                this->parseGenieTablecomment(aXml, tc);
                if(aXml.hasError())
                    return; //error parsing defcomment
                dcList.push_back(tc);
                aXml.readNext();
            }
            else{
                //no more defcomment elements
                if(!dcList.isEmpty()){
                    QVariant qv; qv.setValue(dcList);
                    aNode->setProperty(SMILE_GENIE_DEFCOMMENT, qv);
                }
                break;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNode", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE){
                    //no more defcomment elements
                    if(!dcList.isEmpty()){
                        QVariant qv; qv.setValue(dcList);
                        aNode->setProperty(SMILE_GENIE_DEFCOMMENT, qv);
                    }
                    return;
                }
            }

            //skip current token
            aXml.readNext();
        }
    }

    //costcomment - list
    GenieTableCommentList ccList;
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieNode", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_COSTCOMMENT){
                GenieTableComment tc;
                this->parseGenieTablecomment(aXml, tc);
                if(aXml.hasError())
                    return; //error parsing costcomment
                ccList.push_back(tc);
                aXml.readNext();
            }
            else{
                //no more costcomment elements
                if(!ccList.isEmpty()){
                    QVariant qv; qv.setValue(ccList);
                    aNode->setProperty(SMILE_GENIE_COSTCOMMENT, qv);
                }
                break;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieNode", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_NODE){
                    //no more costcomment elements
                    if(!ccList.isEmpty()){
                        QVariant qv; qv.setValue(ccList);
                        aNode->setProperty(SMILE_GENIE_COSTCOMMENT, qv);
                    }
                    return;
                }
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieFont(QXmlStreamReader& aXml, QNFontPtr* aFont){
    if(*aFont != nullptr)
        delete (*aFont);
    *aFont = nullptr;

    //color - required
    QString color = aXml.attributes().value(SMILE_GENIE_COLOR).toString();
    QRegExp colorRegExp(COLOR_REGEXP);
    if(color.isEmpty() || !colorRegExp.exactMatch(color)){
        if(this->strictMode){
            QString err = "genie font's interior color is not valid";
            this->log.e("parseGenieFont", err);
            aXml.raiseError(err);
            return;
        }
        else{
            color = DEFAULT_FONT_COLOR;
            this->log.d("parseGenieFont", "font color: (default) " + color);
        }
    }
    this->log.d("parseGenieFont", "font color: " + color);

    //name - required
    QString name = aXml.attributes().value(SMILE_GENIE_NAME).toString();
    if(name.isEmpty()){
        if(this->strictMode){
            QString err = "genie font name is required";
            this->log.e("parseGenieFont", err);
            aXml.raiseError(err);
            return;
        }
        else{
            name = DEFAULT_FONT_NAME;
            this->log.d("parseGenieFont", "font name: (default) " + name);
        }
    }
    this->log.d("parseGenieFont", "font name: " + name);

    //size - required
    QString s = aXml.attributes().value(SMILE_GENIE_SIZE).toString();
    if(s.isEmpty() && this->strictMode){
        QString err = "genie font size is required";
        this->log.e("parseGenieFont", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGenieFont", "font size: " + s);
    bool ok;
    int size = s.toInt(&ok);
    if(!ok){
        if(this->strictMode){
            QString err = "genie font size must be a valid integer";
            this->log.e("parseGenieFont", err);
            aXml.raiseError(err);
            return;
        }
        else{
            size = DEFAULT_FONT_SIZE;
            this->log.d("parseGenieFont", "font size: (default) " + size);
        }
    }

    *aFont = new QNFont(name, size, color);

    //bold - optional default false
    QString b = aXml.attributes().value(SMILE_GENIE_BOLD).toString();
    if(!b.isEmpty()){
        this->log.d("parseGenieFont", "font bold: " + (b == "true") ? "true" : "false");
        (*aFont)->setBold((b == "true") ? true : false);
    }

    //italic - optional default false
    QString i = aXml.attributes().value(SMILE_GENIE_ITALIC).toString();
    if(!i.isEmpty()){
        this->log.d("parseGenieFont", "font italic: " + (i == "true") ? "true" : "false");
        (*aFont)->setItalic((i == "true") ? true : false);
    }

    //align - optional default left
    QString a = aXml.attributes().value(SMILE_GENIE_ALIGN).toString();
    if(!a.isEmpty()){
        if(a == "center"){
            this->log.d("parseGenieFont", "font align: center");
            (*aFont)->setAlign(QNAlignment::centerAlign);
        }
        else if(a == "right"){
            this->log.d("parseGenieFont", "font align: right");
            (*aFont)->setAlign(QNAlignment::rightAlign);
        }
        else if(a == "left"){
            this->log.d("parseGenieFont", "font align: left");
            (*aFont)->setAlign(QNAlignment::leftAlign);
        }
        else{
            if(this->strictMode){
                QString err = "genie font align is not valid";
                this->log.e("parseGenieFont", err);
                aXml.raiseError(err);
                delete (*aFont);
                *aFont = nullptr;
                return;
            }
            else{
                this->log.d("parseGenieFont", "font align: (default) left");
                (*aFont)->setAlign(QNAlignment::leftAlign);
            }
        }
    }
}

void SmileXmlParser::parseGeniePosition(QXmlStreamReader& aXml, QNPositionPtr* aPos){
    if(*aPos != nullptr)
        delete (*aPos);
    *aPos = nullptr;

    QString pos = aXml.readElementText();
    QStringList l = pos.split(" ");
    if(l.size() < 4){
        QString err = QString("%1 must have 4 elements in the list.").arg(SMILE_GENIE_SUBMODEL_WIN);
        this->log.e("parseGeniePosition", err);
        aXml.raiseError(err);
        return;
    }

    bool ok;
    int left = l.at(0).toInt(&ok);
    if(!ok){
        QString err = QString("%1 must be a valid integer.").arg("left");
        this->log.e("parseGeniePosition", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGeniePosition", "left: " + l.at(0));

    int top = l.at(1).toInt(&ok);
    if(!ok){
        QString err = QString("%1 must be a valid integer.").arg("top");
        this->log.e("parseGeniePosition", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGeniePosition", "top: " + l.at(1));

    int right = l.at(2).toInt(&ok);
    if(!ok){
        QString err = QString("%1 must be a valid integer.").arg("right");
        this->log.e("parseGeniePosition", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGeniePosition", "right: " + l.at(2));

    int bottom = l.at(3).toInt(&ok);
    if(!ok){
        QString err = QString("%1 must be a valid integer.").arg("bottom");
        this->log.e("parseGeniePosition", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGeniePosition", "bottom: " + l.at(3));

    *aPos = new QNPosition(left, top, right, bottom);
}

void SmileXmlParser::parseGenieIcon(QXmlStreamReader& aXml, QNIconPtr aIcon){
    //icon id - required
    QString id = aXml.attributes().value(SMILE_ID_TAG).toString();
    if(id.isEmpty()){
        QString err = "genie icon's id is required";
        this->log.e("parseGenieIcon", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseGenieIcon", "icon id: " + id);
    if(aIcon->setId(id)){
        QString msg = "ID: " + id + " is duplicated.";
        this->log.e("parseGenieIcon", msg);
        aXml.raiseError(msg);
        return;
    }

    //show content - optional
    QString toShowString = aXml.attributes().value(SHOW_TAG).toString();
    bool toShow = (toShowString.isEmpty() || toShowString  == "true");
    aIcon->setShowContent(toShow);
    if(toShow)
        this->log.d("parseGenieIcon", "icon is showing content");
    else
        this->log.d("parseGenieIcon", "icon is hiding content");

    aXml.readNext();

    //name - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieIcon", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_NAME){
                QString name = aXml.readElementText();
                this->log.d("parseGenieIcon", "icon name: " + name);
                aIcon->setName(name);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("expected %1, got %2").arg(SMILE_GENIE_NAME).arg(aXml.name().toString());
                this->log.e("parseGenieIcon", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseGenieIcon", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //interior - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieIcon", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_INTERIOR){
                //interior color - required
                QString color = aXml.attributes().value(SMILE_GENIE_COLOR).toString();
                QRegExp colorRegExp(COLOR_REGEXP);
                if(color.isEmpty() || !colorRegExp.exactMatch(color)){
                    if(this->strictMode){
                        QString err = "genie icon's interior color is not valid";
                        this->log.e("parseGenieIcon", err);
                        aXml.raiseError(err);
                        return;
                    }
                    else{
                        color = DEFAULT_BG_COLOR;
                    }
                }
                this->log.d("parseGenieIcon", "icon interior color: " + color);
                aIcon->setColor(color);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_INTERIOR).arg(aXml.name().toString());
                this->log.e("parseGenieIcon", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseGenieIcon", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //outline - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieIcon", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_OUTLINE){
                //outline color - required
                QString color = aXml.attributes().value(SMILE_GENIE_COLOR).toString();
                QRegExp colorRegExp(COLOR_REGEXP);
                if(color.isEmpty() || !colorRegExp.exactMatch(color)){
                    if(this->strictMode){
                        QString err = "genie icon's outline color is not valid";
                        this->log.e("parseGenieIcon", err);
                        aXml.raiseError(err);
                        return;
                    }
                    else{
                        color = DEFAULT_FG_COLOR;
                    }
                }
                this->log.d("parseGenieIcon", "icon outline color: " + color);
                aIcon->setOutlineColor(color);

                //outline width - optional
                QString width = aXml.attributes().value(SMILE_GENIE_WIDTH).toString();
                if(!width.isEmpty()){
                    bool ok;
                    int w = width.toInt(&ok);
                    if(!ok){
                        if(this->strictMode){
                            QString err = "genie icon's outline width is not a valid integer";
                            this->log.e("parseGenieIcon", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            w = DEFAULT_WIDTH;
                        }
                    }
                    this->log.d("parseGenieIcon", "icon outline width: " + width);
                    aIcon->setOutlineWidth(w);
                }

                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_OUTLINE).arg(aXml.name().toString());
                this->log.e("parseGenieIcon", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseGenieIcon", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //font - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieIcon", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_FONT){
                QNFontPtr font = nullptr;
                this->parseGenieFont(aXml, &font);
                if(font == nullptr)
                    return; //there is an error in the font

                aIcon->setFont(font);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_FONT).arg(aXml.name().toString());
                this->log.e("parseGenieIcon", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseGenieIcon", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //position - required
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieIcon", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_POS){
                QNPositionPtr pos = nullptr;
                this->parseGeniePosition(aXml, &pos);
                if(pos == nullptr)
                    return; //there is an error in the pos

                aIcon->setPosition(pos);
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_GENIE_POS).arg(aXml.name().toString());
                this->log.e("parseGenieIcon", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieIcon", QString("%1 tag end").arg(aXml.name().toString()));
            }
            //skip current token
            aXml.readNext();
        }
    }

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieIcon", QString("%1 tag start").arg(aXml.name().toString()));
            //comment - optional
            if(aXml.name() == SMILE_GENIE_COMMENT){
                QString comment = aXml.readElementText();
                if(!comment.isEmpty()){
                    this->log.d("parseGenieIcon", "icon comment: " + comment);
                    aIcon->setComment(comment);
                }
                aXml.readNext();
            }
            //classes - optional
            else if(aXml.name() == CLASSES_TAG){
                QString classes_str = aXml.readElementText();
                this->log.d("parseGenieIcon", "classes: " + classes_str);
                QStringList l = classes_str.split(", ");
                foreach (QString c, l) {
                    aIcon->addClass(c);
                }
                aXml.readNext();
            }
            //label - optional
            else if(aXml.name() == LABEL_TAG){
                int len = aXml.attributes().value(LABEL_LEN_TAG).toInt();
                QString prop = aXml.attributes().value(LABEL_PROP_TAG).toString();
                this->log.d("parseGenieIcon", "label found: " + prop + ", " + len);
                aIcon->setLabelLen(len);
                aIcon->updatePropToShow(prop);

                aXml.readNext();
            }
            else
                break;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieIcon", QString("%1 tag end").arg(aXml.name().toString()));

                if((aXml.name() == SMILE_GENIE_SUBMODEL) || (aXml.name() == SMILE_GENIE_NODE))
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieModel(QXmlStreamReader& aXml, QNNetworkModelPtr aNetModel, QNModelPtr aModel){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieModel", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_NODE){
                //genie node - optional
                QNNodeIconPtr node = new QNNodeIcon(aModel);

                //faultnameformat - optional default inherit
                QString fault = aXml.attributes().value(SMILE_GENIE_FAULTNAMEFORMAT).toString();
                if(!fault.isEmpty()){
                    QVariant qv;
                    if(fault == "node")
                        qv.setValue(GenieFNF::node);
                    else if(fault == "nodestate")
                        qv.setValue(GenieFNF::nodestate);
                    else if(fault == "user")
                        qv.setValue(GenieFNF::user);
                    else if(fault == "inherit")
                        qv.setValue(GenieFNF::inherit);
                    else{
                        if(this->strictMode){
                            QString err = QString("Genie node invalid faultnameformat value: %1").arg(fault);
                            this->log.e("parseGenieModel", err);
                            aXml.raiseError(err);
                            return;
                        }
                        else{
                            qv.setValue(GenieFNF::inherit);
                        }
                    }
                    this->log.d("parseGenieModel", "genie node faultnameformat: " + fault);
                    node->setProperty(SMILE_GENIE_FAULTNAMEFORMAT, qv);
                }

                //parse common properties
                this->parseGenieIcon(aXml, (QNIconPtr)node);
                if(aXml.hasError())
                    return; //error parsing icon

                //check that there is not another node with the same id
                if(aModel->getIconG(node->getId()) != nullptr){
                    if(this->strictMode){
                        QString msg = node->getId() + " id is duplicated.";
                        this->log.e("parseGenieModel", msg);
                        aXml.raiseError(msg);
                        delete node;
                        return;
                    }
                    else{
                        QString msg = node->getId() + " id is duplicated. Skip it";
                        this->log.w("parseGenieModel", msg);
                        delete node;
                        do{
                            aXml.readNext();
                        }
                        while (aXml.isEndElement() && aXml.name() == SMILE_GENIE_NODE);
                        continue;
                    }
                }

                QNNetworkPtr net = aNetModel->getNet();
                QNNodePtr data = net->getNode(node->getId());
                if(data == nullptr){
                    if(this->strictMode){
                        QString err = QString("the %1 node is not defined").arg(node->getId());
                        this->log.e("parseGenieModel", err);
                        aXml.raiseError(err);
                        delete node;
                        return;
                    }
                    else{
                        QString msg = QString("the %1 node is not defined. Skip it").arg(node->getId());
                        this->log.w("parseGenieModel", msg);
                        delete node;
                        do{
                            aXml.readNext();
                        }
                        while (aXml.isEndElement() && aXml.name() == SMILE_GENIE_NODE);
                        continue;
                    }
                }
                node->setData(data);
                data->setIcon(node);

                //parse specific properties
                this->parseGenieNode(aXml, node);

                aModel->addIcon((QNIconPtr)node);
            }
            else if(aXml.name() == SMILE_GENIE_SUBMODEL){
                //genie submodel - optional
                QNModelPtr model = new QNModel(aModel);

                //parse common properties
                this->parseGenieIcon(aXml, model);
                if(aXml.hasError())
                    return; //error parsing icon

//                //check that there is not another model with the same id
//                if(aModel->getIconG(model->getId()) != nullptr){
//                    //                    if(this->strictMode){
//                    QString msg = "model " + model->getId() + " is duplicated.";
//                    this->log.e("parseGenieModel", msg);
//                    aXml.raiseError(msg);
//                    delete model;
//                    return;
//                    //                    }
//                    //                    else{
//                    //                        QString msg = model->getId() + " id is duplicated. Skip it";
//                    //                        this->log.w("parseGenieModel", msg);
//                    //                        delete model;
//                    //                        do{
//                    //                            aXml.readNext();
//                    //                        }
//                    //                        while (aXml.isEndElement() && aXml.name() == SMILE_GENIE_SUBMODEL);
//                    //                        continue;
//                    //                    }
//                }

                //window - optional
                while(!aXml.atEnd()){
                    if(aXml.isStartElement()){
                        this->log.d("parseGenieModel", QString("%1 tag start").arg(aXml.name().toString()));
                        if(aXml.name() == SMILE_GENIE_SUBMODEL_WIN){
                            QNPositionPtr win = nullptr;
                            this->parseGeniePosition(aXml, &win);
                            if(win == nullptr)
                                return; //there is an error in the win

                            model->setWindow(win);
                            aXml.readNext();
                        }
                        break;
                    }
                    else{
                        if(aXml.isEndElement()){
                            this->log.d("parseGenieModel", QString("%1 tag end").arg(aXml.name().toString()));

                            if(aXml.name() == SMILE_GENIE_SUBMODEL)
                                break;
                        }
                        //skip current token
                        aXml.readNext();
                    }
                }
                aModel->addIcon((QNIconPtr)model);

                //parse model content
                this->parseGenieModel(aXml, aNetModel, model);
            }
            else if(aXml.name() == SMILE_GENIE_TEXTBOX){
                //genie textbox - optional
                QNTextBoxPtr textb = new QNTextBox(aModel);

                //parse textbox content
                this->parseGenieTextbox(aXml, textb);
                if(aXml.hasError()){
                    delete textb;
                    return; //error parsing textbox
                }
                aModel->addTextBox(textb);
            }
            else{
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieModel", QString("%1 tag end").arg(aXml.name().toString()));

                if(aXml.name() == SMILE_GENIE_SUBMODEL){
                    aXml.readNext();
                    return; //end recursive call
                }
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieArccomment(QXmlStreamReader& aXml, QNNetworkModelPtr aModel){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieArccomment", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_ARCCOMMENT){
                //parent - required
                QString parent = aXml.attributes().value(SMILE_GENIE_PARENT).toString();
                if(parent.isEmpty()){
                    this->log.e("parseGenieArccomment", "arccomment parent attribute is required");
                    aXml.raiseError("arccomment parent attribute is required");
                    return;
                }
                else{
                    this->log.d("parseGenieArccomment", "arccomment parent: " + parent);
                }

                //child - required
                QString child = aXml.attributes().value(SMILE_GENIE_CHILD).toString();
                if(child.isEmpty()){
                    this->log.e("parseGenieArccomment", "arccomment child attribute is required");
                    aXml.raiseError("arccomment child attribute is required");
                    return;
                }
                else{
                    this->log.d("parseGenieArccomment", "arccomment child: " + child);
                }

                //get the edge reference
                QString id = QString("%1 - %2").arg(parent).arg(child);
                QNEdgePtr edge = aModel->getNet()->getEdge(id);
                if(edge == nullptr){
                    if(this->strictMode){
                        QString err = QString("there is no edge between %1 and %2").arg(parent).arg(child);
                        this->log.e("parseGenieArccomment", err);
                        aXml.raiseError(err);
                        return;
                    }
                    else{
                        QString msg = QString("there is no edge between %1 and %2. Skip it").arg(parent).arg(child);
                        this->log.w("parseGenieArccomment", msg);
                        aXml.readNext();
                        continue;
                    }
                }
                QNEdgeStylePtr style = new QNEdgeStyle(edge);
                edge->setIcon(style);

                //cost - optional
                QString cost = aXml.attributes().value(SMILE_GENIE_COST).toString();
                if(!cost.isEmpty()){
                    this->log.d("parseGenieArccomment", "arccomment cost: " + cost);
                    style->setProperty(SMILE_GENIE_COST, cost);
                }

                //classes - optional
                QString classes_str = aXml.attributes().value(CLASSES_TAG).toString();
                if(!classes_str.isEmpty()){
                    this->log.d("parseGenieArccomment", "classes: " + classes_str);
                    QStringList l = classes_str.split(", ");
                    foreach (QString c, l) {
                        style->addClass(c);
                    }
                }

                //comment - mixed element
                QString comment = aXml.readElementText();
                if(!comment.isEmpty()){
                    style->setComment(comment);
                    this->log.d("parseGenieArccomment", "arccomment comment: " + comment);
                }

                //add edge to the model
                aModel->addEdge(style);

                aXml.readNext();
            }
            else{
                //edge comments end
                break;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieArccomment", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_GENIE)
                    return;
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieDiagautoformat(QXmlStreamReader& aXml, SmileGenieExt& aGenie){
    GenieDAFPtr daf = new GenieDAF();

    //diagautoformat target - required
    QString target = aXml.attributes().value(SMILE_GENIE_DAF_TARGET).toString();
    if(target.isEmpty()){
        this->log.e("parseGenieDiagautoformat", "genie diagautoformat target is required");
        aXml.raiseError("genie diagautoformat target is required");
        return;
    }
    else{
        this->log.d("parseGenieDiagautoformat", "genie diagautoformat target: " + target);
        daf->setTarget(target);
    }

    //diagautoformat targetnr - required
    QString targetnr = aXml.attributes().value(SMILE_GENIE_DAF_TARGET).toString();
    if(targetnr.isEmpty()){
        this->log.e("parseGenieDiagautoformat", "genie diagautoformat targetnr is required");
        aXml.raiseError("genie diagautoformat targetnr is required");
        return;
    }
    else{
        this->log.d("parseGenieDiagautoformat", "genie diagautoformat targetnr: " + targetnr);
        daf->setTargetnr(targetnr);
    }

    //diagautoformat observation - required
    QString observation = aXml.attributes().value(SMILE_GENIE_DAF_OBSERVATION).toString();
    if(observation.isEmpty()){
        this->log.e("parseGenieDiagautoformat", "genie diagautoformat observation is required");
        aXml.raiseError("genie diagautoformat observation is required");
        return;
    }
    else{
        this->log.d("parseGenieDiagautoformat", "genie diagautoformat observation: " + observation);
        daf->setObservation(observation);
    }

    //diagautoformat observationnr - required
    QString observationnr = aXml.attributes().value(SMILE_GENIE_DAF_OBSERVATION).toString();
    if(observationnr.isEmpty()){
        this->log.e("parseGenieDiagautoformat", "genie diagautoformat observationnr is required");
        aXml.raiseError("genie diagautoformat observationnr is required");
        return;
    }
    else{
        this->log.d("parseGenieDiagautoformat", "genie diagautoformat observationnr: " + observationnr);
        daf->setObservationnr(observationnr);
    }

    //diagautoformat auxiliary - required
    QString auxiliary = aXml.attributes().value(SMILE_GENIE_DAF_AUXILIARY).toString();
    if(auxiliary.isEmpty()){
        this->log.e("parseGenieDiagautoformat", "genie diagautoformat auxiliary is required");
        aXml.raiseError("genie diagautoformat auxiliary is required");
        return;
    }
    else{
        this->log.d("parseGenieDiagautoformat", "genie diagautoformat auxiliary: " + auxiliary);
        daf->setAuxiliary(auxiliary);
    }

    aGenie.setDiagautoformat(daf);
}

void SmileXmlParser::parseGenieChildren(QXmlStreamReader& aXml, SmileGenieExt& aGenie, QNNetworkModelPtr aModel){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseGenieChildren", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE_COMMENT){
                //genie comment - optional
                QString comment = aXml.readElementText();
                this->log.d("parseGenieChildren", "genie comment: " + comment);
                aGenie.setComment(comment);
                aXml.readNext();
            }
            else if(aXml.name() == SMILE_GENIE_DIAGAUTOFORMAT){
                //genie diagautoformat - optional
                this->parseGenieDiagautoformat(aXml, aGenie);
                aXml.readNext();
            }
            else if((aXml.name() == SMILE_GENIE_NODE) ||
                    (aXml.name() == SMILE_GENIE_SUBMODEL) ||
                    (aXml.name() == SMILE_GENIE_TEXTBOX)){

                this->parseGenieModel(aXml, aModel, aModel);
            }
            else if(aXml.name() == SMILE_GENIE_ARCCOMMENT){
                //genie arccomment - list
                this->parseGenieArccomment(aXml, aModel);
            }
            else{
                QString err = QString("Expected %1 or %2 or %3 or %4 or %5 or %6, got %7.").arg(SMILE_GENIE_COMMENT).arg(SMILE_GENIE_DIAGAUTOFORMAT)
                              .arg(SMILE_GENIE_NODE).arg(SMILE_GENIE_SUBMODEL).arg(SMILE_GENIE_TEXTBOX).arg(SMILE_GENIE_ARCCOMMENT).arg(aXml.name().toString());
                this->log.e("parseSmileExtensions", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseGenieChildren", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_GENIE)
                    return;
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseGenieAttributes(QXmlStreamReader& aXml, SmileGenieExt& aGenie){
    //genie version - required
    QString version = aXml.attributes().value(SMILE_GENIE_VERSION).toString();
    if(version.isEmpty()){
        this->log.e("parseGenieAttributes", "genie version is required");
        aXml.raiseError("genie version is required");
        return;
    }
    else{
        this->log.d("parseGenieAttributes", "genie version: " + version);
        aGenie.setVersion(version);
    }

    //genie name - required
    QString name = aXml.attributes().value(SMILE_GENIE_NAME).toString();
    if(name.isEmpty()){
        this->log.e("parseGenieAttributes", "genie name is required");
        aXml.raiseError("genie name is required");
        return;
    }
    else{
        this->log.d("parseGenieAttributes", "genie name: " + name);
        aGenie.setName(name);
    }

    //genie faultnameformat - optional
    QString fault = aXml.attributes().value(SMILE_GENIE_FAULTNAMEFORMAT).toString();
    if(!fault.isEmpty()){
        if(fault == "node")
            aGenie.setFaultnameformat(GenieFNF::node);
        else if(fault == "nodestate")
            aGenie.setFaultnameformat(GenieFNF::nodestate);
        else if(fault == "inherit")
            aGenie.setFaultnameformat(GenieFNF::inherit);
        else if(fault == "user")
            aGenie.setFaultnameformat(GenieFNF::user);
        else{
            if(this->strictMode){
                QString err = QString("Genie properties invalid faultnameformat value: %1").arg(fault);
                this->log.e("parseGenieAttributes", err);
                aXml.raiseError(err);
                return;
            }
            else{
                aGenie.setFaultnameformat(GenieFNF::user);
            }
        }
        this->log.d("parseGenieAttributes", "genie faultnameformat: " + fault);
    }
}

void SmileXmlParser::parseSmileExtensions(QXmlStreamReader& aXml, QNNetworkPtr aNet){
    QNNetworkModelPtr model = new QNNetworkModel(aNet);
    model->setId(aNet->getId());
    aNet->setNetModel(model);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileExtensions", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_GENIE){
                //parse genie element
                SmileGenieExt genie;
                this->parseGenieAttributes(aXml, genie);
                model->setVersion(genie.getVersion());
                aXml.readNext();
                this->parseGenieChildren(aXml, genie, model);

                //set genie to the model
                QVariant qv; qv.setValue(genie);
                model->setProperty(SMILE_GENIE, qv);
            }
            else{
                QString err = QString("Expected %1 got %2.").arg(SMILE_GENIE).arg(aXml.name().toString());
                this->log.e("parseSmileExtensions", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseSmileExtensions", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_EXTENSIONS)
                    return;
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseNodeProperties(QXmlStreamReader &aXml, QNNodePtr aNode){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseNodeProperties", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_PROP){
                QString id = aXml.attributes().value(SMILE_ID_TAG).toString();
                if(id.isEmpty()){
                    QString err = "SmileNode property has empty id";
                    this->log.e("parseNodeProperties", err);
                    aXml.raiseError(err);
                    return;
                }
                QString value = aXml.readElementText();
                aNode->setProperty(id, value);

                //get next token
                aXml.readNext();
            }
            else{
                //no more properties to read but the current token is a start element
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseNodeProperties", QString("%1 tag end").arg(aXml.name().toString()));
                if((aXml.name() == SMILE_NODES_CPT) || (aXml.name() == SMILE_NODES_NOISYMAX) || (aXml.name() == SMILE_NODES_NOISYADDER)
                   || (aXml.name() == SMILE_NODES_DETERMINISTIC) || (aXml.name() == SMILE_NODES_DECISION)
                   || (aXml.name() == SMILE_NODES_UTILITY) || (aXml.name() == SMILE_NODES_MAU))
                    return;
            }

            //skip token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseNodeParents(QXmlStreamReader &aXml, QNNodePtr aNode){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseNodeParents", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_PARENTS){
                QString parents = aXml.readElementText();
                if(!parents.isEmpty()){
                    QNNetworkPtr net = aNode->getNet();
                    if(net == nullptr){
                        QString err = QString("this node has no container network");
                        this->log.e("parseNodeParents", err);
                        aXml.raiseError(err);
                        return;
                    }

                    //split parents string into tokens
                    QStringList l = parents.split(" ");
                    foreach (QString s, l) {
                        QString id = QString("%1 - %2").arg(s).arg(aNode->getId());
                        //add new edge to net
                        //correctness checks after all nodes have been parsed
                        QNEdgePtr tmp = new QNEdge(id, net);
                        net->addEdge(tmp);
                    }
                    //go to next token
                    aXml.readNext();
                }
            }
            return;
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseNodeParents", QString("%1 tag end").arg(aXml.name().toString()));

                if((aXml.name() == SMILE_NODES_CPT) || (aXml.name() == SMILE_NODES_NOISYMAX) || (aXml.name() == SMILE_NODES_NOISYADDER)
                   || (aXml.name() == SMILE_NODES_DETERMINISTIC) || (aXml.name() == SMILE_NODES_DECISION)
                   || (aXml.name() == SMILE_NODES_UTILITY) || (aXml.name() == SMILE_NODES_MAU) || (aXml.name() == SMILE_NODES)){

                    return;
                }
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseNodeStates(QXmlStreamReader& aXml, QNNodePtr aNode){
    int num_state = 0;
    SmileNodeStateList list;

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseNodeStates", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_STATE){
                num_state ++;
                SmileNodeState state;

                //state id - required
                QString id = aXml.attributes().value(SMILE_NODE_STATE_ID).toString();
                if(id.isEmpty()){
                    QString err = "Smile node state has empty id";
                    this->log.e("parseNodeStates", err);
                    aXml.raiseError(err);
                    return;
                }
                this->log.d("parseNodeStates", "id: " + id);
                state.setId(id);

                //state label - optional
                QString label = aXml.attributes().value(SMILE_NODE_STATE_LABEL).toString();
                if(label != ""){
                    this->log.d("parseNodeStates", "label: " + label);
                    state.setLabel(label);
                }

                //state target - optional
                QString target = aXml.attributes().value(SMILE_NODE_STATE_TARGET).toString();
                if(target != ""){
                    this->log.d("parseNodeStates", "target: " + target);
                    bool t = (target == "true") ? true : false;
                    state.setTarget(t);
                }

                //state default - optional
                QString defaul = aXml.attributes().value(SMILE_NODE_STATE_DEFAULT).toString();
                if(defaul != ""){
                    this->log.d("parseNodeStates", "default: " + defaul);
                    bool d = (defaul == "true") ? true : false;
                    state.setDefault(d);
                }

                //state fault - optional
                QString fault = aXml.attributes().value(SMILE_NODE_STATE_FAULT).toString();
                if(fault != ""){
                    this->log.d("parseNodeStates", "fault: " + fault);
                    bool f = (fault == "true") ? true : false;
                    state.setFault(f);
                }

                //add this state to the list
                list.push_back(state);

                aXml.readNext();
            }
            else{
                if(num_state < MIN_STATE_NUM){
                    QString err = QString("Smile node must contain at least %1 states").arg(MIN_STATE_NUM);
                    this->log.e("parseNodeStates", err);
                    aXml.raiseError(err);
                }
                else{
                    //add states to this node
                    if(!list.isEmpty()){
                        QVariant qv; qv.setValue(list);
                        aNode->setProperty(SMILE_NODE_STATE_LIST, qv);
                    }
                }
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseNodeStates", QString("%1 tag end").arg(aXml.name().toString()));

                if((aXml.name() == SMILE_NODES_CPT) || (aXml.name() == SMILE_NODES_NOISYMAX) || (aXml.name() == SMILE_NODES_NOISYADDER)
                   || (aXml.name() == SMILE_NODES_DETERMINISTIC) || (aXml.name() == SMILE_NODES_DECISION)
                   || (aXml.name() == SMILE_NODES_UTILITY) || (aXml.name() == SMILE_NODES_MAU) || (aXml.name() == SMILE_NODES)){

                    if(num_state < MIN_STATE_NUM){
                        QString err = QString("Smile node must contain at least %1 states").arg(MIN_STATE_NUM);
                        this->log.e("parseNodeStates", err);
                        aXml.raiseError(err);
                    }
                    else{
                        //add states to this node
                        if(!list.isEmpty()){
                            QVariant qv; qv.setValue(list);
                            aNode->setProperty(SMILE_NODE_STATE_LIST, qv);
                        }
                    }

                    return;
                }
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseNodeAttributes(QXmlStreamReader& aXml, QNNodePtr aNode){
    //node id - required
    QString id = aXml.attributes().value(SMILE_ID_TAG).toString();
    if(id.isEmpty()){
        QString err = "node attributes with empty id";
        this->log.e("parseNodeAttributes", err);
        aXml.raiseError(err);
        return;
    }
    this->log.d("parseNodeAttributes", "id: " + id);
    aNode->setId(id);

    //node target - optional
    QString target = aXml.attributes().value(SMILE_NODES_TARGET).toString();
    if(target != ""){
        this->log.d("parseNodeAttributes", "target: " + target);
        bool t = (target == "true") ? true : false;
        aNode->setProperty(SMILE_NODES_TARGET, t);
    }

    //node ranked - optional default false
    QString ranked = aXml.attributes().value(SMILE_NODES_RANKED).toString();
    if(ranked != ""){
        this->log.d("parseNodeAttributes", "ranked: " + ranked);
        bool r = (ranked == "true") ? true : false;
        aNode->setProperty(SMILE_NODES_RANKED, r);
    }

    //node mandatory - optional default false
    QString mandatory = aXml.attributes().value(SMILE_NODES_MANDATORY).toString();
    if(mandatory != ""){
        this->log.d("parseNodeAttributes", "mandatory: " + mandatory);
        bool m = (mandatory == "true") ? true : false;
        aNode->setProperty(SMILE_NODES_MANDATORY, m);
    }

    //node diagtype - optional default auxiliary
    QString diagType = aXml.attributes().value(SMILE_NODES_DIAGTYPE).toString();
    if(diagType != ""){
        if(diagType == "auxiliary")
            aNode->setProperty(SMILE_NODES_DIAGTYPE, DiagType::auxiliary);
        else if(diagType == "observation")
            aNode->setProperty(SMILE_NODES_DIAGTYPE, DiagType::observation);
        else if(diagType == "target")
            aNode->setProperty(SMILE_NODES_DIAGTYPE, DiagType::target);
        else{
            if(this->strictMode){
                QString err = QString("SmileNode properties invalid diagType value: %1").arg(diagType);
                this->log.e("parseNodeAttributes", err);
                aXml.raiseError(err);
                return;
            }
            else{
                aNode->setProperty(SMILE_NODES_DIAGTYPE, DiagType::auxiliary);
            }
        }
        this->log.d("parseNodeAttributes", "diagType: " + diagType);
    }
}

void SmileXmlParser::parseSmileNodeCpt(QXmlStreamReader &aXml, QNNodePtr aNode){
    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node states
    this->parseNodeStates(aXml, aNode);

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileCpt", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_PROB){
                QString prob = aXml.readElementText();
                this->log.d("parseSmileCpt", "cpt probabilities: " + prob);

                DoubleList ld;
                if(!prob.isEmpty()){
                    QStringList l = prob.split(" ");
                    bool ok;
                    for(QString s: l){
                        double d = s.toDouble(&ok);
                        if(!ok){
                            QString err = "cpt probabilities does not contain a list of doubles";
                            this->log.e("parseSmileCpt", err);
                            aXml.raiseError(err);
                            return;
                        }
                        ld.push_back(d);
                    }
                }
                QVariant qv; qv.setValue(ld);
                aNode->setProperty(SMILE_NODE_PROB, qv);

                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_NODE_PROB).arg(aXml.name().toString());
                this->log.e("parseSmileCpt", err);
                aXml.raiseError();
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileCpt", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodeNoisyadder(QXmlStreamReader &aXml, QNNodePtr aNode){
    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node states
    this->parseNodeStates(aXml, aNode);

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNoisyadder", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_DSTATES){
                QString dstates = aXml.readElementText();
                this->log.d("parseSmileNoisyadder", "noisyadder dstates: " + dstates);

                IntList li;
                if(!dstates.isEmpty()){
                    QStringList l = dstates.split(" ");
                    bool ok;
                    for(QString s: l){
                        int i = s.toInt(&ok);
                        if(!ok){
                            QString err = "noisyadder dstates does not contain a list of integers";
                            this->log.e("parseSmileNoisyadder", err);
                            aXml.raiseError(err);
                            return;
                        }
                        li.push_back(i);
                    }
                }
                QVariant qv; qv.setValue(li);
                aNode->setProperty(SMILE_NODE_DSTATES, qv);

                //go to next token
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1 got %2.").arg(SMILE_NODE_DSTATES).arg(aXml.name().toString());
                this->log.e("parseSmileNoisyadder", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileNoisyadder", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNoisyadder", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_WEIGHTS){
                QString weights = aXml.readElementText();
                this->log.d("parseSmileNoisyadder", "noisyadder weights: " + weights);

                DoubleList ld;
                if(!weights.isEmpty()){
                    QStringList l = weights.split(" ");
                    bool ok;
                    for(QString s: l){
                        double d = s.toDouble(&ok);
                        if(!ok){
                            QString err = "noisyadder weights does not contain a list of doubles";
                            this->log.e("parseSmileNoisyadder", err);
                            aXml.raiseError(err);
                            return;
                        }
                        ld.push_back(d);
                    }
                }
                QVariant qv; qv.setValue(ld);
                aNode->setProperty(SMILE_NODE_WEIGHTS, qv);

                //go to next token
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1 got %2.").arg(SMILE_NODE_WEIGHTS).arg(aXml.name().toString());
                this->log.e("parseSmileNoisyadder", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileNoisyadder", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNoisyadder", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_PARAMETERS){
                QString params = aXml.readElementText();
                this->log.d("parseSmileNoisyadder", "noisyadder params: " + params);

                DoubleList ld;
                if(!params.isEmpty()){
                    QStringList l = params.split(" ");
                    bool ok;
                    for(QString s: l){
                        double d = s.toDouble(&ok);
                        if(!ok){
                            QString err = "noisyadder params does not contain a list of doubles";
                            this->log.e("parseSmileNoisyadder", err);
                            aXml.raiseError(err);
                            return;
                        }
                        ld.push_back(d);
                    }
                }
                QVariant qv; qv.setValue(ld);
                aNode->setProperty(SMILE_NODE_PARAMETERS, qv);

                //go to next token
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1 got %2.").arg(SMILE_NODE_PARAMETERS).arg(aXml.name().toString());
                this->log.e("parseSmileNoisyadder", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileNoisyadder", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodeNoisymax(QXmlStreamReader &aXml, QNNodePtr aNode){

    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node states
    this->parseNodeStates(aXml, aNode);

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNodeNoisymax", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_STRENGHTS){
                QString strenghts = aXml.readElementText();
                this->log.d("parseSmileNodeNoisymax", "noisymax strenghts: " + strenghts);

                IntList li;
                if(!strenghts.isEmpty()){
                    QStringList l = strenghts.split(" ");
                    bool ok;
                    for(QString s: l){
                        int i = s.toInt(&ok);
                        if(!ok){
                            QString err = "noisymax strenghts does not contain a list of integers";
                            this->log.e("parseSmileNodeNoisymax", err);
                            aXml.raiseError(err);
                            return;
                        }
                        li.push_back(i);
                    }
                }
                QVariant qv; qv.setValue(li);
                aNode->setProperty(SMILE_NODE_STRENGHTS, qv);

                //go to next token
                aXml.readNext();
            }
            break;
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileNodeNoisymax", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNodeNoisymax", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_PARAMETERS){
                QString params = aXml.readElementText();
                this->log.d("parseSmileNodeNoisymax", "noisymax params: " + params);

                DoubleList ld;
                if(!params.isEmpty()){
                    QStringList l = params.split(" ");
                    bool ok;
                    for(QString s: l){
                        double d = s.toDouble(&ok);
                        if(!ok){
                            QString err = "noisymax params does not contain a list of doubles";
                            this->log.e("parseSmileNodeNoisymax", err);
                            aXml.raiseError(err);
                            return;
                        }
                        ld.push_back(d);
                    }
                }
                QVariant qv; qv.setValue(ld);
                aNode->setProperty(SMILE_NODE_PARAMETERS, qv);

                //go to next token
                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1 got %2.").arg(SMILE_NODE_PARAMETERS).arg(aXml.name().toString());
                this->log.e("parseSmileNodeNoisymax", err);
                aXml.raiseError(err);
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileNodeNoisymax", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodeDeterministic(QXmlStreamReader &aXml, QNNodePtr aNode){
    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node states
    this->parseNodeStates(aXml, aNode);

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNodeDeterministic", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_RESULTINGSTATES){
                QString resstates = aXml.readElementText();
                this->log.d("parseSmileNodeDeterministic", "deterministic resultingstates: " + resstates);
                QStringList l = resstates.split(" ");
                QVariant qv; qv.setValue(l);
                aNode->setProperty(SMILE_NODE_RESULTINGSTATES, qv);

                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_NODE_RESULTINGSTATES).arg(aXml.name().toString());
                this->log.e("parseSmileNodeDeterministic", err);
                aXml.raiseError();
                break;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileNodeDeterministic", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodeDecision(QXmlStreamReader& aXml, QNNodePtr aNode){
    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node states
    this->parseNodeStates(aXml, aNode);

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodeUtility(QXmlStreamReader &aXml, QNNodePtr aNode){
    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileUtility", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_UTILITIES){
                QString utils = aXml.readElementText();
                this->log.d("parseSmileUtility", "utility utils: " + utils);

                DoubleList ld;
                if(!utils.isEmpty()){
                    QStringList l = utils.split(" ");
                    bool ok;
                    for(QString s: l){
                        double d = s.toDouble(&ok);
                        if(!ok){
                            QString err = "utility utils does not contain a list of doubles";
                            this->log.e("parseSmileUtility", err);
                            aXml.raiseError(err);
                            return;
                        }
                        ld.push_back(d);
                    }
                }
                QVariant qv; qv.setValue(ld);
                aNode->setProperty(SMILE_NODE_UTILITIES, qv);

                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_NODE_UTILITIES).arg(aXml.name().toString());
                this->log.e("parseSmileUtility", err);
                aXml.raiseError();
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileUtility", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodeMau(QXmlStreamReader &aXml, QNNodePtr aNode){
    //parse node attributes
    this->parseNodeAttributes(aXml, aNode);

    //start parsing sub-elements
    aXml.readNext();

    //parse node parents
    this->parseNodeParents(aXml, aNode);

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileMau", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_NODE_WEIGHTS){
                QString weights = aXml.readElementText();
                this->log.d("parseSmileMau", "mau weights: " + weights);

                DoubleList ld;
                if(!weights.isEmpty()){
                    QStringList l = weights.split(" ");
                    bool ok;
                    for(QString s: l){
                        double d = s.toDouble(&ok);
                        if(!ok){
                            QString err = "mau weights does not contain a list of doubles";
                            this->log.e("parseSmileMau", err);
                            aXml.raiseError(err);
                            return;
                        }
                        ld.push_back(d);
                    }
                }
                QVariant qv; qv.setValue(ld);
                aNode->setProperty(SMILE_NODE_WEIGHTS, qv);

                aXml.readNext();
                break;
            }
            else{
                QString err = QString("Expected %1, got: %2").arg(SMILE_NODE_WEIGHTS).arg(aXml.name().toString());
                this->log.e("parseSmileMau", err);
                aXml.raiseError();
                return;
            }
        }
        else{
            if(aXml.isEndElement())
                this->log.d("parseSmileMau", QString("%1 tag end").arg(aXml.name().toString()));

            //skip current token
            aXml.readNext();
        }
    }

    //parse node properties
    this->parseNodeProperties(aXml, aNode);
}

void SmileXmlParser::parseSmileNodes(QXmlStreamReader &aXml, QNNetworkPtr aNet){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileNodes", QString("%1 tag start").arg(aXml.name().toString()));

            QNNodePtr node = new QNNode(aNet);
            if(aXml.name() == SMILE_NODES_CPT){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_CPT);

                //parse node specific items
                this->parseSmileNodeCpt(aXml, node);
            }
            else if(aXml.name() == SMILE_NODES_NOISYMAX){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_NOISYMAX);

                //parse specific sub-elements
                this->parseSmileNodeNoisymax(aXml, node);
            }
            else if(aXml.name() == SMILE_NODES_NOISYADDER){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_NOISYADDER);

                //parse specific sub-elements
                this->parseSmileNodeNoisyadder(aXml, node);
            }
            else if(aXml.name() == SMILE_NODES_DETERMINISTIC){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_DETERMINISTIC);

                //parse specific sub-elements
                this->parseSmileNodeDeterministic(aXml, node);
            }
            else if(aXml.name() == SMILE_NODES_DECISION){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_DECISION);

                //parse specific sub-elements
                this->parseSmileNodeDecision(aXml, node);
            }
            else if(aXml.name() == SMILE_NODES_UTILITY){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_UTILITY);

                //parse node specific items
                this->parseSmileNodeUtility(aXml, node);
            }
            else if(aXml.name() == SMILE_NODES_MAU){
                node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_MAU);

                //parse node specific items
                this->parseSmileNodeMau(aXml, node);
            }
            else{
                delete node;
                QString msg = "Smile nodes invalid child start: " + aXml.name().toString();
                this->log.e("parseSmileNodes", msg);
                aXml.raiseError(msg);
                return;
            }

            //check that there is not another node with the same id
            if(aNet->getNode(node->getId()) != nullptr){
                if(this->strictMode){
                    QString msg = node->getId() + " id is duplicated.";
                    this->log.e("parseSmileNodes", msg);
                    aXml.raiseError(msg);
                    delete node;
                    return;
                }
                else{
                    QString msg = node->getId() + " id is duplicated. Skip it";
                    this->log.w("parseSmileNodes", msg);
                    delete node;
                    continue;
                }
            }
            aNet->addNode(node);
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseSmileNodes", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_NODES){
                    //check edges correctness
                    this->checkEdges(aNet);
                    return;
                }
            }

            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseSmileObscost(QXmlStreamReader &aXml, QNNetworkPtr aNet){
    SmileObservationcost obscost;

    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileObscost", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_OBSCOST_NODE){
                //parse node's attributes
                QString id = aXml.attributes().value(SMILE_ID_TAG).toString();
                if(id.isEmpty()){
                    this->log.e("parseSmileObscost", "node id must be not empty");
                    aXml.raiseError("node id must be not empty");
                    return;
                }
                this->log.d("parseSmileObscost", "node id: " + id);
                SmileObscostNode node(id);

                //parse node's sub-elements
                aXml.readNext();
                while(!aXml.atEnd()){
                    if(aXml.isStartElement()){
                        this->log.d("parseSmileObscost", QString("%1 tag start").arg(aXml.name().toString()));
                        // parse parents - optional
                        if(aXml.name() == SMILE_OBSCOST_NODE_PARENTS){
                            QString parents = aXml.readElementText();
                            this->log.d("parseSmileObscost", "node parents: " + parents);

                            //split parents string into tokens
                            QStringList l = parents.split(" ");
                            foreach (QString s, l) {
                                node.addParent(s);
                            }
                            //go to next token
                            aXml.readNext();
                        }
                        break;
                    }
                    else{
                        if(aXml.isEndElement())
                            this->log.d("parseSmileObscost", QString("%1 tag end").arg(aXml.name().toString()));

                        //skip current token
                        aXml.readNext();
                    }
                }

                while(!aXml.atEnd()){
                    if(aXml.isStartElement()){
                        this->log.d("parseSmileObscost", QString("%1 tag start").arg(aXml.name().toString()));
                        // parse cost - required
                        if(aXml.name() == SMILE_OBSCOST_NODE_COST){
                            QString cost = aXml.readElementText();
                            this->log.d("parseSmileObscost", "node cost: " + cost);

                            if(!cost.isEmpty()){
                                QStringList l = cost.split(" ");
                                bool ok;
                                for(QString s: l){
                                    double d = s.toDouble(&ok);
                                    if(!ok){
                                        QString err = "cost does not contain a list of doubles";
                                        this->log.e("parseSmileObscost", err);
                                        aXml.raiseError(err);
                                        return;
                                    }
                                    node.addCost(d);
                                }
                            }

                            //go to next token
                            aXml.readNext();
                        }
                        else{
                            QString err = QString("expected %1 got %2").arg(SMILE_OBSCOST_NODE_COST).arg(aXml.name().toString());
                            this->log.e("parseSmileObscost", err);
                            aXml.raiseError(err);
                            return;
                        }
                    }
                    else{
                        if(aXml.isEndElement()){
                            this->log.d("parseSmileObscost", QString("%1 tag end").arg(aXml.name().toString()));
                            if(aXml.name() == SMILE_OBSCOST_NODE){
                                obscost.addNode(node);
                                break; //end of node tag
                            }
                        }
                        //skip current token
                        aXml.readNext();
                    }
                }
                aXml.readNext();
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseSmileObscost", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_OBSCOST){
                    //set observationcost property
                    QVariant qv; qv.setValue(obscost);
                    aNet->setProperty(SMILE_OBSCOST, qv);
                    return;
                }
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseSmileAlgparams(QXmlStreamReader &aXml, QNNetworkPtr aNet){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileAlgparams", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_EPIS){
                //parse epis's attributes
                ScopedSmileEpis epis(new SmileEpis());

                bool ok;
                QString str = aXml.attributes().value(SMILE_EPIS_PROPLEN).toString();
                //proplen - required
                int proplen = str.toInt(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis proplen must be a valid integer");
                    aXml.raiseError("epis proplen must be a valid integer");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis proplen: " + str);
                epis->setProplen(proplen);

                //numstates1 - required
                str = aXml.attributes().value(SMILE_EPIS_NUMSTATES1).toString();
                int numstates1 = str.toInt(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis numstates1 must be a valid integer");
                    aXml.raiseError("epis numstates1 must be a valid integer");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis numstates1: " + str);
                epis->setNumStates1(numstates1);

                //numstates2 - required
                str = aXml.attributes().value(SMILE_EPIS_NUMSTATES2).toString();
                int numstates2 = str.toInt(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis numstates2 must be a valid integer");
                    aXml.raiseError("epis numstates2 must be a valid integer");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis numstates2: " + str);
                epis->setNumStates2(numstates2);

                //numstates3 - required
                str = aXml.attributes().value(SMILE_EPIS_NUMSTATES3).toString();
                int numstates3 = str.toInt(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis numstates3 must be a valid integer");
                    aXml.raiseError("epis numstates3 must be a valid integer");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis numstates3: " + str);
                epis->setNumStates3(numstates3);

                //eps1 - required
                str = aXml.attributes().value(SMILE_EPIS_EPS1).toString();
                double eps1 = str.toDouble(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis eps1 must be a valid double");
                    aXml.raiseError("epis eps1 must be a valid double");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis ep1: " + str);
                epis->setEps1(eps1);

                //eps2 - required
                str = aXml.attributes().value(SMILE_EPIS_EPS2).toString();
                double eps2 = str.toDouble(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis eps2 must be a valid double");
                    aXml.raiseError("epis eps2 must be a valid double");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis eps2: " + str);
                epis->setEps2(eps2);

                //eps3 - required
                str = aXml.attributes().value(SMILE_EPIS_EPS3).toString();
                double eps3 = str.toDouble(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis eps3 must be a valid double");
                    aXml.raiseError("epis eps3 must be a valid double");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis eps3: " + str);
                epis->setEps3(eps3);

                //eps4 - required
                str = aXml.attributes().value(SMILE_EPIS_EPS4).toString();
                double eps4 = str.toDouble(&ok);
                if(!ok){
                    this->log.e("parseSmileAlgparams", "epis eps4 must be a valid double");
                    aXml.raiseError("epis eps4 must be a valid double");
                    epis.reset();
                    return;
                }
                this->log.d("parseSmileAlgparams", "epis eps4: " + str);
                epis->setEps4(eps4);

                //add current epis to algparams
                SmileAlgparams alg;
                alg.setEpis(epis);
                QVariant qv; qv.setValue(alg);
                aNet->setProperty(SMILE_ALGPARAMS, qv);

                aXml.readNext();
            }
            else{
                //empty algparams
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseSmileAlgparams", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_ALGPARAMS)
                    return;
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseSmileProperties(QXmlStreamReader &aXml, QNNetworkPtr aNet){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileProperties", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_PROPERTIES_CHILD){
                QString id = aXml.attributes().value(SMILE_ID_TAG).toString();
                if(id.isEmpty()){
                    this->log.e("parseSmileProperties", "property id must be not empty");
                    aXml.raiseError("property id must be not empty");
                    return;
                }
                QString value = aXml.readElementText();

                this->log.d("parseSmileProperties", "smile property " + id + ": " + value);
                aNet->setProperty(id, value);
                aXml.readNext();
            }
            else{
                //no more properties
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseSmileProperties", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_PROPERTIES)
                    return;
            }
            //skip current token
            aXml.readNext();
        }
    }
}

void SmileXmlParser::parseSmileChildren(QXmlStreamReader &aXml, QNNetworkPtr aNet){
    while(!aXml.atEnd()){
        if(aXml.isStartElement()){
            this->log.d("parseSmileChildren", QString("%1 tag start").arg(aXml.name().toString()));
            if(aXml.name() == SMILE_PROPERTIES){
                //parse properties
                aXml.readNext();
                this->parseSmileProperties(aXml, aNet);
            }
            else if(aXml.name() == SMILE_ALGPARAMS){
                //parse algparams
                aXml.readNext();
                this->parseSmileAlgparams(aXml, aNet);
            }
            else if(aXml.name() == SMILE_NODES){
                //parse nodes
                aXml.readNext();
                this->parseSmileNodes(aXml, aNet);
            }
            else if(aXml.name() == SMILE_OBSCOST){
                //parse observationCosts
                aXml.readNext();
                this->parseSmileObscost(aXml, aNet);
            }
            else if(aXml.name() == SMILE_EXTENSIONS){
                //parse extensions
                aXml.readNext();
                this->parseSmileExtensions(aXml, aNet);
            }
            else{
                this->log.e("parseSmileChildren", "Invalid smile child start: " + aXml.name().toString());
                aXml.raiseError("Invalid smile child start");
                return;
            }
        }
        else{
            if(aXml.isEndElement()){
                this->log.d("parseSmileChildren", QString("%1 tag end").arg(aXml.name().toString()));
                if(aXml.name() == SMILE_TAG)
                    return;
            }

            //skip token
            aXml.readNext();
        }
    }
}

SmileXmlParser::SmileXmlParser(const QString &aSmileDoc, const bool aStrictMode): log("smileXml", "SmileXmlParser") {
    this->smileDoc = aSmileDoc;
    this->strictMode = aStrictMode;
}

QNError SmileXmlParser::parse(ScopedQNNetwork& aNet) {
    QFile file(this->smileDoc);
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
        if(xml.name() == SMILE_TAG){
            //smile id - required
            QString id = xml.attributes().value(SMILE_ID_TAG).toString();
            if(id.isEmpty()){
                xml.raiseError("smile's id must be not null");
            }
            else{
                this->log.d("parse", "smile id: " + id);

                //smile version
                QString version = xml.attributes().value(SMILE_ID_VERSION).toString();
                this->log.d("parse", "smile version: " + version);

                aNet.reset(new QNNetwork(id, version));

                //smile numsamples - optional default 1000
                QString ns = xml.attributes().value(SMILE_ID_NUMSAMPLES).toString();
                if(!ns.isEmpty()){
                    bool ok;
                    int numsamples = ns.toInt(&ok);
                    if(!ok){
                        xml.raiseError("smile's numsamples must be a valid integer");
                    }
                    else{
                        this->log.d("parse", "smile numsamples: " + ns);
                        aNet->setProperty(SMILE_ID_NUMSAMPLES, QVariant(numsamples));
                    }
                }

                if (!xml.hasError()){
                    //parse smile's children
                    xml.readNext();
                    this->parseSmileChildren(xml, aNet.data());
                }
            }
        }
        else{
            xml.raiseError(QString("Root element must be %1").arg(SMILE_TAG));
        }
    }
    file.close();

    if (xml.hasError()){
        QString err = QString("Failed to parse file %1: %2").arg(file.fileName()).arg(xml.errorString());
        this->log.e("parse", err);
        aNet.reset();
        return QNError(NET_READ_ERROR, err);
    }
    else if (file.error() != QFile::NoError){
        QString err = QString("Cannot read file %1: %2").arg(file.fileName()).arg(file.errorString());
        this->log.e("parse", err);
        return QNError(NET_READ_ERROR, err);
    }

    return QNError();
}

QNError SmileXmlParser::validate(const QString& aSmileSchema) {
    QFile schemaFile (aSmileSchema);
    schemaFile.open(QIODevice::ReadOnly);
    QXmlSchema schema;
    schema.load(&schemaFile, QUrl::fromLocalFile(schemaFile.fileName()));

    if (schema.isValid()) {
        QFile file(this->smileDoc);
        file.open(QIODevice::ReadOnly);

        QXmlSchemaValidator validator(schema);
        SmileMessageHandler h(&(this->log));
        validator.setMessageHandler(&h);
        bool valid = validator.validate(&file, QUrl::fromLocalFile(file.fileName()));
        schemaFile.close();
        file.close();

        if (valid)
            return QNError();
        else
            return QNError(NET_READ_ERROR, this->smileDoc + " does not match the schema file; see log for further details.");
    }
    else{
        schemaFile.close();
        QString msg = QString("%1 schema file is not valid.").arg(aSmileSchema);
        this->log.e("validate", msg);
        return QNError(NET_READ_ERROR, msg);
    }
}
