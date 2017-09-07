/**
 ** @file benchmarkclass.cpp
 ** @author Angelo Prudentino
 ** @date 2017-02-18
 **
 ** Copyright 2017 Angelo Prudentino <angelo.prudentino@studenti.polito.it>.
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

#include <QCoreApplication>
#include <QFile>
#include <QDir>
#include <QtGlobal>
#include "benchmarkclass.h"
#include "qnnetwork.h"
#include "qnnetworkmodel.h"
#include "qnnode.h"
#include "qnedge.h"
#include "qnnodeicon.h"
#include "qnmodel.h"
#include "qnedgestyle.h"
#include "qntextbox.h"
#include "qnproperties.h"
#include "qnpersistencemanager.h"
#include "smilenode.h"
#include "smiletypes.h"
#include "smilexmlconst.h"
#include "smilealgparams.h"
#include "smileobservationcost.h"
#include "smilegenieext.h"
#include "utility.h"
#include "smilenodegenieext.h"
#include "qnstylemanager.h"
#include "qnstyle.h"
#include "qnstyleclass.h"
#include "basestyleconst.h"
#include "basestyletypes.h"

#define intRand() qrand()
#define doubleRand() qrand() + ((double)qrand() / RAND_MAX) * qrand()
#define stringRand() genRandString(qrand()%30, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
#define idRand() genRandString(20, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
#define colorRand() genRandString(6, "ABCDEFabcdef0123456789")

#define STYLE_FILE "./test.xqsf"
#define NET_FILE "./test.xdsl"

#define SEC 1000
#define MIN (60*SEC)
#define HOUR (60*MIN)


void BenchmarkClass::init(){
    this->nodeIds.clear();
    this->edgeIds.clear();
    this->modelIds.clear();

    const QString NET_ID = idRand();
    const QString NET_VERSION = QString::number(doubleRand()/10, 'f', 2);
    const QString NET_PROP_1 = "property_1";
    const QString NET_PROP_2 = "property_2";
    const QString NET_PROP_3 = "property_3";
    const QString NET_PROP_4 = "property_4";
    const QString NET_PROP_1_VALUE = QString::number(intRand());
    const QString NET_PROP_2_VALUE = "";
    const QString NET_PROP_3_VALUE = stringRand();
    const QString COMMENT = stringRand();

    //init network
    this->net.reset(new QNNetwork(NET_ID, NET_VERSION));

    //network properties
    this->net->setProperty(NET_PROP_1, NET_PROP_1_VALUE);
    this->net->setProperty(NET_PROP_2, NET_PROP_2_VALUE);
    this->net->setProperty(NET_PROP_3, NET_PROP_3_VALUE);
    this->net->setProperty(NET_PROP_4, QVariant());

    //algparams
    SmileAlgparams origAlgP;
    ScopedSmileEpis origEpis(new SmileEpis());
    origEpis->setProplen(intRand()); origEpis->setNumStates1(intRand()); origEpis->setNumStates2(intRand());
    origEpis->setNumStates3(doubleRand()); origEpis->setEps1(doubleRand()); origEpis->setEps2(doubleRand());
    origEpis->setEps3(doubleRand()); origEpis->setEps4(doubleRand());
    origAlgP.setEpis(origEpis);
    QVariant qv; qv.setValue(origAlgP);
    this->net->setProperty(SMILE_ALGPARAMS, qv);

    //observationcost
    SmileObservationcost origObsCost;
    SmileObscostNodeList origNodes;
    SmileObscostNode node(idRand());
    node.addParent(idRand()); node.addParent(idRand());
    node.addCost(doubleRand()); node.addCost(doubleRand());
    origObsCost.addNode(node);
    origNodes.push_back(node);
    node = SmileObscostNode(idRand());
    node.addCost(doubleRand()); node.addCost(doubleRand()); node.addCost(doubleRand());
    origObsCost.addNode(node);
    origNodes.push_back(node);
    qv.setValue(origObsCost);
    this->net->setProperty(SMILE_OBSCOST, qv);

    this->netModel = new QNNetworkModel(this->net.data());
    //genie
    SmileGenieExt origGenie;
    origGenie.setName(NET_ID);
    origGenie.setVersion(NET_VERSION);
    origGenie.setComment(COMMENT);
    origGenie.setFaultnameformat(GenieFNF::nodestate);
    GenieDAFPtr origDaf = new GenieDAF();
    origDaf->setAuxiliary(colorRand()); origDaf->setTarget(colorRand());
    origDaf->setTargetnr(colorRand()); origDaf->setObservation(colorRand());
    origDaf->setObservationnr(colorRand());
    origGenie.setDiagautoformat(origDaf);
    qv.setValue(origGenie);
    this->netModel->setProperty(SMILE_GENIE, qv);
    this->net->setNetModel(this->netModel);

    const bool TARGET = (intRand()%2 > 0)? true: false;
    const bool RANKED = (intRand()%2 > 0)? true: false;
    const bool MANDATORY = (intRand()%2 > 0)? true: false;
    const DiagType DIAGTYPE = (DiagType)(intRand()%DiagType::auxiliary);
    const QString STATE_1 = idRand();
    const QString STATE_2 = idRand();
    const OptionalBool STATE_TARGET = (intRand()%2 > 0)? true: false;
    const OptionalBool DEFAULT = (intRand()%2 > 0)? true: false;
    const OptionalBool FAULT = (intRand()%2 > 0)? true: false;
    const QString FONT = idRand();
    const QString FONT_COL = colorRand();
    const QString BACK_COL = colorRand();
    const QString OUT_COL = colorRand();
    const int TOP = intRand();
    const int LEFT = intRand();
    const int RIGHT = intRand();
    const int BOTTOM = intRand();
    const int WIDTH = intRand();
    const int FONT_SIZE = intRand();
    const OptionalBool BOLD = (intRand()%2 > 0)? true: false;
    const OptionalBool ITALIC = (intRand()%2 > 0)? true: false;
    const QNAlignment ALIGN = (QNAlignment)(intRand()%QNAlignment::rightAlign);
    const QString PATH_1 = idRand();
    const QString TITLE_1 = idRand();
    const QString PATH_2 = idRand();
    const QString TITLE_2 = idRand();
    const QString QUESTION = idRand();
    const GenieFNF FNF = (GenieFNF)(intRand()%GenieFNF::inherit);

    //states
    SmileNodeStateList states;
    SmileNodeState ok; ok.setId(STATE_1); ok.setLabel(STATE_1);
    ok.setDefault(DEFAULT); ok.setFault(FAULT); ok.setTarget(STATE_TARGET);
    SmileNodeState ko; ko.setId(STATE_2); ko.setLabel(STATE_2);
    states.push_back(ok);
    states.push_back(ko);

    //genie states
    GenieNodeStateList genieStates;
    GenieNodeState genieOK; genieOK.setId(STATE_1); genieOK.setComment(COMMENT);
    genieOK.setFix(STATE_1); genieOK.setFaultname(STATE_1);
    genieStates.push_back(genieOK);
    GenieLinkList links;
    GenieLink l1; l1.setPath(PATH_1); l1.setTitle(TITLE_1); links.push_back(l1);
    genieOK.addLink(l1);
    GenieLink l2; l2.setPath(PATH_2); l2.setTitle(TITLE_2); links.push_back(l2);
    genieOK.addLink(l2);
    GenieNodeState genieKO; genieKO.setId(STATE_2); genieKO.setComment(COMMENT);
    genieKO.setFix(STATE_2); genieKO.setFaultname(STATE_2);
    genieStates.push_back(genieKO);

    //barchart
    GenieBarchart bar; bar.setActive((intRand()%2 > 0)? true: false);
    bar.setHeight(intRand()); bar.setWidth(intRand());

    //defcomment list
    GenieTableCommentList defComments;
    GenieTableComment tc1; tc1.setCol(intRand()); tc1.setRow(intRand());
    defComments.push_back(tc1);
    GenieTableComment tc2; tc2.setCol(intRand()); tc2.setRow(intRand());
    defComments.push_back(tc2);
    GenieTableComment tc3; tc3.setCol(intRand()); tc3.setRow(intRand());
    defComments.push_back(tc3);

    //costcomment list
    GenieTableCommentList costComments;
    GenieTableComment tc4; tc4.setCol(intRand()); tc4.setRow(intRand());
    costComments.push_back(tc4);
    GenieTableComment tc5; tc5.setCol(intRand()); tc5.setRow(intRand());
    costComments.push_back(tc5);
    GenieTableComment tc6; tc6.setCol(intRand()); tc6.setRow(intRand());
    costComments.push_back(tc6);


    //init nodes
    for(int i=0; i<NUM_CPT; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_CPT);
        QVariant qv; qv.setValue(states);
        node->setProperty(SMILE_NODE_STATE_LIST, qv);
        DoubleList ld; ld.append(doubleRand());
        ld.append(doubleRand()); ld.append(doubleRand());
        qv.setValue(ld);
        node->setProperty(SMILE_NODE_PROB, qv);
        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_NOISYMAX; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_NOISYMAX);
        QVariant qv; qv.setValue(states);
        node->setProperty(SMILE_NODE_STATE_LIST, qv);
        //parameters
        DoubleList ld2; ld2.append(doubleRand());
        ld2.append(doubleRand()); ld2.append(doubleRand());
        qv.setValue(ld2);
        node->setProperty(SMILE_NODE_PARAMETERS, qv);
        //strengths
        IntList il; il.append(intRand());
        il.append(intRand()); il.append(intRand());
        qv.setValue(il);
        node->setProperty(SMILE_NODE_STRENGHTS, qv);

        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_NOISYADDER; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_NOISYADDER);
        QVariant qv; qv.setValue(states);
        node->setProperty(SMILE_NODE_STATE_LIST, qv);
        //weights
        DoubleList ld; ld.append(doubleRand());
        ld.append(doubleRand()); ld.append(doubleRand());
        qv.setValue(ld);
        node->setProperty(SMILE_NODE_WEIGHTS, qv);
        //parameters
        DoubleList ld2; ld2.append(doubleRand());
        ld2.append(doubleRand()); ld2.append(doubleRand());
        qv.setValue(ld2);
        node->setProperty(SMILE_NODE_PARAMETERS, qv);
        //dstates
        IntList il; il.append(intRand());
        il.append(intRand()); il.append(intRand());
        qv.setValue(il);
        node->setProperty(SMILE_NODE_DSTATES, qv);

        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_DECISION; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_DECISION);
        QVariant qv; qv.setValue(states);
        node->setProperty(SMILE_NODE_STATE_LIST, qv);
        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_DETER; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_DETERMINISTIC);
        QVariant qv; qv.setValue(states);
        node->setProperty(SMILE_NODE_STATE_LIST, qv);
        //resultingstates
        QStringList l; l.append(stringRand());
        l.append(stringRand()); l.append(stringRand());
        qv.setValue(l);
        node->setProperty(SMILE_NODE_RESULTINGSTATES, qv);
        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_UTIL; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_UTILITY);
        DoubleList ld; ld.append(doubleRand());
        ld.append(doubleRand()); ld.append(doubleRand());
        QVariant qv; qv.setValue(ld);
        node->setProperty(SMILE_NODE_UTILITIES, qv);
        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_MAU; i++){
        //create a new node
        QNNodePtr node = new QNNode(this->net.data());
        node->initId();
        node->setProperty(SMILE_NODE_TYPE, SMILE_NODES_MAU);
        DoubleList ld; ld.append(doubleRand());
        ld.append(doubleRand()); ld.append(doubleRand());
        QVariant qv; qv.setValue(ld);
        node->setProperty(SMILE_NODE_WEIGHTS, qv);
        this->net->addNode(node);
        this->nodeIds.push_back(node->getId());
    }

    for(int i=0; i<NUM_NODES; i++){
        QNNodePtr node = this->net->getNode(this->nodeIds.at(i));
        //set common properties
        node->setProperty(SMILE_NODES_TARGET, TARGET);
        node->setProperty(SMILE_NODES_RANKED, RANKED);
        node->setProperty(SMILE_NODES_MANDATORY, MANDATORY);
        node->setProperty(SMILE_NODES_DIAGTYPE, DIAGTYPE);

        //create a new node icon
        QNModelPtr container = this->netModel;
        QNNodeIconPtr nodeIcon = new QNNodeIcon(container);
        nodeIcon->setData(node);
        node->setIcon(nodeIcon);
        nodeIcon->setId(node->getId());
        nodeIcon->setName("New Node " + QString::number(i+1));
        QNFont* f = new QNFont(FONT, FONT_SIZE, FONT_COL);
        f->setBold(BOLD);
        f->setItalic(ITALIC);
        f->setAlign(ALIGN);
        nodeIcon->setFont(f);
        nodeIcon->setColor(BACK_COL);
        nodeIcon->setOutlineColor(OUT_COL);
        nodeIcon->setOutlineWidth(WIDTH);
        nodeIcon->setPosition(new QNPosition(LEFT, TOP, RIGHT, BOTTOM));
        nodeIcon->setComment(COMMENT);
        //genie properties
        QVariant qv; qv.setValue(genieStates);
        nodeIcon->setProperty(SMILE_GENIE_NODE_STATE, qv);
        qv.setValue(bar);
        nodeIcon->setProperty(SMILE_GENIE_BARCHART, qv);
        qv.setValue(QUESTION);
        nodeIcon->setProperty(SMILE_GENIE_QUESTION, qv);
        qv.setValue(links);
        nodeIcon->setProperty(SMILE_GENIE_LINK, qv);
        qv.setValue(defComments);
        nodeIcon->setProperty(SMILE_GENIE_DEFCOMMENT, qv);
        qv.setValue(costComments);
        nodeIcon->setProperty(SMILE_GENIE_COSTCOMMENT, qv);
        qv.setValue(FNF);
        nodeIcon->setProperty(SMILE_GENIE_FAULTNAMEFORMAT, qv);
        container->addIcon(nodeIcon);
    }
    this->nodeIds.sort();

    //init edges
    for(int i=0; i<NUM_NODES; i++){
        QNNodePtr source = this->net->getNode(this->nodeIds.at(i));
        int j = (i+1)%NUM_NODES;
        //        for(int j=0; j<NUM_NODES; j++){
        //            if(i </*!=*/ j){
        QNNodePtr dest = this->net->getNode(this->nodeIds.at(j));

        //create Edge
        source->addSuccessor(dest);
        dest->addAncestor(source);
        QNEdgePtr edge = new QNEdge(source, dest, this->net.data());
        this->net->addEdge(edge);
        this->edgeIds.push_back(source->getId() + " - " + dest->getId());

        //create EdgeStyle
        QNEdgeStylePtr edgeStyle = new QNEdgeStyle(edge);
        edge->setIcon(edgeStyle);
        edgeStyle->setComment(stringRand());
        this->netModel->addEdge(edgeStyle);
        //            }
        //        }
    }
    this->edgeIds.sort();

    //init models and textboxes
    for(int i=0; i<NUM_MODELS; i++){
        QNModelPtr container = this->netModel;
        QNModelPtr model = new QNModel(container);
        model->initId();
        this->modelIds.insert(i, model->getId());
        model->setName("New Model " + QString::number(i+1));
        QNFont* f = new QNFont(FONT, FONT_SIZE, FONT_COL);
        f->setBold(BOLD);
        f->setItalic(ITALIC);
        f->setAlign(ALIGN);
        model->setFont(f);
        model->setColor(BACK_COL);
        model->setOutlineColor(OUT_COL);
        model->setOutlineWidth(WIDTH);
        model->setPosition(new QNPosition(LEFT, TOP, RIGHT, BOTTOM));
        model->setWindow(new QNPosition(LEFT, TOP, RIGHT, BOTTOM));
        model->setComment(COMMENT);
        container->addIcon(model);
    }
    this->modelIds.sort();

    for(int i=0; i<NUM_TEXTBOXES; i++){
        QNModelPtr container = this->netModel;
        QNTextBoxPtr textb = new QNTextBox(container);
        textb->setCaption("New Note " + QString::number(i+1));
        QNFont* f = new QNFont(FONT, FONT_SIZE, FONT_COL);
        f->setBold(BOLD);
        f->setItalic(ITALIC);
        f->setAlign(ALIGN);
        textb->setFont(f);
        textb->setComment(COMMENT);
        textb->setPosition(new QNPosition(LEFT, TOP, RIGHT, BOTTOM));
        container->addTextBox(textb);
    }
}

void BenchmarkClass::initClasses(){
    //init style
    const QString ID = idRand();
    const QString VERSION = QString::number(doubleRand()/10, 'f', 2);
    this->styleMgr->newStyle(STYLE_FILE, ID, VERSION);
    QNStylePtr style = this->styleMgr->getStyle();

    for(int i=0; i<this->NUM_CLASSES; i++){
        const QString CLASS = idRand();
        const QFont::StyleHint FONT = (QFont::StyleHint)(intRand()%QFont::StyleHint::Fantasy);
        const QString FONT_COL = colorRand();
        const int FONT_SIZE = intRand();
        const bool BOLD = (intRand()%2 > 0)? true: false;
        const bool ITALIC = (intRand()%2 > 0)? true: false;
        const BSTextAlign ALIGN = (BSTextAlign)(intRand()%BSTextAlign::textRight);
        const QString BACK_COL = colorRand();
        const QString OUT_COL = colorRand();
        const int WIDTH = intRand();
        const BSBrushStyle BRUSH = (BSBrushStyle)(intRand()%BSBrushStyle::radialGradient);
        const BSLineStyle LINE = (BSLineStyle)(intRand()%BSLineStyle::dashDotDotLine);
        const BSCapStyle CAP = (BSCapStyle)(intRand()%BSCapStyle::roundCap);
        const BSJoinStyle JOIN = (BSJoinStyle)(intRand()%BSJoinStyle::roundJoin);
        const BSNodeShape SHAPE = (BSNodeShape)(intRand()%BSNodeShape::hexagon);
        const BSArrowStyle ARROW = (BSArrowStyle)(intRand()%BSArrowStyle::diamond);

        QNStyleClassPtr c = new QNStyleClass(CLASS);
        c->setProperty(STYLE_CLASS_BACKGROUND_COLOR, BACK_COL);
        c->setProperty(STYLE_CLASS_BACKGROUND_BRUSH, BRUSH);
        c->setProperty(STYLE_CLASS_OUTLINE_COLOR, OUT_COL);
        c->setProperty(STYLE_CLASS_OUTLINE_WIDTH, WIDTH);
        c->setProperty(STYLE_CLASS_OUTLINE_LINE, LINE);
        c->setProperty(STYLE_CLASS_OUTLINE_CAP, CAP);
        c->setProperty(STYLE_CLASS_OUTLINE_JOIN, JOIN);
        c->setProperty(STYLE_CLASS_SHAPE, SHAPE);
        c->setProperty(STYLE_CLASS_ARROW, ARROW);
        QFont f; f.setStyleHint(FONT);
        c->setProperty(STYLE_CLASS_FONT_NAME, f);
        c->setProperty(STYLE_CLASS_FONT_COLOR, FONT_COL);
        c->setProperty(STYLE_CLASS_FONT_SIZE, FONT_SIZE);
        c->setProperty(STYLE_CLASS_FONT_ALIGN, ALIGN);
        c->setProperty(STYLE_CLASS_FONT_BOLD, BOLD);
        c->setProperty(STYLE_CLASS_FONT_ITALIC, ITALIC);
        c = style->addClass(c);
    }
}

void BenchmarkClass::logErr(const QString& aMsg){
    qCritical("***************************");
    qCritical("***************************");
    qCritical("**");
    qCritical(QString("** ").append(aMsg).toStdString().c_str());
    qCritical("**");
    qCritical("***************************");
    qCritical("***************************");
}

void BenchmarkClass::printResults(const QString& aMsg){
    //log to console
    qDebug(aMsg.toStdString().c_str());

    //log to file
    QDir currDir(QCoreApplication::applicationDirPath());
    QString resFile = currDir.absoluteFilePath("results.txt");
    QString toLog = QString("%1\n").arg(aMsg);

    QFile outFile(resFile);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    outFile.write(toLog.toStdString().c_str());
    outFile.close();
}

QString BenchmarkClass::formatTime(const int aMs){
    if(aMs <= SEC){
        return QString::number(aMs) + "ms";
    }
    else if(aMs > SEC && aMs <= MIN){
        return QString::number(aMs/SEC) + "s " +
                QString::number(aMs%SEC) + "ms";
    }
    else if(aMs > MIN && aMs <= HOUR){
        int min = aMs/MIN;
        int sec = (aMs%MIN)/SEC;
        int ms = (aMs%MIN)%SEC;
        return QString::number(min) + "m " +
                QString::number(sec) + "s " +
                QString::number(ms) + "ms";
    }
    else{
        int h = aMs/HOUR;
        int min = (aMs%HOUR)/MIN;
        int sec = ((aMs%HOUR)%MIN)/SEC;
        int ms = ((aMs%HOUR)%MIN)%SEC;
        return QString::number(h) + "h " +
                QString::number(min) + "m " +
                QString::number(sec) + "s " +
                QString::number(ms) + "ms";
    }
}

BenchmarkClass::BenchmarkClass(){
#ifdef QT_DEBUG
        this->persistMgr = new QNPersistenceManager(QDir("/home/angelo/Projects/QtNets/Projects/Plugin/SmilePersistencyPlugin/Debug/"));
        this->styleMgr = new QNStyleManager(QDir("/home/angelo/Projects/QtNets/Projects/Plugin/BaseStylePlugin/Debug/"));
#else
        QDir pluginDir = QDir(QCoreApplication::applicationDirPath());
        pluginDir.cd("Plugins");
        this->persistMgr = new QNPersistenceManager(pluginDir);
        this->styleMgr = new QNStyleManager(pluginDir);
#endif
}

BenchmarkClass::~BenchmarkClass(){
    this->net.reset();
    delete this->persistMgr;
    delete this->styleMgr;
}

void BenchmarkClass::benchmark(){
    this->timer.start();
    printResults("###############################");
    printResults(QString("# %1 %2").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd")).arg(timer.toString()));
    printResults("###############################");
    printResults("#");

    for(int i=0; i<5; i++){
        //init network
        int value = pow10(i+1);
        this->NUM_CPT = value;
        this->NUM_NOISYMAX = value;
        this->NUM_NOISYADDER = value;
        this->NUM_DECISION = value;
        this->NUM_DETER = value;
        this->NUM_UTIL = value;
        this->NUM_MAU = value;
        this->NUM_MODELS = value;
        this->NUM_TEXTBOXES = value;
        this->NUM_CLASSES = value;
        this->NUM_NODES = this->NUM_CPT + this->NUM_NOISYMAX + this->NUM_NOISYADDER+
                          this->NUM_DECISION + this->NUM_DETER + this->NUM_UTIL + this->NUM_MAU;

        this->NUM_EDGES = this->NUM_NODES; //(this->NUM_NODES * (this->NUM_NODES-1))/2;

        this->NUM_STYLABLES = this->NUM_NODES + this->NUM_EDGES +
                              this->NUM_MODELS + this->NUM_TEXTBOXES;

        printResults("# *********************");
        printResults("# *********************");
        printResults("# ** Nodes: " + QString::number(this->NUM_NODES));
        printResults("# ** Edges: " + QString::number(this->NUM_EDGES));
        printResults("# ** Models: " + QString::number(this->NUM_MODELS));
        printResults("# ** Textboxes: " + QString::number(this->NUM_TEXTBOXES));
        printResults("# ** Style classes: " + QString::number(this->NUM_CLASSES));
        printResults("# *********************");
        printResults("# ** ");

        //init
        this->timer.restart();
        this->init();
        int ms = this->timer.elapsed();
        printResults(QString("# ** network initialization: %1").arg(formatTime(ms)));

        //store on file
        this->timer.restart();
        QNError err = this->persistMgr->store(NET_FILE, this->net);
        ms = this->timer.elapsed();
        if(err)
            printResults(QString("# ** store with error [%1]: %2").arg(err.getDetail()).arg(formatTime(ms)));
        else
            printResults(QString("# ** store: %1").arg(formatTime(ms)));

        //load from file
        this->timer.restart();
        ScopedQNNetwork tmp;
        err = this->persistMgr->load(NET_FILE, tmp);
        ms = this->timer.elapsed();
        if(err)
            printResults(QString("# ** load with error [%1]: %2").arg(err.getDetail()).arg(formatTime(ms)));
        else
            printResults(QString("# ** load: %1").arg(formatTime(ms)));
        tmp.reset();
        QFile(NET_FILE).remove();

        //extract all nodes
        QStringList keys = this->net->getAllNodesKeys();
        this->timer.restart();
        foreach (QString k, keys) {
            this->net->getNode(k);
        }
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all nodes (one by one): %1").arg(formatTime(ms)));

        this->timer.restart();
        this->net->getNodesByProperties();
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all nodes (by empty properties): %1").arg(formatTime(ms)));

        //extract all nodes with comment = "something"
        QNProperty p("comment", "something");
        QNPropertyList list; list.append(p);
        this->timer.restart();
        this->net->getNodesByProperties(list);
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all nodes with comment \"something\": %1").arg(formatTime(ms)));


        //extract all edges
        keys = this->net->getAllEdgesKeys();
        this->timer.restart();
        foreach (QString k, keys) {
            this->net->getEdge(k);
        }
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all edges (one by one): %1").arg(formatTime(ms)));

        this->timer.restart();
        this->net->getEdgesByProperties();
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all edges (by empty properties): %1").arg(formatTime(ms)));

        //extract all edges with comment = "something"
        this->timer.restart();
        this->net->getEdgesByProperties(list);
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all edges with comment \"something\": %1").arg(formatTime(ms)));


        //extract all icons
        keys = this->netModel->getAllIconsKeys();
        this->timer.restart();
        foreach (QString k, keys) {
            this->net->getNetModel()->getIcon(k);
        }
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all icons (one by one): %1").arg(formatTime(ms)));

        this->timer.restart();
        this->net->getNetModel()->getIconsByProperties();
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all icons (by empty properties): %1").arg(formatTime(ms)));

        //extract all icons with comment = "something"
        this->timer.restart();
        this->net->getNetModel()->getIconsByProperties(list);
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all icons with comment \"something\": %1").arg(formatTime(ms)));


        QNTextBoxList textboxes;
        QNTextBoxListPtr l = this->net->getNetModel()->getTextboxes();
        if(l != nullptr){
            //extract all textboxes one by one
            this->timer.restart();
            for(int i=0; i<l->size(); i++){
                textboxes.push_back(l->at(i));
            }
            ms = this->timer.elapsed();
            printResults(QString("# ** extraction of all textboxes (one by one): %1").arg(formatTime(ms)));
        }

        this->timer.restart();
        this->net->getNetModel()->getTextboxesByProperties();
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all textboxes (by empty properties): %1").arg(formatTime(ms)));

        //extract all textboxes with comment = "something"
        this->timer.restart();
        this->net->getNetModel()->getTextboxesByProperties(list);
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all textboxes with comment \"something\": %1").arg(formatTime(ms)));


        //extract all edgeStyles
        keys = this->netModel->getAllEdgesKeys();
        this->timer.restart();
        foreach (QString k, keys) {
            this->net->getNetModel()->getEdge(k);
        }
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all edgeStyles (one by one): %1").arg(formatTime(ms)));

        this->timer.restart();
        this->net->getNetModel()->getEdgesByProperties();
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all edgeStyles (by empty properties): %1").arg(formatTime(ms)));

        //extract all edgeStyles with comment = "something"
        this->timer.restart();
        this->net->getNetModel()->getEdgesByProperties(list);
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all edgeStyles with comment \"something\": %1").arg(formatTime(ms)));


        //extract all stylables one by one
        this->timer.restart();
        QNStylableList stylables = this->net->getNetModel()->getStylablesByClass();
        for(int i=0; i<stylables.size(); i++){
            stylables.at(i);
        }
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all stylables (one by one): %1").arg(formatTime(ms)));

        //extract all stylables
        this->timer.restart();
        stylables = this->net->getNetModel()->getStylablesByClass();
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all stylables (by empty properties): %1").arg(formatTime(ms)));

        //extract all stylables with class "node"
        this->timer.restart();
        QStringList classes = QStringList("node");
        this->net->getNetModel()->getStylablesByClass(classes);
        ms = this->timer.elapsed();
        printResults(QString("# ** extraction of all stylables with class \"node\": %1").arg(formatTime(ms)));

        //init classes
        this->timer.restart();
        this->initClasses();
        ms = this->timer.elapsed();
        printResults(QString("# ** style classes initialization: %1").arg(formatTime(ms)));

        //store style on file
        this->timer.restart();
        err = this->styleMgr->store(STYLE_FILE);
        ms = this->timer.elapsed();
        if(err)
            printResults(QString("# ** store style with error [%1]: %2").arg(err.getDetail()).arg(formatTime(ms)));
        else
            printResults(QString("# ** store style: %1").arg(formatTime(ms)));

        //load style from file
        this->timer.restart();
        QNStyleManagerPtr tmpStyle;
#ifdef QT_DEBUG
        tmpStyle = new QNStyleManager(QDir("/home/angelo/Projects/QtNets/Projects/Plugin/BaseStylePlugin/Debug/"));
#else
        QDir pluginDir = QDir(QCoreApplication::applicationDirPath());
        pluginDir.cd("Plugins");
        tmpStyle = new QNStyleManager(pluginDir);
#endif
        err = tmpStyle->load(STYLE_FILE);
        ms = this->timer.elapsed();
        if(err)
            printResults(QString("# ** load style with error [%1]: %2").arg(err.getDetail()).arg(formatTime(ms)));
        else
            printResults(QString("# ** load style: %1").arg(formatTime(ms)));
        delete tmpStyle;
        QFile(STYLE_FILE).remove();

        printResults("# *********************");
        printResults("# *********************");
        printResults("#");
    }
    printResults("###############################");
}
