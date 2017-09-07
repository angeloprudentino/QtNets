/**
 ** @file smilexmlwriter.cpp
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

#include <QFile>
#include "smilexmlwriter.h"
#include "smilexmlconst.h"
#include "smilealgparams.h"
#include "smileobservationcost.h"
#include "smilenode.h"
#include "smilenodegenieext.h"
#include "qnnetworkmodel.h"
#include "qnnodeicon.h"
#include "qnmodel.h"
#include "qngraphicconst.h"


QNError SmileXmlWriter::writeGenieTextbox(QXmlStreamWriter& aWriter, const QNTextBoxPtr aTextbox){
    aWriter.writeStartElement(SMILE_GENIE_TEXTBOX);

    //show content - optional
    bool sc = aTextbox->isShowingContent();
    if(sc){
        aWriter.writeAttribute(SHOW_TAG, "true");
        this->log.d("writeGenieTextbox", "textbox is showing content");
    }
    else{
        aWriter.writeAttribute(SHOW_TAG, "false");
        this->log.d("writeGenieTextbox", "textbox is hiding content");
    }

    //caption - required
    aWriter.writeStartElement(SMILE_GENIE_CAPTION);
    QString caption = aTextbox->getCaption();
    this->log.d("writeGenieTextbox", "genie textbox caption: " + caption);
    aWriter.writeCharacters(caption);
    aWriter.writeEndElement();

    //font - required
    QNFontPtr font = aTextbox->getFont();
    if(font == nullptr){
        QString msg = "genie textbox font must be not empty";
        this->log.e("writeGenieTextbox", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    QNError err = this->writeGenieFont(aWriter, font);
    if(err)
        return err;

    //position - required
    QNPositionPtr pos = aTextbox->getPosition();
    if(pos == nullptr){
        QString msg = "genie textbox position must be not empty";
        this->log.e("writeGenieTextbox", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    err = this->writeGeniePosition(aWriter, pos);
    if(err)
        return err;

    //comment - optional
    QString comment = aTextbox->getComment();
    if(!comment.isEmpty()){
        this->log.d("writeGenieTextbox", "genie textbox comment: " + comment);
        aWriter.writeStartElement(SMILE_GENIE_COMMENT);
        aWriter.writeCharacters(comment);
        aWriter.writeEndElement();
    }

    //classes - optional
    QStringListPtr classes = aTextbox->getClasses();
    if(classes != nullptr && classes->size() > 0){
        aWriter.writeStartElement(CLASSES_TAG);
        int size = classes->size();
        QString classes_str = "";
        for(int i=0; i<size; i++){
            classes_str.append(classes->at(i));
            if(i != size - 1)
                classes_str.append(", ");
        }
        aWriter.writeCharacters(classes_str);
        this->log.d("writeGenieTextbox", "genie textbox classes: " + classes_str);
        aWriter.writeEndElement();
    }

    //label - optional
    QString prop = aTextbox->getPropToShow();
    int len = aTextbox->getLabelLen();
    this->log.d("writeGenieTextbox", "genie textbox label: " + prop + ", " + QString::number(len));
    aWriter.writeStartElement(LABEL_TAG);
    aWriter.writeAttribute(LABEL_PROP_TAG, prop);
    aWriter.writeAttribute(LABEL_LEN_TAG, QString::number(len));
    aWriter.writeEndElement();

    aWriter.writeEndElement();

    return QNError();
}

QNError SmileXmlWriter::writeGenieLink(QXmlStreamWriter& aWriter, const GenieLinkList& aLinkList){
    for(GenieLink l: aLinkList){
        aWriter.writeStartElement(SMILE_GENIE_LINK);

        //link title - required
        QString title = l.getTitle();
        if(title.isEmpty()){
            QString msg = "genie node link title must be not empty";
            this->log.e("writeGenieLink", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        this->log.d("writeGenieLink", "genie node link title: " + title);
        aWriter.writeAttribute(SMILE_GENIE_LINK_TITLE, title);

        //link path - required
        QString path = l.getPath();
        if(path.isEmpty()){
            QString msg = "genie node link path must be not empty";
            this->log.e("writeGenieLink", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        this->log.d("writeGenieLink", "genie node link path: " + path);
        aWriter.writeAttribute(SMILE_GENIE_LINK_PATH, path);

        aWriter.writeEndElement();
    }

    return QNError();
}

QNError SmileXmlWriter::writeGenieNodeStates(QXmlStreamWriter& aWriter, const QNNodeIconPtr aNode){
    QVariant qv = aNode->getProperty(SMILE_GENIE_NODE_STATE);
    if(qv.isValid()){
        if(!qv.canConvert<GenieNodeStateList>()){
            QString msg = QString("Genie node state is not of the expected type");
            this->log.e("writeGenieNodeStates", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        GenieNodeStateList list = qv.value<GenieNodeStateList>();
        for(GenieNodeState state: list){
            aWriter.writeStartElement(SMILE_GENIE_NODE_STATE);

            //id - required
            QString id = state.getId();
            if(id.isEmpty()){
                QString msg = QString("Genie node state id must not be empty");
                this->log.e("writeGenieNodeStates", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }
            this->log.d("writeGenieNodeStates", "Genie node state id: " + id);
            aWriter.writeAttribute(SMILE_ID_TAG, id);

            //faultname - optional
            QString fault = state.getFaultname();
            if(!fault.isEmpty()){
                this->log.d("writeGenieNodeStates", "Genie node state faultname: " + fault);
                aWriter.writeAttribute(SMILE_GENIE_NODE_FAULTNAME, fault);
            }

            //fix - optional
            QString fix = state.getFix();
            if(!fix.isEmpty()){
                this->log.d("writeGenieNodeStates", "Genie node state fix: " + fix);
                aWriter.writeStartElement(SMILE_GENIE_FIX);
                aWriter.writeCharacters(fix);
                aWriter.writeEndElement();
            }

            //comment - optional
            QString comment = state.getComment();
            if(!comment.isEmpty()){
                this->log.d("writeGenieNodeStates", "Genie node state comment: " + comment);
                aWriter.writeStartElement(SMILE_GENIE_COMMENT);
                aWriter.writeCharacters(comment);
                aWriter.writeEndElement();
            }

            //link - list
            GenieLinkListPtr list = state.getLinks();
            if(list != nullptr){
                QNError err = this->writeGenieLink(aWriter, *list);
                if(err)
                    return err;
            }

            aWriter.writeEndElement();
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeGeniePosition(QXmlStreamWriter& aWriter, const QNPositionPtr aPos){
    aWriter.writeStartElement(SMILE_GENIE_POS);
    QString content = QString("%1 %2 %3 %4").arg(QString::number(aPos->getLeft())).arg(QString::number(aPos->getTop()))
                      .arg(QString::number(aPos->getRight())).arg(QString::number(aPos->getBottom()));
    this->log.d("writeGeniePosition", "position: " + content);
    aWriter.writeCharacters(content);
    aWriter.writeEndElement();

    return QNError();
}

QNError SmileXmlWriter::writeGenieFont(QXmlStreamWriter& aWriter, const QNFontPtr aFont){
    aWriter.writeStartElement(SMILE_GENIE_FONT);

    //color - required
    QString color = aFont->getFontColor();
    if(color.isEmpty()){
        if(this->strictMode){
            QString msg = "genie font color must be not empty";
            this->log.e("writeGenieFont", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            color = DEFAULT_FONT_COLOR;
        }
    }
    this->log.d("writeGenieIcon", "font color: " + color);
    aWriter.writeAttribute(SMILE_GENIE_COLOR, color);

    //name - required
    QString name = aFont->getFontName();
    if(name.isEmpty()){
        if(this->strictMode){
            QString msg = "genie font name must be not empty";
            this->log.e("writeGenieFont", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            name = DEFAULT_FONT_NAME;
        }
    }
    this->log.d("writeGenieFont", "font name: " + name);
    aWriter.writeAttribute(SMILE_GENIE_NAME, name);

    //size - required
    int size = aFont->getFontSize();
    this->log.d("writeGenieFont", "font size: " + QString::number(size));
    aWriter.writeAttribute(SMILE_GENIE_SIZE, QString::number(size));

    //bold - optional default false
    OptionalBool bold = aFont->isBold();
    if(bold.isValid()){
        QString value = (bold) ? "true" : "false";
        this->log.d("writeGenieFont", "font bold: " + value);
        aWriter.writeAttribute(SMILE_GENIE_BOLD, value);
    }

    //italic - optional default false
    OptionalBool italic = aFont->isItalic();
    if(italic.isValid()){
        QString value = (italic) ? "true" : "false";
        this->log.d("writeGenieFont", "font italic: " + value);
        aWriter.writeAttribute(SMILE_GENIE_ITALIC, value);
    }

    //align - optional default left
    QNAlignment align = aFont->getAlign();
    if(align != QNAlignment::undefAlign){
        if(align == QNAlignment::centerAlign){
            this->log.d("writeGenieFont", "font align: center");
            aWriter.writeAttribute(SMILE_GENIE_ALIGN, "center");
        }
        else if(align == QNAlignment::rightAlign){
            this->log.d("writeGenieFont", "font align: right");
            aWriter.writeAttribute(SMILE_GENIE_ALIGN, "right");
        }
        else if(align == QNAlignment::leftAlign){
            this->log.d("writeGenieFont", "font align: left");
            aWriter.writeAttribute(SMILE_GENIE_ALIGN, "left");
        }
        else{
            if(this->strictMode){
                QString msg = "genie font align is not valid";
                this->log.e("writeGenieFont", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }
            else{
                this->log.d("parseGenieFont", "font align: (default) left");
                aWriter.writeAttribute(SMILE_GENIE_ALIGN, "left");
            }
        }
    }

    aWriter.writeEndElement();

    return QNError();
}

QNError SmileXmlWriter::writeGenieIcon(QXmlStreamWriter& aWriter, const QNIconPtr aIcon){
    //id - required
    QString id = aIcon->getId();
    if(id.isEmpty()){
        QString msg = "genie icon id must be not empty";
        this->log.e("writeGenieIcon", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("writeGenieIcon", "icon id: " + id);
    aWriter.writeAttribute(SMILE_ID_TAG, id);

    //show content - optional
    bool sc = aIcon->isShowingContent();
    if(sc){
        aWriter.writeAttribute(SHOW_TAG, "true");
        this->log.d("writeGenieIcon", "icon is showing content");
    }
    else{
        aWriter.writeAttribute(SHOW_TAG, "false");
        this->log.d("writeGenieIcon", "icon is hiding content");
    }

    //name - required
    QString name = aIcon->getName();
    if(name.isEmpty()){
        QString msg = "genie icon name must be not empty";
        this->log.e("writeGenieIcon", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("writeGenieIcon", "icon name: " + name);
    aWriter.writeStartElement(SMILE_GENIE_NAME);
    aWriter.writeCharacters(name);
    aWriter.writeEndElement();

    //interior - required
    aWriter.writeStartElement(SMILE_GENIE_INTERIOR);
    //color
    QString color = aIcon->getColor();
    if(color.isEmpty()){
        if(this->strictMode){
            QString msg = "genie icon color must be not empty";
            this->log.e("writeGenieIcon", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            color = DEFAULT_BG_COLOR;
        }
    }
    this->log.d("writeGenieIcon", "icon color: " + color);
    aWriter.writeAttribute(SMILE_GENIE_COLOR, color);
    aWriter.writeEndElement();

    //outline - required
    aWriter.writeStartElement(SMILE_GENIE_OUTLINE);

    //color
    QString outColor = aIcon->getOutlineColor();
    if(outColor.isEmpty()){
        if(this->strictMode){
            QString msg = "genie icon outline color must be not empty";
            this->log.e("writeGenieIcon", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            outColor = DEFAULT_FG_COLOR;
        }
    }
    this->log.d("writeGenieIcon", "icon outline color: " + outColor);
    aWriter.writeAttribute(SMILE_GENIE_COLOR, outColor);

    //width - optional
    int width = aIcon->getOutlineWidth();
    this->log.d("writeGenieIcon", "icon outline width: " + QString::number(width));
    aWriter.writeAttribute(SMILE_GENIE_WIDTH, QString::number(width));
    aWriter.writeEndElement();

    //font - required
    QNFontPtr font = aIcon->getFont();
    if(font == nullptr){
        QString msg = "genie icon font must be not empty";
        this->log.e("writeGenieIcon", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    QNError err = this->writeGenieFont(aWriter, font);
    if(err)
        return err;

    //position - required
    QNPositionPtr pos = aIcon->getPosition();
    if(pos == nullptr){
        QString msg = "genie icon position must be not empty";
        this->log.e("writeGenieIcon", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    err = this->writeGeniePosition(aWriter, pos);
    if(err)
        return err;

    //comment - optional
    QString comment = aIcon->getComment();
    if(!comment.isEmpty()){
        this->log.d("writeGenieIcon", "genie icon comment: " + comment);
        aWriter.writeStartElement(SMILE_GENIE_COMMENT);
        aWriter.writeCharacters(comment);
        aWriter.writeEndElement();
    }

    //classes - optional
    QStringListPtr classes = aIcon->getClasses();
    if(classes != nullptr && classes->size() > 0){
        aWriter.writeStartElement(CLASSES_TAG);
        int size = classes->size();
        QString classes_str = "";
        for(int i=0; i<size; i++){
            classes_str.append(classes->at(i));
            if(i != size - 1)
                classes_str.append(", ");
        }
        aWriter.writeCharacters(classes_str);
        this->log.d("writeGenieIcon", "genie icon classes: " + classes_str);
        aWriter.writeEndElement();
    }

    //label - optional
    QString prop = aIcon->getPropToShow();
    int len = aIcon->getLabelLen();
    this->log.d("writeGenieIcon", "genie icon label: " + prop + ", " + QString::number(len));
    aWriter.writeStartElement(LABEL_TAG);
    aWriter.writeAttribute(LABEL_PROP_TAG, prop);
    aWriter.writeAttribute(LABEL_LEN_TAG, QString::number(len));
    aWriter.writeEndElement();

    return QNError();
}

QNError SmileXmlWriter::writeGenieNode(QXmlStreamWriter& aWriter, const QNNodeIconPtr aNode){
    //state - list
    QNError err = this->writeGenieNodeStates(aWriter, aNode);
    if(err)
        return err;

    //barchart - optional
    QVariant qv = aNode->getProperty(SMILE_GENIE_BARCHART);
    if(qv.isValid()){
        if(!qv.canConvert<GenieBarchart>()){
            QString msg = QString("Genie node barchart is not of the expected type");
            this->log.e("writeGenieNode", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        GenieBarchart bc = qv.value<GenieBarchart>();
        aWriter.writeStartElement(SMILE_GENIE_BARCHART);

        //barchart active - required
        QString active = (bc.isActive()) ? "true" : "false";
        this->log.d("writeGenieNode", "barchart is active: " + active);
        aWriter.writeAttribute(SMILE_GENIE_BARCHART_ACTIVE, active);

        //barchart width - required
        int w = bc.getWidth();
        this->log.d("writeGenieNode", "barchart width: " + QString::number(w));
        aWriter.writeAttribute(SMILE_GENIE_BARCHART_W, QString::number(w));

        //barchart height - required
        int h = bc.getHeight();
        this->log.d("writeGenieNode", "barchart height: " + QString::number(h));
        aWriter.writeAttribute(SMILE_GENIE_BARCHART_H, QString::number(h));

        aWriter.writeEndElement();
    }

    //question - optional
    QString question = aNode->getProperty(SMILE_GENIE_QUESTION).toString();
    if(!question.isEmpty()){
        this->log.d("writeGenieNode", "question: " + question);
        aWriter.writeStartElement(SMILE_GENIE_QUESTION);
        aWriter.writeCharacters(question);
        aWriter.writeEndElement();
    }

    //link - list
    qv = aNode->getProperty(SMILE_GENIE_LINK);
    if(qv.isValid()){
        if(!qv.canConvert<GenieLinkList>()){
            QString msg = QString("Genie node link is not of the expected type");
            this->log.e("writeGenieNode", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        err = this->writeGenieLink(aWriter, qv.value<GenieLinkList>());
        if(err)
            return err;
    }

    //defcomment - list
    qv = aNode->getProperty(SMILE_GENIE_DEFCOMMENT);
    if(qv.isValid()){
        if(!qv.canConvert<GenieTableCommentList>()){
            QString msg = QString("Genie node defcomment is not of the expected type");
            this->log.e("writeGenieNode", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        GenieTableCommentList list = qv.value<GenieTableCommentList>();
        for(GenieTableComment tc: list){
            aWriter.writeStartElement(SMILE_GENIE_DEFCOMMENT);

            //col - required
            int col = tc.getCol();
            this->log.d("writeGenieNode", "defcomment col: " + QString::number(col));
            aWriter.writeAttribute(SMILE_GENIE_COL, QString::number(col));

            //row - required
            int row = tc.getRow();
            this->log.d("writeGenieNode", "defcomment row: " + QString::number(row));
            aWriter.writeAttribute(SMILE_GENIE_ROW, QString::number(row));

            aWriter.writeEndElement();
        }
    }

    //costcomment - list
    qv = aNode->getProperty(SMILE_GENIE_COSTCOMMENT);
    if(qv.isValid()){
        if(!qv.canConvert<GenieTableCommentList>()){
            QString msg = QString("Genie node costcomment is not of the expected type");
            this->log.e("writeGenieNode", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        GenieTableCommentList list = qv.value<GenieTableCommentList>();
        for(GenieTableComment tc: list){
            aWriter.writeStartElement(SMILE_GENIE_COSTCOMMENT);

            //col - required
            int col = tc.getCol();
            this->log.d("writeGenieNode", "costcomment col: " + QString::number(col));
            aWriter.writeAttribute(SMILE_GENIE_COL, QString::number(col));

            //row - required
            int row = tc.getRow();
            this->log.d("writeGenieNode", "costcomment row: " + QString::number(row));
            aWriter.writeAttribute(SMILE_GENIE_ROW, QString::number(row));

            aWriter.writeEndElement();
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeGenieModel(QXmlStreamWriter& aWriter, const QNModelPtr aModel){
    QNError err;
    QStringList keys = aModel->getAllIconsKeys();
    //write all items
    foreach(QString k, keys){
        QNIconPtr icon = aModel->getIcon(k);
        if(dynamic_cast<QNNodeIconPtr>(icon)){
            aWriter.writeStartElement(SMILE_GENIE_NODE);
            QNNodeIconPtr node = dynamic_cast<QNNodeIconPtr>(icon);

            //faultnameformat - optional default inherit
            QVariant qv = node->getProperty(SMILE_GENIE_FAULTNAMEFORMAT);
            if(qv.isValid()){
                if(!qv.canConvert<GenieFNF>()){
                    QString msg = QString("Genie node faultnameformat is not of the expected type");
                    this->log.e("writeGenieModel", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }

                GenieFNF fault = qv.value<GenieFNF>();
                if(fault == GenieFNF::node){
                    this->log.d("writeGenieModel", "genie node faultnameformat: node");
                    aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "node");
                }
                else if(fault == GenieFNF::nodestate){
                    this->log.d("writeGenieModel", "genie node faultnameformat: nodestate");
                    aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "nodestate");
                }
                else if(fault == GenieFNF::user){
                    this->log.d("writeGenieModel", "genie node faultnameformat: user");
                    aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "user");
                }
                else if(fault == GenieFNF::inherit){
                    this->log.d("writeGenieModel", "genie node faultnameformat: inherit");
                    aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "inherit");
                }
                else{
                    if(this->strictMode){
                        QString msg = QString("Genie node invalid faultnameformat value: %1").arg(fault);
                        this->log.e("writeGenieModel", msg);
                        return QNError(NET_WRITE_ERROR, msg);
                    }
                    else{
                        this->log.d("writeGenieModel", "genie node faultnameformat: (default)inherit");
                        aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "inherit");
                    }
                }
            }

            //write common properties
            err = this->writeGenieIcon(aWriter, icon);
            if(err)
                return err;

            //write specific properties
            err = this->writeGenieNode(aWriter, node);
            if(err)
                return err;

            aWriter.writeEndElement();
        }
        else if(dynamic_cast<QNModelPtr>(icon)){
            aWriter.writeStartElement(SMILE_GENIE_SUBMODEL);

            //write common properties
            err = this->writeGenieIcon(aWriter, icon);
            if(err)
                return err;

            //write model window
            QNModelPtr model = dynamic_cast<QNModelPtr>(icon);
            QNPositionPtr win = model->getWindow();
            if(win != nullptr){
                aWriter.writeStartElement(SMILE_GENIE_SUBMODEL_WIN);

                QString content = QString("%1 %2 %3 %4").arg(QString::number(win->getLeft())).arg(QString::number(win->getTop()))
                                  .arg(QString::number(win->getRight())).arg(QString::number(win->getBottom()));
                this->log.d("writeGenieModel", "model window: " + content);
                aWriter.writeCharacters(content);

                aWriter.writeEndElement();
            }

            //write model's content
            err = this->writeGenieModel(aWriter, model);
            if(err)
                return err;

            aWriter.writeEndElement();
        }
        else{
            //ignored icon
            this->log.w("", "icon " + icon->getId() + " ignored because is not a node or a model");
        }
    }

    QNTextBoxListPtr textboxes = aModel->getTextboxes();
    if(textboxes != nullptr){
        //write all textboxes
        int size = textboxes->size();
        for(int i=0; i<size; i++){
            QNError err = this->writeGenieTextbox(aWriter, textboxes->at(i));
            if(err)
                return err;
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeGenieArccomment(QXmlStreamWriter& aWriter, const QNNetworkModelPtr aModel){
    QStringList keys = aModel->getAllEdgesKeys();
    if(!keys.isEmpty()){
        for(QString k: keys){
            QNEdgeStylePtr a = aModel->getEdge(k);
            aWriter.writeStartElement(SMILE_GENIE_ARCCOMMENT);

            //parent - required
            if(a->getData() != nullptr && a->getData()->getSrc() != nullptr){
                QString parent = a->getData()->getSrc()->getId();
                if(!parent.isEmpty()){
                    this->log.d("writeGenieArccomment", "arccomment parent: " + parent);
                    aWriter.writeAttribute(SMILE_GENIE_PARENT, parent);
                }
                else{
                    QString msg = "genie arccomment parent cannot be empty";
                    this->log.e("writeGenieArccomment", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }
            }
            else{
                QString msg = "genie arccomment parent cannot be retrieved";
                this->log.e("writeGenieArccomment", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }

            //child - required
            if(a->getData() != nullptr && a->getData()->getSrc() != nullptr){
                QString child = a->getData()->getDst()->getId();
                if(!child.isEmpty()){
                    this->log.d("writeGenieArccomment", "arccomment child: " + child);
                    aWriter.writeAttribute(SMILE_GENIE_CHILD, child);
                }
                else{
                    QString msg = "genie arccomment child cannot be empty";
                    this->log.e("writeGenieArccomment", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }
            }
            else{
                QString msg = "genie arccomment child cannot be retrieved";
                this->log.e("writeGenieArccomment", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }

            //cost - optional default false
            QString cost = a->getProperty(SMILE_GENIE_COST).toString();
            if(!cost.isEmpty()){
                this->log.d("writeGenieArccomment", "arccomment cost: " + cost);
                aWriter.writeAttribute(SMILE_GENIE_COST, (cost == "true") ? "true" : "false");
            }

            //classes - optional
            QStringListPtr classes = a->getClasses();
            if(classes != nullptr && classes->size() > 0){
                int size = classes->size();
                QString classes_str = "";
                for(int i=0; i<size; i++){
                    classes_str.append(classes->at(i));
                    if(i != size - 1)
                        classes_str.append(", ");
                }
                aWriter.writeAttribute(CLASSES_TAG, classes_str);
            }

            //comment
            QString comment = a->getComment();
            if(!comment.isEmpty()){
                this->log.d("writeGenieArccomment", "arccomment comment: " + comment);
                aWriter.writeCharacters(comment);
            }

            aWriter.writeEndElement();
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeGenieDiagautoformat(QXmlStreamWriter& aWriter, const SmileGenieExt& aGenie){
    GenieDAFPtr daf = aGenie.getDiagautoformat();
    if(daf != nullptr){
        aWriter.writeStartElement(SMILE_GENIE_DIAGAUTOFORMAT);

        //target - required
        QString target = daf->getTarget();
        if(target.isEmpty()){
            QString msg = "diagautoformat target must be not empty";
            this->log.e("writeGenieDiagautoformat", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.d("writeGenieDiagautoformat", "diagautoformat target: " + target);
            aWriter.writeAttribute(SMILE_GENIE_DAF_TARGET, target);
        }

        //targetnr - required
        QString targetnr = daf->getTargetnr();
        if(targetnr.isEmpty()){
            QString msg = "diagautoformat targetnr must be not empty";
            this->log.e("writeGenieDiagautoformat", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.d("writeGenieDiagautoformat", "diagautoformat targetnr: " + targetnr);
            aWriter.writeAttribute(SMILE_GENIE_DAF_TARGETNR, targetnr);
        }

        //observation - required
        QString observation = daf->getObservation();
        if(observation.isEmpty()){
            QString msg = "diagautoformat observation must be not empty";
            this->log.e("writeGenieDiagautoformat", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.d("writeGenieDiagautoformat", "diagautoformat observation: " + observation);
            aWriter.writeAttribute(SMILE_GENIE_DAF_OBSERVATION, observation);
        }

        //observationnr - required
        QString observationnr = daf->getObservationnr();
        if(observationnr.isEmpty()){
            QString msg = "diagautoformat observationnr must be not empty";
            this->log.e("writeGenieDiagautoformat", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.d("writeGenieDiagautoformat", "diagautoformat observationnr: " + observationnr);
            aWriter.writeAttribute(SMILE_GENIE_DAF_OBSERVATIONNR, observationnr);
        }

        //auxiliary - required
        QString auxiliary = daf->getAuxiliary();
        if(auxiliary.isEmpty()){
            QString msg = "diagautoformat auxiliary must be not empty";
            this->log.e("writeGenieDiagautoformat", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            this->log.d("writeGenieDiagautoformat", "diagautoformat auxiliary: " + auxiliary);
            aWriter.writeAttribute(SMILE_GENIE_DAF_AUXILIARY, auxiliary);
        }

        aWriter.writeEndElement();
    }

    return QNError();
}

QNError SmileXmlWriter::writeGenieChildren(QXmlStreamWriter& aWriter, const SmileGenieExt& aGenie, const QNNetworkModelPtr aModel){
    //genie comment - optional
    QString comment = aGenie.getComment();
    if(!comment.isEmpty()){
        this->log.d("writeGenieChildren", "genie's comment: " + comment);
        aWriter.writeStartElement(SMILE_GENIE_COMMENT);
        aWriter.writeCharacters(comment);
        aWriter.writeEndElement();
    }

    //genie diagautoformat - optional
    QNError err = this->writeGenieDiagautoformat(aWriter, aGenie);
    if(err)
        return err;

    //genie models, icons and textboxes
    err = this->writeGenieModel(aWriter, aModel);
    if(err)
        return err;

    //genie archcomment - list
    err = this->writeGenieArccomment(aWriter, aModel);
    if(err)
        return err;

    return QNError();
}

QNError SmileXmlWriter::writeGenieAttributes(QXmlStreamWriter& aWriter, const SmileGenieExt& aGenie){
    //genie version - required
    QString version = aGenie.getVersion();
    if(version.isEmpty()){
        QString msg = "genie version must be not empty";
        this->log.e("writeGenieAttributes", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("writeGenieAttributes", "genie version: " + version);
    aWriter.writeAttribute(SMILE_GENIE_VERSION, version);

    //genie name - required
    QString name = aGenie.getName();
    if(name.isEmpty()){
        QString msg = "genie name must be not empty";
        this->log.e("writeGenieAttributes", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("writeGenieAttributes", "genie name: " + name);
    aWriter.writeAttribute(SMILE_GENIE_NAME, name);

    //genie faultnameformat - optional default user
    GenieFNF fault = aGenie.getFaultnameformat();
    if(fault == GenieFNF::node){
        aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "node");
        this->log.d("writeGenieAttributes", "genie faultnameformat: node");
    }
    else if(fault == GenieFNF::nodestate){
        aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "nodestate");
        this->log.d("writeGenieAttributes", "genie faultnameformat: nodestate");
    }
    else if(fault == GenieFNF::inherit){
        aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "inherit");
        this->log.d("writeGenieAttributes", "genie faultnameformat: inherit");
    }
    else if(fault == GenieFNF::user){
        aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "user");
        this->log.d("writeGenieAttributes", "genie faultnameformat: user");
    }
    else{
        if(this->strictMode){
            QString msg = QString("Genie properties invalid faultnameformat value: %1").arg(fault);
            this->log.e("writeGenieAttributes", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        else{
            aWriter.writeAttribute(SMILE_GENIE_FAULTNAMEFORMAT, "user");
            this->log.d("writeGenieAttributes", "genie faultnameformat: (default)user");
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeSmileExtensions(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet){
    QNNetworkModelPtr model = aNet->getNetModel();
    if(model != nullptr){
        QVariant qv = model->getProperty(SMILE_GENIE);
        if(!qv.isValid() || qv.canConvert<SmileGenieExt>()){
            if(this->strictMode){
                QString msg = "no valid genie attributes found";
                this->log.e("writeSmileExtensions", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }
            else{
                //create a default genie
                SmileGenieExt genie;
                genie.setName(aNet->getId());
                genie.setVersion(aNet->getVersion());
                qv.setValue(genie);
                model->setProperty(SMILE_GENIE, qv);
            }
        }
        //there are extensions
        aWriter.writeStartElement(SMILE_EXTENSIONS);
        aWriter.writeStartElement(SMILE_GENIE);

        //write down genie attributes
        SmileGenieExt genie = qv.value<SmileGenieExt>();
        QNError err = this->writeGenieAttributes(aWriter, genie);
        if(err)
            return err;

        //write down genie children
        err = this->writeGenieChildren(aWriter, genie, model);
        if(err)
            return err;

        aWriter.writeEndElement();
        aWriter.writeEndElement();
    }

    return QNError();
}

QNError SmileXmlWriter::writeNodeProperties(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QStringList keys = aNode->getAllKeys();
    //remove properties not necessary for this format
    keys.removeAll("id");
    keys.removeAll("node_type");
    if(keys.size() > 0){
        for(QString k: keys){
            QVariant qv = aNode->getProperty(k);
            if(qv.canConvert(QMetaType::QString)){
                this->log.d("writeSmileProperties", QString("%1: %2").arg(k).arg(qv.toString()));
                aWriter.writeStartElement(SMILE_PROPERTIES_CHILD);
                aWriter.writeAttribute(SMILE_ID_TAG, k);
                aWriter.writeCharacters(qv.toString());
                aWriter.writeEndElement();
            }
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeNodeParents(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    //write node parents
    QNNodeList* nl = aNode->getAncestors();
    if(nl != nullptr){
        int size = nl->size();
        if(size > 0){
            aWriter.writeStartElement(SMILE_NODE_PARENTS);
            QString parents;
            int i = 0;
            for(QNNodePtr n: *nl){
                if(n == nullptr){
                    QString msg = "one of the ancestor nodes is null";
                    this->log.e("writeNodeParents", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }

                QString id = n->getId();
                if(id.isEmpty()){
                    QString msg = "the id of an ancestor node cannot be empty";
                    this->log.e("writeNodeParents", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }

                parents.append(id);
                if(++i < size)
                    parents.append(" ");
            }
            this->log.d("writeNodeParents", "parents: " + parents);
            aWriter.writeCharacters(parents);
            aWriter.writeEndElement();
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeNodeStates(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    //write node states
    QVariant qv = aNode->getProperty(SMILE_NODE_STATE_LIST);
    if(!qv.isValid() || !qv.canConvert<SmileNodeStateList>()){
        QString msg = "node state element is invalid";
        this->log.e("writeNodeStates", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    SmileNodeStateList nsl = qv.value<SmileNodeStateList>();
    int size = nsl.size();
    if(size < 2){
        QString msg = "this smile node must have at least 2 states";
        this->log.e("writeNodeStates", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    for(int i=0; i<size; i++){
        SmileNodeState n = nsl.at(i);
        aWriter.writeStartElement(SMILE_NODE_STATE);

        //state id - required
        QString id = n.getId();
        if(id.isEmpty()){
            QString msg = "state id is mandatory";
            this->log.e("writeNodeStates", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }
        this->log.d("writeNodeStates", "id: " + id);
        aWriter.writeAttribute(SMILE_ID_TAG, id);

        //state label - optional
        QString label = n.getLabel();
        if(!label.isEmpty()){
            this->log.d("writeNodeStates", "label: " + label);
            aWriter.writeAttribute(SMILE_NODE_STATE_LABEL, label);
        }

        //state target - optional
        OptionalBool target = n.getTarget();
        if(target.isValid()){
            this->log.d("writeNodeStates", "target: " + target? "true": "false");
            aWriter.writeAttribute(SMILE_NODE_STATE_TARGET, target? "true": "false");
        }

        //state default - optional
        OptionalBool def = n.getDefault();
        if(def.isValid()){
            this->log.d("writeNodeStates", "default: " + def? "true": "false");
            aWriter.writeAttribute(SMILE_NODE_STATE_DEFAULT, def? "true": "false");
        }

        //state fault - optional
        OptionalBool fault = n.getFault();
        if(fault.isValid()){
            this->log.d("writeNodeStates", "fault: " + fault? "true": "false");
            aWriter.writeAttribute(SMILE_NODE_STATE_FAULT, fault? "true": "false");
        }

        aWriter.writeEndElement();
    }

    return QNError();
}

QNError SmileXmlWriter::writeNodeAttributes(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    //node id - required
    QString id = aNode->getId();
    if(id.isEmpty()){
        QString msg = "node id cannot be empty";
        this->log.e("writeNodeAttributes", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("writeNodeAttributes", "id: " + id);
    aWriter.writeAttribute(SMILE_ID_TAG, id);

    //target - optional
    QVariant qv = aNode->getProperty(SMILE_NODES_TARGET);
    if(qv.isValid()){
        if(!qv.canConvert<bool>()){
            QString msg = "node target is not of the expected type";
            this->log.e("writeNodeAttributes", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        bool target = qv.value<bool>();
        this->log.d("writeNodeAttributes", "target: " + target);
        aWriter.writeAttribute(SMILE_NODES_TARGET, target? "true" : "false");
    }

    //node ranked - optional default false
    qv = aNode->getProperty(SMILE_NODES_RANKED);
    if(qv.isValid()){
        if(!qv.canConvert<bool>()){
            QString msg = "node ranked is not of the expected type";
            this->log.e("writeNodeAttributes", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        bool ranked = qv.value<bool>();
        this->log.d("writeNodeAttributes", "ranked: " + ranked);
        aWriter.writeAttribute(SMILE_NODES_RANKED, ranked? "true" : "false");
    }

    //node mandatory - optional default false
    qv = aNode->getProperty(SMILE_NODES_MANDATORY);
    if(qv.isValid()){
        if(!qv.canConvert<bool>()){
            QString msg = "node mandatory is not of the expected type";
            this->log.e("writeNodeAttributes", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        bool mandatory = qv.value<bool>();
        this->log.d("writeNodeAttributes", "mandatory: " + mandatory);
        aWriter.writeAttribute(SMILE_NODES_MANDATORY, mandatory? "true" : "false");
    }

    //node diagtype - optional default auxiliary
    qv = aNode->getProperty(SMILE_NODES_DIAGTYPE);
    if(qv.isValid()){
        if(!qv.canConvert<DiagType>()){
            QString msg = "node diagtype is not of the expected type";
            this->log.e("writeNodeAttributes", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        DiagType diagtype = qv.value<DiagType>();
        QString value;
        switch (diagtype) {
            case DiagType::auxiliary:
                value = "auxiliary";
                break;
            case DiagType::target:
                value = "target";
                break;
            case DiagType::observation:
                value = "observation";
                break;
            default:
                if(this->strictMode){
                    QString msg = "node diagtype is invalid: " + diagtype;
                    this->log.e("writeNodeAttributes", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }
                else
                    value = "auxiliary";
                break;
        }

        this->log.d("parseNodeAttributes", "diagType: " + value);
        aWriter.writeAttribute(SMILE_NODES_DIAGTYPE, value);
    }

    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeCpt(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_CPT);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node states
    err = writeNodeStates(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write node probabilities
    QVariant qvProb = aNode->getProperty(SMILE_NODE_PROB);
    if(!qvProb.isValid() || !qvProb.canConvert<DoubleList>()){
        QString msg = "cpt probabilities is a required sub element";
        this->log.e("writeSmileNodeCpt", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_PROB);
    DoubleList list = qvProb.value<DoubleList>();
    QString prob;
    int size = list.size();
    for(int i=0; i<size; i++){
        double d = list.at(i);
        prob.append(QString::number(d, 'g', 10));
        if(i != size-1)
            prob.append(" ");
    }
    this->log.d("writeSmileNodeCpt", "cpt probabilities: " + prob);
    aWriter.writeCharacters(prob);
    aWriter.writeEndElement();

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeNoisyadder(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_NOISYADDER);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node states
    err = writeNodeStates(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write noisyadder proper sub-elements
    QVariant qv = aNode->getProperty(SMILE_NODE_DSTATES);
    if(!qv.isValid() || !qv.canConvert<IntList>()){
        QString msg = QString("%1 sub-element is mandatory for noisyadder nodes").arg(SMILE_NODE_DSTATES);
        this->log.e("writeSmileNodeNoisyadder", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_DSTATES);
    IntList list = qv.value<IntList>();
    QString dstates;
    int size = list.size();
    for(int i=0; i<size; i++){
        int ds = list.at(i);
        dstates.append(QString::number(ds, 'g', 10));
        if(i != size-1)
            dstates.append(" ");
    }
    this->log.d("writeSmileNodeNoisyadder", "noisyadder dstates: " + dstates);
    aWriter.writeCharacters(dstates);
    aWriter.writeEndElement();

    qv = aNode->getProperty(SMILE_NODE_WEIGHTS);
    if(!qv.isValid() || !qv.canConvert<DoubleList>()){
        QString msg = QString("%1 sub-element is mandatory for noisyadder nodes").arg(SMILE_NODE_WEIGHTS);
        this->log.e("writeSmileNodeNoisyadder", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_WEIGHTS);
    DoubleList dlist = qv.value<DoubleList>();
    QString weight;
    size = dlist.size();
    for(int i=0; i<size; i++){
        double d = dlist.at(i);
        weight.append(QString::number(d, 'g', 10));
        if(i != size-1)
            weight.append(" ");
    }
    this->log.d("writeSmileNodeNoisyadder", "noisyadder weight: " + weight);
    aWriter.writeCharacters(weight);
    aWriter.writeEndElement();

    qv = aNode->getProperty(SMILE_NODE_PARAMETERS);
    if(!qv.isValid() || !qv.canConvert<DoubleList>()){
        QString msg = QString("%1 sub-element is mandatory for noisyadder nodes").arg(SMILE_NODE_PARAMETERS);
        this->log.e("writeSmileNodeNoisyadder", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_PARAMETERS);
    dlist = qv.value<DoubleList>();
    QString params;
    size = dlist.size();
    for(int i=0; i<size; i++){
        double d = dlist.at(i);
        params.append(QString::number(d, 'g', 10));
        if(i != size-1)
            params.append(" ");
    }
    this->log.d("writeSmileNodeNoisyadder", "noisyadder parameters: " + params);
    aWriter.writeCharacters(params);
    aWriter.writeEndElement();

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeNoisymax(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_NOISYMAX);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node states
    err = writeNodeStates(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write noisymax proper sub-elements
    QVariant qv = aNode->getProperty(SMILE_NODE_STRENGHTS);
    if(qv.isValid()){
        if(!qv.canConvert<IntList>()){
            QString msg = QString("%1 is not of the expected type").arg(SMILE_NODE_PARAMETERS);
            this->log.e("writeSmileNodeNoisymax", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        aWriter.writeStartElement(SMILE_NODE_STRENGHTS);
        IntList list = qv.value<IntList>();
        QString str;
        int size = list.size();
        for(int i=0; i<size; i++){
            int s = list.at(i);
            str.append(QString::number(s, 'g', 10));
            if(i != size-1)
                str.append(" ");
        }
        this->log.d("writeSmileNodeNoisymax", "noisymax strenght: " + str);
        aWriter.writeCharacters(str);
        aWriter.writeEndElement();
    }

    qv = aNode->getProperty(SMILE_NODE_PARAMETERS);
    if(!qv.isValid() || !qv.canConvert<DoubleList>()){
        QString msg = QString("%1 sub-element is mandatory for noisymax nodes").arg(SMILE_NODE_PARAMETERS);
        this->log.e("writeSmileNodeNoisymax", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_PARAMETERS);
    DoubleList dlist = qv.value<DoubleList>();
    QString params;
    int size = dlist.size();
    for(int i=0; i<size; i++){
        double d = dlist.at(i);
        params.append(QString::number(d, 'g', 10));
        if(i != size-1)
            params.append(" ");
    }
    this->log.d("writeSmileNodeNoisymax", "noisymax parameters: " + params);
    aWriter.writeCharacters(params);
    aWriter.writeEndElement();

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeDeterministic(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_DETERMINISTIC);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node states
    err = writeNodeStates(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write deterministic proper sub-elements
    QVariant qv = aNode->getProperty(SMILE_NODE_RESULTINGSTATES);
    if(!qv.isValid() || !qv.canConvert<QStringList>()){
        QString msg = QString("%1 sub-element is mandatory for deterministic nodes").arg(SMILE_NODE_RESULTINGSTATES);
        this->log.e("writeSmileNodeDeterministic", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_RESULTINGSTATES);
    QStringList sList = qv.value<QStringList>();
    QString res;
    int size = sList.size();
    for(int i=0; i<size; i++){
        res.append(sList.at(i));
        if(i != size-1)
            res.append(" ");
    }
    this->log.d("writeSmileNodeDeterministic", "deterministic resultingstates: " + res);
    aWriter.writeCharacters(res);
    aWriter.writeEndElement();

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeDecision(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_DECISION);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node states
    err = writeNodeStates(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeUtility(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_UTILITY);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write utility proper sub-elements
    QVariant qv = aNode->getProperty(SMILE_NODE_UTILITIES);
    if(!qv.isValid() || !qv.canConvert<DoubleList>()){
        QString msg = QString("%1 sub-element is mandatory for utility nodes").arg(SMILE_NODE_UTILITIES);
        this->log.e("writeSmileNodeUtility", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_UTILITIES);
    DoubleList dlist = qv.value<DoubleList>();
    QString utils;
    int size = dlist.size();
    for(int i=0; i<size; i++){
        double d = dlist.at(i);
        utils.append(QString::number(d, 'g', 10));
        if(i != size-1)
            utils.append(" ");
    }
    this->log.d("writeSmileNodeUtility", "utility utilities: " + utils);
    aWriter.writeCharacters(utils);
    aWriter.writeEndElement();

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodeMau(QXmlStreamWriter& aWriter, const QNNodePtr aNode){
    QNError err;
    aWriter.writeStartElement(SMILE_NODES_MAU);

    //write node attributes
    err = writeNodeAttributes(aWriter, aNode);
    if(err)
        return err;

    //write node parents
    err = writeNodeParents(aWriter, aNode);
    if(err)
        return err;

    //write mau proper sub-elements
    QVariant qv = aNode->getProperty(SMILE_NODE_WEIGHTS);
    if(!qv.isValid() || !qv.canConvert<DoubleList>()){
        QString msg = QString("%1 sub-element is mandatory for mau nodes").arg(SMILE_NODE_WEIGHTS);
        this->log.e("writeSmileNodeMau", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    aWriter.writeStartElement(SMILE_NODE_WEIGHTS);
    DoubleList dlist = qv.value<DoubleList>();
    QString weight;
    int size = dlist.size();
    for(int i=0; i<size; i++){
        double d = dlist.at(i);
        weight.append(QString::number(d, 'g', 10));
        if(i != size-1)
            weight.append(" ");
    }
    this->log.d("writeSmileNodeMau", "mau weight: " + weight);
    aWriter.writeCharacters(weight);
    aWriter.writeEndElement();

    //write node properties
    err = writeNodeProperties(aWriter, aNode);
    if(err)
        return err;

    aWriter.writeEndElement();
    return QNError();
}

QNError SmileXmlWriter::writeSmileNodes(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet){
    QStringList keys = aNet->getAllNodesKeys();
    if(!keys.isEmpty()){
        aWriter.writeStartElement(SMILE_NODES);

        //write every single node
        foreach(QString k, keys){
            QNNodePtr node = aNet->getNode(k);
            if(node == nullptr){
                QString msg = "a network's node is null";
                this->log.e("writeSmileNodes", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }

            QVariant qvType = node->getProperty(SMILE_NODE_TYPE);
            if((!qvType.isValid() || !qvType.canConvert<QString>()) && this->strictMode){
                QString msg = "this node has not a valid node type.";
                this->log.e("writeSmileNodes", msg);
                return QNError(NET_WRITE_ERROR, msg);
            }
            QString type = qvType.toString();
            QNError error;
            if(type == SMILE_NODES_CPT){
                //write smile cpt node
                error = this->writeSmileNodeCpt(aWriter, node);
            }
            else if(type == SMILE_NODES_NOISYMAX){
                //write smile noisymax node
                error = this->writeSmileNodeNoisymax(aWriter, node);
            }
            else if(type == SMILE_NODES_NOISYADDER){
                //write smile noisyadder node
                error = this->writeSmileNodeNoisyadder(aWriter, node);
            }
            else if(type == SMILE_NODES_DETERMINISTIC){
                //write smile deterministic node
                error = this->writeSmileNodeDeterministic(aWriter, node);
            }
            else if(type == SMILE_NODES_DECISION){
                //write smile decision node
                error = this->writeSmileNodeDecision(aWriter, node);
            }
            else if(type == SMILE_NODES_UTILITY){
                //write smile utility node
                error = this->writeSmileNodeUtility(aWriter, node);
            }
            else if(type == SMILE_NODES_MAU){
                //write smile mau node
                error = this->writeSmileNodeMau(aWriter, node);
            }
            else{
                if(this->strictMode){
                    QString msg = QString("the node type '%1' is not supported.").arg(type);
                    this->log.e("writeSmileNodes", msg);
                    error = QNError(NET_WRITE_ERROR, msg);
                }
                else{
                    QString msg = QString("the node type '%1' is not supported; fallback to %2").arg(type).arg(SMILE_NODES_DECISION);
                    this->log.w("writeSmileNodes", msg);
                    SmileNodeStateList states;
                    SmileNodeState ok; ok.setId("OK"); ok.setLabel("OK");
                    SmileNodeState ko; ko.setId("KO"); ko.setLabel("KO");
                    states.push_back(ok);
                    states.push_back(ko);
                    QVariant qv; qv.setValue(states);
                    node->setProperty(SMILE_NODE_STATE_LIST, qv);
                    error = this->writeSmileNodeDecision(aWriter, node);
                }
            }

            if(error)
                return error;
        }

        aWriter.writeEndElement();
    }
    return QNError();
}

QNError SmileXmlWriter::writeSmileObscost(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet){
    QVariant qv = aNet->getProperty(SMILE_OBSCOST);
    if(qv.isValid()){
        if(!qv.canConvert<SmileObservationcost>()){
            QString msg = "smile observationcost is not of the expected type";
            this->log.e("writeSmileObscost", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        SmileObservationcost obscost = qv.value<SmileObservationcost>();
        SmileObscostNodeList* nl = obscost.getNodes();
        if(nl != nullptr){
            aWriter.writeStartElement(SMILE_OBSCOST);
            //write nodes
            for(SmileObscostNode n: *nl){
                aWriter.writeStartElement(SMILE_OBSCOST_NODE);

                //node id - required
                QString id = n.getId();
                if(id.isEmpty()){
                    QString msg = "id attribute is mandatory for obscost's nodes";
                    this->log.e("writeSmileObscost", msg);
                    return QNError(NET_WRITE_ERROR, msg);
                }
                aWriter.writeAttribute(SMILE_ID_TAG, id);

                //parents - optional
                QStringList parents = n.getParents();
                if(!parents.isEmpty()){
                    aWriter.writeStartElement(SMILE_OBSCOST_NODE_PARENTS);
                    QString p;
                    int size = parents.size();
                    for(int i=0; i<size; i++){
                        p.append(parents.at(i));
                        if(i != size-1)
                            p.append(" ");
                    }
                    this->log.d("writeSmileObscost", "parents: " + p);
                    aWriter.writeCharacters(p);
                    aWriter.writeEndElement();
                }

                //costs - required
                DoubleList list = n.getCosts();
                aWriter.writeStartElement(SMILE_OBSCOST_NODE_COST);
                QString costs;
                int size = list.size();
                for(int i=0; i<size; i++){
                    double d = list.at(i);
                    costs.append(QString::number(d, 'g', 10));
                    if(i != size-1)
                        costs.append(" ");
                }
                this->log.d("writeSmileObscost", "costs: " + costs);
                aWriter.writeCharacters(costs);
                aWriter.writeEndElement();

                aWriter.writeEndElement();
            }

            aWriter.writeEndElement();
        }
    }

    return QNError();
}

QNError SmileXmlWriter::writeSmileAlgparams(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet){
    QVariant qvAlgp = aNet->getProperty(SMILE_ALGPARAMS);
    if(qvAlgp.isValid()){
        if(!qvAlgp.canConvert<SmileAlgparams>()){
            QString msg = "smile algparams is not of the expected type";
            this->log.e("writeSmileAlgparams", msg);
            return QNError(NET_WRITE_ERROR, msg);
        }

        SmileAlgparams algp = qvAlgp.value<SmileAlgparams>();
        aWriter.writeStartElement(SMILE_ALGPARAMS);
        this->log.d("writeSmileAlgparams", SMILE_ALGPARAMS);

        //write epis
        SmileEpisPtr e = algp.getEpis();
        if(e != nullptr){
            aWriter.writeStartElement(SMILE_EPIS);
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_PROPLEN).arg(e->getProplen()));
            aWriter.writeAttribute(SMILE_EPIS_PROPLEN, QString::number(e->getProplen(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_NUMSTATES1).arg(e->getNumStates1()));
            aWriter.writeAttribute(SMILE_EPIS_NUMSTATES1, QString::number(e->getNumStates1(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_NUMSTATES2).arg(e->getNumStates2()));
            aWriter.writeAttribute(SMILE_EPIS_NUMSTATES2, QString::number(e->getNumStates2(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_NUMSTATES3).arg(e->getNumStates3()));
            aWriter.writeAttribute(SMILE_EPIS_NUMSTATES3, QString::number(e->getNumStates3(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_EPS1).arg(e->getEps1()));
            aWriter.writeAttribute(SMILE_EPIS_EPS1, QString::number(e->getEps1(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_EPS2).arg(e->getEps2()));
            aWriter.writeAttribute(SMILE_EPIS_EPS2, QString::number(e->getEps2(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_EPS3).arg(e->getEps3()));
            aWriter.writeAttribute(SMILE_EPIS_EPS3, QString::number(e->getEps3(), 'g', 10));
            this->log.d("writeSmileAlgparams", QString("epis %1: %2").arg(SMILE_EPIS_EPS4).arg(e->getEps4()));
            aWriter.writeAttribute(SMILE_EPIS_EPS4, QString::number(e->getEps4(), 'g', 10));
            aWriter.writeEndElement();
        }

        aWriter.writeEndElement();
    }

    return QNError();
}

QNError SmileXmlWriter::writeSmileProperties(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet){
    QStringList keys = aNet->getAllKeys();
    //remove properties not necessary for this format
    keys.removeAll("id");
    keys.removeAll("version");

    bool tagWritten = false;
    if(!keys.isEmpty()){
        for(QString k: keys){
            QVariant qv = aNet->getProperty(k);
            if(qv.canConvert(QMetaType::QString)){
                if(!tagWritten){
                    aWriter.writeStartElement(SMILE_PROPERTIES);
                    tagWritten = true;
                }
                this->log.d("writeSmileProperties", QString("%1: %2").arg(k).arg(qv.toString()));
                aWriter.writeStartElement(SMILE_PROPERTIES_CHILD);
                aWriter.writeAttribute(SMILE_ID_TAG, k);
                aWriter.writeCharacters(qv.toString());
                aWriter.writeEndElement();
            }
        }
        if(tagWritten)
            aWriter.writeEndElement();
    }

    return QNError();
}

QNError SmileXmlWriter::writeSmileChildren(QXmlStreamWriter& aWriter, const QNNetworkPtr aNet){
    QNError err;

    //smile properties
    err = this->writeSmileProperties(aWriter, aNet);
    if(err)
        return err;

    //smile algparams
    err = this->writeSmileAlgparams(aWriter, aNet);
    if(err)
        return err;

    //smile nodes
    err = this->writeSmileNodes(aWriter, aNet);
    if(err)
        return err;

    //smile observationcost
    err = this->writeSmileObscost(aWriter, aNet);
    if(err)
        return err;

    //smile extensions
    err = this->writeSmileExtensions(aWriter, aNet);
    if(err)
        return err;

    return QNError();
}

SmileXmlWriter::SmileXmlWriter(const QString& aSmileDoc, const bool aStrictMode): log("smileXml", "SmileXmlWriter") {
    this->smileDoc = aSmileDoc;
    this->strictMode = aStrictMode;
}

QNError SmileXmlWriter::write(const ScopedQNNetwork& aNet){
    if(aNet.isNull()){
        QString msg("this network is empty.");
        this->log.e("write", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }

    QFile file(this->smileDoc);
    file.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&file);
    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(4);

    //start writing doc
    writer.setCodec("ISO 8859-1");
    writer.writeStartDocument();

    //smile root element
    writer.writeStartElement(SMILE_TAG);

    //net id
    QString id = aNet->getId();
    if(id.isEmpty()){
        file.close();
        QString msg("this network has no id.");
        this->log.e("write", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("write", "smile id: " + id);
    writer.writeAttribute(SMILE_ID_TAG, id);

    //net version
    QString version = aNet->getVersion();
    if(version.isEmpty()){
        file.close();
        QString msg("this network has no version.");
        this->log.e("write", msg);
        return QNError(NET_WRITE_ERROR, msg);
    }
    this->log.d("write", "smile version: " + version);
    writer.writeAttribute(SMILE_ID_VERSION, version);

    //net numsamples - optional
    QVariant qv = aNet->getProperty(SMILE_ID_NUMSAMPLES);
    if(qv.isValid()){
        int numsamples = qv.toInt();
        writer.writeAttribute(SMILE_ID_NUMSAMPLES, QString::number(numsamples, 'g', 10));
        this->log.d("write", "smile numsamples: " + QString::number(numsamples, 'g', 10));
    }

    //write sub-elements
    QNError err =  this->writeSmileChildren(writer, aNet.data());
    if(err){
        file.close();
        return err;
    }

    writer.writeEndElement();
    writer.writeEndDocument();
    file.close();
    return QNError();
}
