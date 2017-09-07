#include "basestyleplugin.h"
#include "basestylexmlparser.h"
#include "basestylexmlwriter.h"
#include "basestyleconst.h"
#include "qnnodeitem.h"
#include "qnmodelitem.h"
#include "qntextboxitem.h"
#include "qnedgeitem.h"
#include "qnedge.h"
#include "qnedgestyle.h"
#include "qnnodeicon.h"
#include "qnmodel.h"
#include "qntextbox.h"
#include "qnstyle.h"
#include "qnstyleclass.h"
#include <QRegExp>


BaseStylePlugin::BaseStylePlugin(){
    this->supportedPattern.setPatternSyntax(QRegExp::Wildcard);
    this->supportedPattern.setPattern("*.xqsf"); //Xml QtNets Style Format
}

QRegExpList BaseStylePlugin::supportedFormats(){
    QRegExpList result;
    result.push_back(this->supportedPattern);
    return result;
}

bool BaseStylePlugin::isSupported(const QString& aFile){
    return this->supportedPattern.exactMatch(aFile);
}

QNError BaseStylePlugin::load(const QString& aFile, ScopedQNStyle& aStyle){
    BaseStyleXmlParser parser(aFile);
    return parser.parse(aStyle);
}

QNError BaseStylePlugin::store(const QString& aFile, const ScopedQNStyle& aStyle){
    BaseStyleXmlWriter writer(aFile);
    return writer.write(aStyle);
}

QNError BaseStylePlugin::updateFont(QNStylePtr aStyle, QGraphicsItemPtr aItem){

    if(aStyle == nullptr)
        return QNError(INVALID_STYLE_STATE, "No style plug-in selected");

    QStringListPtr classes = nullptr;
    QNNodeItemPtr node = nullptr;
    QNModelItemPtr model = nullptr;
    QNTextBoxItemPtr textb = nullptr;
    QNFontPtr fPtr = nullptr;

    switch (aItem->type()) {
        case QNNodeItem::Type :
            node = qgraphicsitem_cast<QNNodeItemPtr>(aItem);
            classes = node->getNode()->getClasses();
            fPtr = node->getNode()->getFont();
            break;
        case QNModelItem::Type :
            model = qgraphicsitem_cast<QNModelItemPtr>(aItem);
            classes = model->getModel()->getClasses();
            fPtr = model->getModel()->getFont();
            break;
        case QNTextBoxItem::Type :
            textb = qgraphicsitem_cast<QNTextBoxItemPtr>(aItem);
            classes = textb->getTextbox()->getClasses();
            fPtr = textb->getTextbox()->getFont();
            break;
        default:
            break;
    }

    //init properties
    QString fontName = (fPtr != nullptr)? fPtr->getFontName(): STYLE_DEFAULT_FONT_NAME;
    int fontSize = (fPtr != nullptr)? fPtr->getFontSize(): STYLE_DEFAULT_FONT_SIZE;
    QString fontColor = (fPtr != nullptr)? fPtr->getFontColor(): STYLE_DEFAULT_FONT_COLOR;
    bool fontBold = (fPtr != nullptr)? (bool)fPtr->isBold(): false;
    bool fontItalic = (fPtr != nullptr)? (bool)fPtr->isItalic(): false;
    BSTextAlign fontAlign = (fPtr != nullptr)? (BSTextAlign)(fPtr->getAlign()-1): BSTextAlign::textLeft;

    //parse classes
    if(classes != nullptr && !classes->isEmpty()){
        int size = classes->size();
        for(int i=0; i<size; i++) {
            QNStyleClassPtr cPtr = aStyle->getClass(classes->at(i));
            if(cPtr != nullptr){
                fontName = cPtr->getProperty(STYLE_CLASS_FONT_NAME).toString();
                fontSize = cPtr->getProperty(STYLE_CLASS_FONT_SIZE).toInt();
                fontColor = cPtr->getProperty(STYLE_CLASS_FONT_COLOR).toString();
                fontBold = cPtr->getProperty(STYLE_CLASS_FONT_BOLD).toBool();
                fontItalic = cPtr->getProperty(STYLE_CLASS_FONT_ITALIC).toBool();
                fontAlign = cPtr->getProperty(STYLE_CLASS_FONT_ALIGN).value<BSTextAlign>();
            }
        }
    }

    //font
    QNFont f(fontName, fontSize, fontColor);
    f.setBold(fontBold);
    f.setItalic(fontItalic);
    f.setAlign((QNAlignment)(fontAlign+1));
    if(node != nullptr) node->updateFont(&f);
    if(model != nullptr) model->updateFont(&f);
    if(textb != nullptr) textb->updateFont(&f);

    return QNError();
}

QNError BaseStylePlugin::paint(QNStylePtr aStyle, QGraphicsItemPtr aItem,
                               QPainterPtr aPainter, QStyleOptionGraphicsItemConstPtr aOption, QWidgetPtr aWidget){

    Q_UNUSED(aWidget)

    if(aStyle == nullptr)
        return QNError(INVALID_STYLE_STATE, "No style plug-in selected");

    //init properties
    QString backColor = STYLE_DEFAULT_BG_COLOR;
    BSBrushStyle backBrush = BSBrushStyle::solid;
    QString outColor = STYLE_DEFAULT_FG_COLOR;
    int outWidth = STYLE_DEFAULT_WIDTH;
    BSLineStyle outLine = BSLineStyle::solidLine;
    BSCapStyle outCap = BSCapStyle::flatCap;
    BSJoinStyle outJoin = BSJoinStyle::miterJoin;
    BSNodeShape shape = BSNodeShape::rectagle;
    QString path = "";
    BSArrowStyle arrow = BSArrowStyle::full;

    int width = 0;
    int height = 0;
    QLineF line;
    QRectF boundingR;
    QNNodeItemPtr node = nullptr;
    QNModelItemPtr model = nullptr;
    QNTextBoxItemPtr textb = nullptr;
    QNEdgeItemPtr edge = nullptr;
    QStringListPtr classes = nullptr;
    switch (aItem->type()) {
        case QNNodeItem::Type :{
            node = qgraphicsitem_cast<QNNodeItemPtr>(aItem);
            width = node->getWidth();
            height = node->getHeight();
            boundingR = node->boundingRect();
            QNNodeIconPtr n = node->getNode();
            classes = n->getClasses();
            backColor = n->getColor();
            outColor = n->getOutlineColor();
            outWidth = n->getOutlineWidth();
            break;
        }
        case QNModelItem::Type :{
            model = qgraphicsitem_cast<QNModelItemPtr>(aItem);
            width = model->getWidth();
            height = model->getHeight();
            boundingR = model->boundingRect();
            QNModelPtr m = model->getModel();
            classes = m->getClasses();
            backColor = m->getColor();
            outColor = m->getOutlineColor();
            outWidth = m->getOutlineWidth();
            break;
        }
        case QNTextBoxItem::Type :{
            textb = qgraphicsitem_cast<QNTextBoxItemPtr>(aItem);
            width = textb->getWidth();
            height = textb->getHeight();
            boundingR = textb->boundingRect();
            classes = textb->getTextbox()->getClasses();
            break;
        }
        case QNEdgeItem::Type :{
            edge = qgraphicsitem_cast<QNEdgeItemPtr>(aItem);
            line = edge->line();
            boundingR = edge->boundingRect();
            classes = edge->getEdge()->getClasses();
            backColor = STYLE_DEFAULT_FG_COLOR;
            break;
        }
        default:
            break;
    }

    //parse classes
    if(classes != nullptr && !classes->isEmpty()){
        int size = classes->size();
        for(int i=0; i<size; i++) {
            QNStyleClassPtr cPtr = aStyle->getClass(classes->at(i));
            if(cPtr != nullptr){
                QVariant v;
                v = cPtr->getProperty(STYLE_CLASS_BACKGROUND_COLOR);
                if(v.isValid())
                    backColor = v.toString();

                v = cPtr->getProperty(STYLE_CLASS_BACKGROUND_BRUSH);
                if(v.isValid())
                    backBrush = v.value<BSBrushStyle>();

                v = cPtr->getProperty(STYLE_CLASS_OUTLINE_COLOR);
                if(v.isValid())
                    outColor = v.toString();

                v = cPtr->getProperty(STYLE_CLASS_OUTLINE_WIDTH);
                if(v.isValid())
                    outWidth = v.toInt();

                v = cPtr->getProperty(STYLE_CLASS_OUTLINE_LINE);
                if(v.isValid())
                    outLine = v.value<BSLineStyle>();

                v = cPtr->getProperty(STYLE_CLASS_OUTLINE_CAP);
                if(v.isValid())
                    outCap = v.value<BSCapStyle>();

                v = cPtr->getProperty(STYLE_CLASS_OUTLINE_JOIN);
                if(v.isValid())
                    outJoin = v.value<BSJoinStyle>();

                v = cPtr->getProperty(STYLE_CLASS_SHAPE);
                if(v.isValid())
                    shape = v.value<BSNodeShape>();

                v = cPtr->getProperty(STYLE_CLASS_SHAPE_PATH);
                if(v.isValid())
                    path = v.toString();

                v = cPtr->getProperty(STYLE_CLASS_ARROW);
                if(v.isValid())
                    arrow = v.value<BSArrowStyle>();
            }
        }
    }

    aPainter->setRenderHint(QPainter::Antialiasing, true);
    aPainter->setRenderHint(QPainter::TextAntialiasing, true);

    //back color
    QColor fillColor = QColor("#" + backColor);
    QBrush brush = aPainter->brush();
    brush.setColor((aOption->state & QStyle::State_Selected) ? fillColor.dark(150) : fillColor);

    //back brush
    switch (backBrush) {
        case BSBrushStyle::linearGradient:{
            QLinearGradient linearGradient(0, 0, width, height);
            linearGradient.setColorAt(0.0, fillColor.light(150));
            linearGradient.setColorAt(0.3, fillColor);
            linearGradient.setColorAt(1.0, fillColor.dark(200));
            brush = linearGradient;
            break;
        }
        case BSBrushStyle::radialGradient:{
            QRadialGradient radialGradient(QPoint(width-5, height-5), width);
            radialGradient.setColorAt(0.0, fillColor.light(150));
            radialGradient.setColorAt(0.3, fillColor);
            radialGradient.setColorAt(1.0, fillColor.dark(200));
            radialGradient.setSpread(QGradient::ReflectSpread);
            brush = radialGradient;
            break;
        }
        default:
            brush.setStyle((Qt::BrushStyle)backBrush);
            break;
    }
    aPainter->setBrush(brush);

    //outline color
    QPen pen = aPainter->pen();
    pen.setColor(QColor("#" + outColor));

    //outline width
    pen.setWidth((aOption->state & QStyle::State_Selected) ? outWidth+2 : outWidth);

    //outline line
    pen.setStyle((Qt::PenStyle)outLine);

    //outline cap
    pen.setCapStyle((Qt::PenCapStyle)outCap);

    //outline join
    pen.setJoinStyle((Qt::PenJoinStyle)outJoin);
    aPainter->setPen(pen);

    //node shape
    if(aItem->type() == QNNodeItem::Type){
        switch (shape) {
            case BSNodeShape::square:{
                int l = (width <= height) ? width : height;
                aPainter->drawRect(0, 0, l, l);
                break;
            }
            case BSNodeShape::roundedSquare:{
                int l = (width <= height) ? width : height;
                aPainter->drawRoundedRect(0, 0, l, l, 10, 10);
                break;
            }
            case BSNodeShape::rectagle:{
                aPainter->drawRect(boundingR);
                break;
            }
            case BSNodeShape::roundedRect:{
                aPainter->drawRoundedRect(boundingR, 10, 10);
                break;
            }
            case BSNodeShape::rhombus:{
                QPoint left(0, height/2);
                QPoint right(width, height/2);
                QPoint top(width/2, 0);
                QPoint bottom(width/2, height);
                QPolygon rhombus;
                rhombus << left << top << right << bottom;
                aPainter->drawPolygon(rhombus);
                break;
            }
            case BSNodeShape::triangle:{
                QPoint bottoml(0, height);
                QPoint bottomr(width, height);
                QPoint top(width/2, 0);
                QPolygon tri;
                tri << bottoml << top << bottomr;
                aPainter->drawPolygon(tri);
                break;
            }
            case BSNodeShape::reverseTriangle:{
                QPoint topl(0, 0);
                QPoint topr(width, 0);
                QPoint bottom(width/2, height);
                QPolygon tri;
                tri << topl << topr << bottom;
                aPainter->drawPolygon(tri);
                break;
            }
            case BSNodeShape::ellipse:{
                aPainter->drawEllipse(node->getCenter(), width/2, height/2);
                break;
            }
            case BSNodeShape::circle:{
                int r = (width <= height) ? width/2 : height/2;
                aPainter->drawEllipse(node->getCenter(), r, r);
                break;
            }
            case BSNodeShape::hexagon:{
                QPoint left(0, height/2);
                QPoint top1(width/4, 0);
                QPoint top2(3*width/4, 0);
                QPoint right(width, height/2);
                QPoint bottom1(width/4, height);
                QPoint bottom2(3*width/4, height);
                QPolygon hexagon;
                hexagon << left << top1 << top2 << right << bottom2 << bottom1;
                aPainter->drawPolygon(hexagon);
                break;
            }
            case BSNodeShape::image:{
                QPixmap pixmap(path);
                int w = aItem->boundingRect().width();
                int h = aItem->boundingRect().height();
                aPainter->drawPixmap(0, 0, w, h, pixmap);
                break;
            }
            default:{
                aPainter->drawRect(boundingR);
                break;
            }
        }
    }

    //arrow shape
    if(aItem->type() == QNEdgeItem::Type){
        edge->setPen(pen);
        edge->setLine(line);
        const double Pi = 3.14;
        if(arrow == BSArrowStyle::full || arrow == BSArrowStyle::empty || arrow == BSArrowStyle::diamond){
            double angle = ::acos(line.dx() / line.length());
            if (line.dy() >= 0)
                angle = (Pi * 2) - angle;

            QPointF arrowP1 = line.p1() + QPointF(sin(angle + Pi/3) * STYLE_ARROW_SIZE,
                                                  cos(angle + Pi/3) * STYLE_ARROW_SIZE);
            QPointF arrowP2 = line.p1() + QPointF(sin(angle + Pi - Pi/3) * STYLE_ARROW_SIZE,
                                                  cos(angle + Pi - Pi/3) * STYLE_ARROW_SIZE);

            pen.setStyle(Qt::PenStyle::SolidLine);
            aPainter->setPen(pen);
            QPolygonF arrowHead;
            if(arrow == BSArrowStyle::full){
                arrowHead << line.p1() << arrowP1 << arrowP2;
                aPainter->drawPolygon(arrowHead);
            }
            else if(arrow == BSArrowStyle::empty){
                aPainter->drawLine(QLineF(line.p1(), arrowP1));
                aPainter->drawLine(QLineF(line.p1(), arrowP2));
            }
            else{
                QPointF arrowP3 = line.p1() + QPointF(sin(angle + Pi/2) * STYLE_ARROW_SIZE*1.8,
                                                      cos(angle + Pi/2) * STYLE_ARROW_SIZE*1.8);
                arrowHead << line.p1() << arrowP1 << arrowP3 << arrowP2;
                aPainter->drawPolygon(arrowHead);
            }
        }
        else if(arrow == BSArrowStyle::circular){
            double angle = ::acos(line.dx() / line.length());
            if (line.dy() >= 0)
                angle = (Pi * 2) - angle;

            QPointF center = line.p1() + QPointF(sin(angle + Pi/2) * STYLE_ARROW_R,
                                                 cos(angle + Pi/2) * STYLE_ARROW_R);
            pen.setStyle(Qt::PenStyle::SolidLine);
            aPainter->setPen(pen);
            aPainter->drawEllipse(center, STYLE_ARROW_R, STYLE_ARROW_R);
        }
    }

    //model shape
    if(aItem->type() == QNModelItem::Type){
        int textW = model->getLabel().size().width();
        int textH = model->getLabel().size().height();
        int arch = ((double)textH / STYLE_DEFAULT_TEXT_H)*STYLE_MODEL_ARCH;
        QPoint topLeft(0,0);
        QPoint bottomLeft(0,textH);
        QPoint bottomRight(textW+arch, textH);
        QPoint topRight(textW,0);
        QPolygon nameSlot;
        nameSlot << topLeft << bottomLeft << bottomRight << topRight;
        aPainter->drawPolygon(nameSlot);

        topLeft = QPoint(0,textH);
        bottomLeft = QPoint(0,height);
        bottomRight = QPoint(width,height);
        topRight = QPoint(width,textH);
        QPolygon body;
        body << topLeft << bottomLeft << bottomRight << topRight;
        aPainter->drawPolygon(body);
    }

    //textbox shape
    if(aItem->type() == QNTextBoxItem::Type){
        QPoint topLeft(0,0);
        QPoint bottomLeft(0,height);
        QPoint bottomRight(width,height);
        QPoint topRight1(width,STYLE_TEXTBOX_ARC);
        QPoint topRight2(width-STYLE_TEXTBOX_ARC,0);
        QPolygon body;
        body << topLeft << bottomLeft << bottomRight << topRight1 << topRight2;
        aPainter->drawPolygon(body);
        QPolygon tri;
        tri << topRight2 << QPoint(width-STYLE_TEXTBOX_ARC,STYLE_TEXTBOX_ARC) << topRight1;
        aPainter->drawPolygon(tri);
    }

    return QNError();
}
