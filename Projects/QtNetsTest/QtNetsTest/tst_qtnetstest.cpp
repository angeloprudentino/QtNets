/**
 ** @file tst_qtnetstest.cpp
 ** @author Angelo Prudentino
 ** @date 2017-02-09
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

#include <QtTest>
#include <QtWidgets>
#include "qntypes.h"
#include "qnnetwork.h"
#include "qnnetworkmodel.h"
#include "qnnode.h"
#include "qnedge.h"
#include "qnnodeicon.h"
#include "qnmodel.h"
#include "qnedgestyle.h"
#include "qntextbox.h"
#include "qnproperties.h"
#include "qnnetworkscene.h"
#include "qndrawtypes.h"
#include "qnnetworkview.h"
#include "qnstylemanager.h"
#include "qnpersistencetypes.h"
#include "qnpersistencemanager.h"
#include "smilenode.h"
#include "smiletypes.h"
#include "smilexmlconst.h"
#include "qnstyle.h"
#include "qnstyleclass.h"
#include "basestyleconst.h"
#include "basestyletypes.h"
#include "smilealgparams.h"
#include "smileobservationcost.h"
#include "smilegenieext.h"
#include "utility.h"
#include "smilenodegenieext.h"
#include "qnnodeitem.h"
#include "qnedgeitem.h"
#include "qnmodelitem.h"
#include "qntextboxitem.h"

#define intRand() qrand()
#define doubleRand() qrand() + ((double)qrand() / RAND_MAX) * qrand()
#define stringRand() genRandString(qrand()%30)
#define idRand() genRandString(20, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")
#define colorRand() genRandString(6, "ABCDEFabcdef0123456789")

#define NUM_CPT 2
#define NUM_NOISYMAX 2
#define NUM_NOISYADDER 2
#define NUM_DECISION 2
#define NUM_DETER 2
#define NUM_UTIL 2
#define NUM_MAU 2
#define NUM_MODELS 10
#define NUM_TEXTBOXES 10
#define NUM_NODES (NUM_CPT+NUM_NOISYMAX+NUM_NOISYADDER+NUM_DECISION+NUM_DETER+NUM_UTIL+NUM_MAU)
#define NUM_NODES_PROP (NUM_NODES/3)
#define NUM_EDGES (NUM_NODES*(NUM_NODES-1))
#define NUM_EDGES_PROP (NUM_EDGES/3)
#define NUM_MODELS_PROP (NUM_MODELS/3)
#define NUM_STYLABLES (NUM_NODES+NUM_EDGES+NUM_MODELS)
#define NUM_STYLABLES_PROP (NUM_NODES_PROP+NUM_EDGES_PROP+NUM_MODELS_PROP)

#define STYLE_FILE "./test.xqsf"
#define NET_FILE "./test.xdsl"
#define DEFAULT_CLASS "defClass"
#define CUSTOM_CLASS "custClass"

class SlotMngr: public QObject{
    Q_OBJECT

public:
    int numSelected = 0;
    int numModified = 0;
    QNNodeItemList selNodes;
    QNModelItemList selModels;
    QNTextBoxItemList selTextboxes;
    QNEdgeItemList selEdges;

public Q_SLOTS:
    void sceneModified(){
        this->numModified++;
    }
    void registerNewItems(QNNodeItemList aNodeList, QNModelItemList aModelList,
                          QNTextBoxItemList aTextboxList, QNEdgeItemList aEdgeList){
        this->numSelected++;

        foreach(QNNodeItemPtr n, aNodeList){
            if(!this->selNodes.contains(n))
                this->selNodes.push_back(n);
        }

        foreach(QNModelItemPtr m, aModelList){
            if(!this->selModels.contains(m))
                this->selModels.push_back(m);
        }

        foreach(QNTextBoxItemPtr t, aTextboxList){
            if(!this->selTextboxes.contains(t))
                this->selTextboxes.push_back(t);
        }

        foreach (QNEdgeItemPtr e, aEdgeList) {
            if(!this->selEdges.contains(e))
                this->selEdges.push_back(e);
        }
    }
};

class QtNetsTest : public QObject{
    Q_OBJECT

private:
    ScopedQNNetwork net;
    QNNetworkModelPtr netModel;
    QNNetworkViewPtr view;
    QNNetworkScenePtr scene;
    QNPersistenceManagerPtr persistMgr;
    QNStyleManagerPtr styleMgr;

    QStringList nodeIds;
    QStringList edgeIds;
    QStringList modelIds;

    QActionPtr selectAllAction = nullptr;
    QActionPtr deleteAction = nullptr;
    QActionPtr toFrontAction = nullptr;
    QActionPtr sendBackAction = nullptr;
    QActionPtr newNodeAction = nullptr;
    QActionPtr newModelAction = nullptr;
    QActionPtr toggleShowContentAction = nullptr;
    QActionPtr newTextboxAction = nullptr;
    QActionPtr newEdgeAction = nullptr;
    QActionPtr newCicleAction = nullptr;

    int expectedModified = 0;
    int expectedSelected = 0;
    SlotMngr slot;

public:
    QtNetsTest(){
#ifdef QT_DEBUG
        this->persistMgr = new QNPersistenceManager(QDir("/home/angelo/Projects/QtNets/Projects/Plugin/SmilePersistencyPlugin/Debug/"));
        this->styleMgr = new QNStyleManager(QDir("/home/angelo/Projects/QtNets/Projects/Plugin/BaseStylePlugin/Debug/"));
#else
        QDir pluginDir = QDir(QCoreApplication::applicationDirPath());
        pluginDir.cd("Plugins");
        this->persistMgr = new QNPersistenceManager(pluginDir);
        this->styleMgr = new QNStyleManager(pluginDir);
#endif

        this->selectAllAction = new QAction(this);
        this->deleteAction = new QAction(this);
        this->toFrontAction = new QAction(this);
        this->sendBackAction = new QAction(this);
        this->newNodeAction = new QAction(this);
        this->newModelAction = new QAction(this);
        this->toggleShowContentAction = new QAction(this);
        this->newTextboxAction = new QAction(this);
        this->newEdgeAction = new QAction(this);
        this->newCicleAction = new QAction(this);
    }

    ~QtNetsTest(){
        this->net.reset();
        delete this->styleMgr;
        delete this->persistMgr;
        delete this->view;
    }

private Q_SLOTS:
    void networkTest(){
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

        //test if net is null
        QVERIFY2(!this->net.isNull(), "Network is null");
        QVERIFY2(this->net->getNetModel() != nullptr, "NetModel is null");

        //test net attributes
        QVERIFY2(this->net->getId() == NET_ID, "Network ID is not correct");
        QVERIFY2(this->net->getProperty("id").toString() == NET_ID, "Network ID property is not correct");
        QVERIFY2(this->net->getVersion() == NET_VERSION, "Network version is not correct");
        QVERIFY2(this->net->getProperty("version").toString() == NET_VERSION, "Network version property is not correct");
        QVERIFY2(this->netModel->getId() == NET_ID, "NetModel ID is not correct");
        QVERIFY2(this->netModel->getProperty("id").toString() == NET_ID, "NetModel ID property is not correct");
        QVERIFY2(this->netModel->getVersion() == NET_VERSION, "NetModel version is not correct");
        QVERIFY2(this->netModel->getProperty("version").toString() == NET_VERSION, "NetModel version property is not correct");

        //test net properties
        //prop1
        QVariant prop = this->net->getProperty(NET_PROP_1);
        QString err = QString("Net property %1 is not valid").arg(NET_PROP_1);
        QVERIFY2(prop.isValid(), err.toStdString().c_str());
        err = QString("Net property %1 is wrong").arg(NET_PROP_1);
        QVERIFY2(prop == NET_PROP_1_VALUE, err.toStdString().c_str());
        //prop2
        prop = this->net->getProperty(NET_PROP_2);
        err = QString("Net property %1 is not valid").arg(NET_PROP_2);
        QVERIFY2(prop.isValid(), err.toStdString().c_str());
        err = QString("Net property %1 is wrong").arg(NET_PROP_2);
        QVERIFY2(prop == NET_PROP_2_VALUE, err.toStdString().c_str());
        //prop3
        prop = this->net->getProperty(NET_PROP_3);
        err = QString("Net property %1 is not valid").arg(NET_PROP_3);
        QVERIFY2(prop.isValid(), err.toStdString().c_str());
        err = QString("Net property %1 is wrong").arg(NET_PROP_3);
        QVERIFY2(prop == NET_PROP_3_VALUE, err.toStdString().c_str());
        //prop4
        prop = this->net->getProperty(NET_PROP_4);
        err = QString("Net property %1 is present").arg(NET_PROP_4);
        QVERIFY2(!prop.isValid(), err.toStdString().c_str());

        //algparams
        qv = this->net->getProperty(SMILE_ALGPARAMS);
        QVERIFY2(qv.isValid(), "No algparams found");
        QVERIFY2(qv.canConvert<SmileAlgparams>(), "Algparams property is of the wrong type");
        SmileAlgparams algp = qv.value<SmileAlgparams>();
        if(origAlgP != algp){
            SmileEpisPtr epis = algp.getEpis();
            QVERIFY2(epis != nullptr, "epis is null");
            QVERIFY2(*epis == *(origEpis.data()), "epis is wrong");
        }

        //observationcost
        qv = this->net->getProperty(SMILE_OBSCOST);
        QVERIFY2(qv.isValid(), "No observationcost found");
        QVERIFY2(qv.canConvert<SmileObservationcost>(), "Observationcost property is of the wrong type");
        SmileObservationcost obscost = qv.value<SmileObservationcost>();
        if(origObsCost != obscost){
            SmileObscostNodeListPtr nodes = obscost.getNodes();
            QVERIFY2(nodes != nullptr, "observationcost nodes are null");
            QVERIFY2(nodes->size() == 2, "observationcost nodes are in wrong number");
            QVERIFY2(*nodes == origNodes, "observationcost nodes are wrong");
        }

        //genie
        qv = this->netModel->getProperty(SMILE_GENIE);
        QVERIFY2(qv.isValid(), "No genie found");
        QVERIFY2(qv.canConvert<SmileGenieExt>(), "Genie property is of the wrong type");
        SmileGenieExt genie = qv.value<SmileGenieExt>();
        QVERIFY2(genie.getName() == NET_ID, "Genie has a wrong name");
        QVERIFY2(genie.getVersion() == NET_VERSION, "Genie has a wrong version");
        QVERIFY2(genie.getComment() == COMMENT, "Genie has a wrong comment");
        QVERIFY2(genie.getFaultnameformat() == GenieFNF::nodestate, "Genie has a wrong faultnameformat");
        GenieDAFPtr daf = genie.getDiagautoformat();
        QVERIFY2(daf != nullptr, "No genie diagautoformat found");
        QVERIFY2(*daf == *origDaf, "genie diagautoformat is wrong");
    }

    void nodesTest(){
        const bool TARGET = (intRand()%2 > 0)? true: false;
        const bool RANKED = (intRand()%2 > 0)? true: false;
        const bool MANDATORY = (intRand()%2 > 0)? true: false;
        const DiagType DIAGTYPE = (DiagType)(intRand()%DiagType::auxiliary);
        const QString COMMENT = stringRand();
        const QString STATE_1 = idRand();
        const QString STATE_2 = idRand();
        const OptionalBool STATE_TARGET = (intRand()%2 > 0)? true: false;
        const OptionalBool DEFAULT = (intRand()%2 > 0)? true: false;
        const OptionalBool FAULT = (intRand()%2 > 0)? true: false;
        const QString FONT = stringRand();
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

        //test nodes
        QStringList n = this->net->getAllNodesKeys();
        n.sort();
        QVERIFY2(this->nodeIds == n, "nodes IDs are not correct");
        n = this->netModel->getAllIconsKeys();
        for(int i=0; i<NUM_NODES; i++){
            QString id = this->nodeIds.at(i);
            QNNodePtr node = this->net->getNode(id);
            QString err = QString("Node %1 missing").arg(id);
            QVERIFY2(node != nullptr, err.toStdString().c_str());
            QString type = node->getProperty(SMILE_NODE_TYPE).toString();
            bool correctType = (type == SMILE_NODES_MAU ||
                                type == SMILE_NODES_UTILITY ||
                                type == SMILE_NODES_DETERMINISTIC ||
                                type == SMILE_NODES_DECISION ||
                                type == SMILE_NODES_NOISYADDER ||
                                type == SMILE_NODES_NOISYMAX ||
                                type == SMILE_NODES_CPT);
            err = QString("Node %1 has a wrong type (%2)").arg(id).arg(type);
            QVERIFY2(correctType, err.toStdString().c_str());

            if(type == SMILE_NODES_DETERMINISTIC ||
               type == SMILE_NODES_DECISION ||
               type == SMILE_NODES_NOISYADDER ||
               type == SMILE_NODES_NOISYMAX ||
               type == SMILE_NODES_CPT){

                //check SmileNodeStateList
                QVariant sqv = node->getProperty(SMILE_NODE_STATE_LIST);
                err = QString("Node %1 missing state list").arg(id);
                QVERIFY2(sqv.canConvert<SmileNodeStateList>(), err.toStdString().c_str());
                SmileNodeStateList list = sqv.value<SmileNodeStateList>();
                err = QString("Node %1 state list, wrong number of items").arg(id);
                QVERIFY2(list.size() == 2, err.toStdString().c_str());
                if(list != states){
                    SmileNodeState state_1 = list.at(0);
                    err = QString("Node %1 state ok is wrong").arg(id);
                    QVERIFY2(state_1 == ok, err.toStdString().c_str());

                    SmileNodeState state_2 = list.at(1);
                    err = QString("Node %1 state ko is wrong").arg(id);
                    QVERIFY2(state_2 == ko, err.toStdString().c_str());
                }
            }

            //test common properties
            QVariant qv = node->getProperty(SMILE_NODES_TARGET);
            err = QString("Node %1 target missing").arg(id);
            QVERIFY2(qv.isValid(), err.toStdString().c_str());
            err = QString("Node %1 target is of the wrong type").arg(id);
            QVERIFY2(qv.canConvert(QMetaType::Bool), err.toStdString().c_str());
            bool targ = qv.toBool();
            err = QString("Node %1 target is wrong").arg(id);
            QVERIFY2(targ == TARGET, err.toStdString().c_str());

            qv = node->getProperty(SMILE_NODES_RANKED);
            err = QString("Node %1 ranked missing").arg(id);
            QVERIFY2(qv.isValid(), err.toStdString().c_str());
            err = QString("Node %1 ranked is of the wrong type").arg(id);
            QVERIFY2(qv.canConvert(QMetaType::Bool), err.toStdString().c_str());
            bool rank = qv.toBool();
            err = QString("Node %1 ranked is wrong").arg(id);
            QVERIFY2(rank == RANKED, err.toStdString().c_str());

            qv = node->getProperty(SMILE_NODES_MANDATORY);
            err = QString("Node %1 mandatory missing").arg(id);
            QVERIFY2(qv.isValid(), err.toStdString().c_str());
            err = QString("Node %1 mandatory is of the wrong type").arg(id);
            QVERIFY2(qv.canConvert(QMetaType::Bool), err.toStdString().c_str());
            bool mand = qv.toBool();
            err = QString("Node %1 mandatory is wrong").arg(id);
            QVERIFY2(mand == MANDATORY, err.toStdString().c_str());

            qv = node->getProperty(SMILE_NODES_DIAGTYPE);
            err = QString("Node %1 diagtype missing").arg(id);
            QVERIFY2(qv.isValid(), err.toStdString().c_str());
            err = QString("Node %1 diagtype is of the wrong type").arg(id);
            QVERIFY2(qv.canConvert<DiagType>(), err.toStdString().c_str());
            DiagType diag = qv.value<DiagType>();
            err = QString("Node %1 diagtype is wrong").arg(id);
            QVERIFY2(diag == DIAGTYPE, err.toStdString().c_str());

            //test icon
            QNIconPtr icon = this->netModel->getIconG(id);
            err = QString("Icon %1 missing").arg(id);
            QVERIFY2(icon != nullptr, err.toStdString().c_str());
            QNNodeIconPtr nodeIcon = dynamic_cast<QNNodeIconPtr>(icon);
            err = QString("Icon %1 is not a node").arg(id);
            QVERIFY2(nodeIcon != nullptr, err.toStdString().c_str());
            err = QString("Node icon %1 ID is not correct").arg(id);
            QVERIFY2(nodeIcon->getId() == id, err.toStdString().c_str());
            err = QString("Node icon %1 ID property is not correct").arg(id);
            QVERIFY2(nodeIcon->getProperty("id") == id, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong color").arg(id);
            QVERIFY2(nodeIcon->getColor() == BACK_COL, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong color property").arg(id);
            QVERIFY2(nodeIcon->getProperty("color") == BACK_COL, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong outline color").arg(id);
            QVERIFY2(nodeIcon->getOutlineColor() == OUT_COL, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong outline color property").arg(id);
            QVERIFY2(nodeIcon->getProperty("outlineColor") == OUT_COL, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong outline width").arg(id);
            QVERIFY2(nodeIcon->getOutlineWidth() == WIDTH, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong outline width property").arg(id);
            QVERIFY2(nodeIcon->getProperty("outlineWidth") == WIDTH, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong comment").arg(id);
            QVERIFY2(nodeIcon->getComment() == COMMENT, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong comment property").arg(id);
            QVERIFY2(nodeIcon->getProperty("comment") == COMMENT, err.toStdString().c_str());
            err = QString("Node icon %1 has null font").arg(id);
            QVERIFY2(nodeIcon->getFont() != nullptr, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong font name").arg(id);
            QVERIFY2(nodeIcon->getFont()->getFontName() == FONT, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong font color").arg(id);
            QVERIFY2(nodeIcon->getFont()->getFontColor() == FONT_COL, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong font size").arg(id);
            QVERIFY2(nodeIcon->getFont()->getFontSize() == FONT_SIZE, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong font bold").arg(id);
            QVERIFY2(nodeIcon->getFont()->isBold() == BOLD, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong font italic").arg(id);
            QVERIFY2(nodeIcon->getFont()->isItalic() == ITALIC, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong font align").arg(id);
            QVERIFY2(nodeIcon->getFont()->getAlign() == ALIGN, err.toStdString().c_str());
            err = QString("Node icon %1 has null position").arg(id);
            QVERIFY2(nodeIcon->getPosition() != nullptr, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong top").arg(id);
            QVERIFY2(nodeIcon->getPosition()->getTop() == TOP, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong left").arg(id);
            QVERIFY2(nodeIcon->getPosition()->getLeft() == LEFT, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong bottom").arg(id);
            QVERIFY2(nodeIcon->getPosition()->getBottom() == BOTTOM, err.toStdString().c_str());
            err = QString("Node icon %1 has wrong right").arg(id);
            QVERIFY2(nodeIcon->getPosition()->getRight() == RIGHT, err.toStdString().c_str());

            //check GenieNodeStateList
            qv = nodeIcon->getProperty(SMILE_GENIE_NODE_STATE);
            err = QString("Node icon %1 missing genie node state list").arg(id);
            QVERIFY2(qv.canConvert<GenieNodeStateList>(), err.toStdString().c_str());
            GenieNodeStateList gList = qv.value<GenieNodeStateList>();
            err = QString("Node icon %1 genie node state list, wrong number of items").arg(id);
            QVERIFY2(gList.size() == genieStates.size(), err.toStdString().c_str());
            if(gList != genieStates){
                GenieNodeState gState_1 = gList.at(0);
                err = QString("Node icon %1 genie node state ok is wrong").arg(id);
                QVERIFY2(gState_1 == genieOK, err.toStdString().c_str());

                GenieNodeState gState_2 = gList.at(1);
                err = QString("Node icon %1 genie node state ko is wrong").arg(id);
                QVERIFY2(gState_2 == genieOK, err.toStdString().c_str());
            }

            //barchart
            qv = nodeIcon->getProperty(SMILE_GENIE_BARCHART);
            err = QString("Node icon %1 missing barchart").arg(id);
            QVERIFY2(qv.canConvert<GenieBarchart>(), err.toStdString().c_str());
            GenieBarchart b = qv.value<GenieBarchart>();
            err = QString("Node icon %1, wrong barchart").arg(id);
            QVERIFY2(b == bar, err.toStdString().c_str());

            //question + fnf
            qv = nodeIcon->getProperty(SMILE_GENIE_QUESTION);
            err = QString("Node icon %1 missing question").arg(id);
            QVERIFY2(qv.canConvert<QString>(), err.toStdString().c_str());
            QString q = qv.value<QString>();
            err = QString("Node icon %1, wrong question").arg(id);
            QVERIFY2(q == QUESTION, err.toStdString().c_str());
            qv = nodeIcon->getProperty(SMILE_GENIE_FAULTNAMEFORMAT);
            err = QString("Node icon %1 missing fnf").arg(id);
            QVERIFY2(qv.canConvert<GenieFNF>(), err.toStdString().c_str());
            GenieFNF fnf = qv.value<GenieFNF>();
            err = QString("Node icon %1, wrong fnf").arg(id);
            QVERIFY2(fnf == FNF, err.toStdString().c_str());

            //costcomments
            qv = nodeIcon->getProperty(SMILE_GENIE_COSTCOMMENT);
            err = QString("Node icon %1 missing genie costcomment list").arg(id);
            QVERIFY2(qv.canConvert<GenieTableCommentList>(), err.toStdString().c_str());
            GenieTableCommentList ccl = qv.value<GenieTableCommentList>();
            err = QString("Node icon %1 genie costcomment list, wrong number of items").arg(id);
            QVERIFY2(ccl.size() == costComments.size(), err.toStdString().c_str());
            if(ccl != costComments){
                GenieTableComment cc1 = ccl.at(0);
                err = QString("Node icon %1 costcomment 1 is wrong").arg(id);
                QVERIFY2(cc1 == tc4, err.toStdString().c_str());

                GenieTableComment cc2 = ccl.at(1);
                err = QString("Node icon %1 costcomment 2 is wrong").arg(id);
                QVERIFY2(cc2 == tc5, err.toStdString().c_str());

                GenieTableComment cc3 = ccl.at(2);
                err = QString("Node icon %1 costcomment 3 is wrong").arg(id);
                QVERIFY2(cc3 == tc6, err.toStdString().c_str());
            }

            //defcomments
            qv = nodeIcon->getProperty(SMILE_GENIE_DEFCOMMENT);
            err = QString("Node icon %1 missing genie defcomment list").arg(id);
            QVERIFY2(qv.canConvert<GenieTableCommentList>(), err.toStdString().c_str());
            GenieTableCommentList dcl = qv.value<GenieTableCommentList>();
            err = QString("Node icon %1 genie defcomment list, wrong number of items").arg(id);
            QVERIFY2(dcl.size() == defComments.size(), err.toStdString().c_str());
            if(dcl != defComments){
                GenieTableComment dc1 = dcl.at(0);
                err = QString("Node icon %1 defcomment 1 is wrong").arg(id);
                QVERIFY2(dc1 == tc1, err.toStdString().c_str());

                GenieTableComment dc2 = dcl.at(1);
                err = QString("Node icon %1 defcomment 2 is wrong").arg(id);
                QVERIFY2(dc2 == tc2, err.toStdString().c_str());

                GenieTableComment dc3 = dcl.at(2);
                err = QString("Node icon %1 defcomment 3 is wrong").arg(id);
                QVERIFY2(dc3 == tc3, err.toStdString().c_str());
            }

            //links
            qv = nodeIcon->getProperty(SMILE_GENIE_LINK);
            err = QString("Node icon %1 missing genie link list").arg(id);
            QVERIFY2(qv.canConvert<GenieLinkList>(), err.toStdString().c_str());
            GenieLinkList ll = qv.value<GenieLinkList>();
            err = QString("Node icon %1 genie link list, wrong number of items").arg(id);
            QVERIFY2(ll.size() == links.size(), err.toStdString().c_str());
            err = QString("Node icon %1 genie link list is wrong").arg(id);
            QVERIFY2(ll == links, err.toStdString().c_str());
        }
    }

    void nodePropertiesTest(){
        QString key = "key";
        QString value = stringRand();
        QString key2 = "key2";
        QString value2 = stringRand();

        QString id;

        //set node property
        for(int i=0; i<NUM_NODES; i++){
            id = this->nodeIds.at(i);
            QNNodePtr tmp = this->net->getNode(id);
            if(i < NUM_NODES_PROP){
                tmp->setProperty(key, value);
            }
            else if(i >= NUM_NODES_PROP && i < NUM_NODES_PROP+2){
                tmp->setProperty(key, value);
                tmp->setProperty(key2, value2);
            }
            else if(i >= NUM_NODES_PROP+2 && i < NUM_NODES_PROP+4){
                tmp->setProperty(key2, value2);
            }
        }

        //test node property
        QNNodeList nl_all = this->net->getNodesByProperties();
        QNIconList nl_all_icons = this->netModel->getIconsByProperties();
        QNPropertyList props;
        props.append(QNProperty(key, value));
        QNNodeList nl1 = this->net->getNodesByProperties(props);
        QNIconList nl1_icons = this->netModel->getIconsByProperties(props);
        props.append(QNProperty(key2, value2));
        QNNodeList nl_and = this->net->getNodesByProperties(props, QNFilterStrategy::AND);
        QNIconList nl_and_icons = this->netModel->getIconsByProperties(props, QNFilterStrategy::AND);
        QNNodeList nl_or = this->net->getNodesByProperties(props, QNFilterStrategy::OR);
        QNIconList nl_or_icons = this->netModel->getIconsByProperties(props, QNFilterStrategy::OR);

        for(int i=0; i<NUM_NODES; i++){
            id = this->nodeIds.at(i);
            QNNodePtr tmp = this->net->getNode(id);
            QNIconPtr tmp_icon = this->netModel->getIconG(id);
            QString err;
            if(i>=0 && i<NUM_NODES_PROP){
                err = QString("getNodesByProperties, ALL: Node %1 missing").arg(id);
                QVERIFY2(nl_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_all_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, 1 PROP: Node %1 missing").arg(id);
                QVERIFY2(nl1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl1_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, AND: Node %1 must not be present").arg(id);
                QVERIFY2(!nl_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!nl_and_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, OR: Node %1 missing").arg(id);
                QVERIFY2(nl_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_or_icons.contains(tmp_icon), err.toStdString().c_str());
            }
            else if(i>=NUM_NODES_PROP && i<NUM_NODES_PROP+2){
                err = QString("getNodesByProperties, ALL: Node %1 missing").arg(id);
                QVERIFY2(nl_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_all_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, 1 PROP: Node %1 missing").arg(id);
                QVERIFY2(nl1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl1_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, AND: Node %1 missing").arg(id);
                QVERIFY2(nl_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_and_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, OR: Node %1 missing").arg(id);
                QVERIFY2(nl_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_or_icons.contains(tmp_icon), err.toStdString().c_str());
            }
            else if(i>=NUM_NODES_PROP+2 && i<NUM_NODES_PROP+4){
                err = QString("getNodesByProperties, ALL: Node %1 missing").arg(id);
                QVERIFY2(nl_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_all_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, 1 PROP: Node %1 must not be present").arg(id);
                QVERIFY2(!nl1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!nl1_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, AND: Node %1 must not be present").arg(id);
                QVERIFY2(!nl_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!nl_and_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, OR: Node %1 missing").arg(id);
                QVERIFY2(nl_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_or_icons.contains(tmp_icon), err.toStdString().c_str());
            }
            else{
                err = QString("getNodesByProperties, ALL: Node %1 missing").arg(id);
                QVERIFY2(nl_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(nl_all_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, 1 PROP: Node %1 must not be present").arg(id);
                QVERIFY2(!nl1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!nl1_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, AND: Node %1 must not be present").arg(id);
                QVERIFY2(!nl_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!nl_and_icons.contains(tmp_icon), err.toStdString().c_str());

                err = QString("getNodesByProperties, OR: Node %1 must not be present").arg(id);
                QVERIFY2(!nl_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!nl_or_icons.contains(tmp_icon), err.toStdString().c_str());
            }
        }
    }

    void edgesTest(){
        //init edges
        for(int i=0; i<NUM_NODES; i++){
            QNNodePtr source = this->net->getNode(this->nodeIds.at(i));
            for(int j=0; j<NUM_NODES; j++){
                if(i != j){
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
                }
            }
        }
        this->edgeIds.sort();

        //test if all edges are there
        QStringList e = this->net->getAllEdgesKeys();
        e.sort();
        QVERIFY2(this->edgeIds == e, "edges IDs are not correct");
        e = this->netModel->getAllEdgesKeys();
        e.sort();
        QVERIFY2(this->edgeIds == e, "edgeStyles IDs are not correct");

        //test ancestor nodes
        QString id = this->nodeIds.at(0);
        QNNodeListPtr ancestors = this->net->getNode(id)->getAncestors();
        for(int i=1; i<NUM_NODES; i++){
            id = this->nodeIds.at(i);
            QNNodePtr tmp = this->net->getNode(id);
            QString err = QString("Ancestor node %1 missing").arg(id);
            QVERIFY2(ancestors->contains(tmp), err.toStdString().c_str());
        }

        //test successor nodes
        id = this->nodeIds.at(0);
        QNNodeListPtr successors = this->net->getNode(id)->getSuccessors();
        for(int i=1; i<NUM_NODES; i++){
            id = this->nodeIds.at(i);
            QNNodePtr tmp = this->net->getNode(id);
            QString err = QString("Successor node %1 missing").arg(id);
            QVERIFY2(successors->contains(tmp), err.toStdString().c_str());
        }
    }

    void modelsAndTextBoxesTest(){
        const QString COMMENT = stringRand();
        const QString FONT = stringRand();
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

        //test if all models are there
        QStringList m = this->netModel->getAllIconsKeys();
        for(int i=0; i<NUM_MODELS; i++){
            QString id = this->modelIds.at(i);
            QNIconPtr icon = this->netModel->getIconG(id);
            QString err = QString("Icon %1 missing").arg(id);
            err = QString("Icon %1 missing").arg(id);
            QVERIFY2(icon != nullptr, err.toStdString().c_str());

            QNModelPtr model = dynamic_cast<QNModelPtr>(icon);
            err = QString("Icon %1 is not a model").arg(id);
            QVERIFY2(model != nullptr, err.toStdString().c_str());
            err = QString("Model icon %1 missing").arg(id);
            QVERIFY2(m.contains(id), err.toStdString().c_str());
            err = QString("Model icon %1 ID is not correct").arg(id);
            QVERIFY2(model->getId() == id, err.toStdString().c_str());
            err = QString("Model icon %1 ID property is not correct").arg(id);
            QVERIFY2(model->getProperty("id") == id, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong color").arg(id);
            QVERIFY2(model->getColor() == BACK_COL, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong color property").arg(id);
            QVERIFY2(model->getProperty("color") == BACK_COL, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong outline color").arg(id);
            QVERIFY2(model->getOutlineColor() == OUT_COL, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong outline color property").arg(id);
            QVERIFY2(model->getProperty("outlineColor") == OUT_COL, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong outline width").arg(id);
            QVERIFY2(model->getOutlineWidth() == WIDTH, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong outline width property").arg(id);
            QVERIFY2(model->getProperty("outlineWidth") == WIDTH, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong comment").arg(id);
            QVERIFY2(model->getComment() == COMMENT, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong comment property").arg(id);
            QVERIFY2(model->getProperty("comment") == COMMENT, err.toStdString().c_str());
            err = QString("Model icon %1 has null font").arg(id);
            QVERIFY2(model->getFont() != nullptr, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong font name").arg(id);
            QVERIFY2(model->getFont()->getFontName() == FONT, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong font color").arg(id);
            QVERIFY2(model->getFont()->getFontColor() == FONT_COL, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong font size").arg(id);
            QVERIFY2(model->getFont()->getFontSize() == FONT_SIZE, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong font bold").arg(id);
            QVERIFY2(model->getFont()->isBold() == BOLD, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong font italic").arg(id);
            QVERIFY2(model->getFont()->isItalic() == ITALIC, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong font align").arg(id);
            QVERIFY2(model->getFont()->getAlign() == ALIGN, err.toStdString().c_str());
            err = QString("Model icon %1 has null position").arg(id);
            QVERIFY2(model->getPosition() != nullptr, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong top").arg(id);
            QVERIFY2(model->getPosition()->getTop() == TOP, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong left").arg(id);
            QVERIFY2(model->getPosition()->getLeft() == LEFT, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong bottom").arg(id);
            QVERIFY2(model->getPosition()->getBottom() == BOTTOM, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong right").arg(id);
            QVERIFY2(model->getPosition()->getRight() == RIGHT, err.toStdString().c_str());
            err = QString("Model icon %1 has null window").arg(id);
            QVERIFY2(model->getWindow() != nullptr, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong window top").arg(id);
            QVERIFY2(model->getWindow()->getTop() == TOP, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong window left").arg(id);
            QVERIFY2(model->getWindow()->getLeft() == LEFT, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong window bottom").arg(id);
            QVERIFY2(model->getWindow()->getBottom() == BOTTOM, err.toStdString().c_str());
            err = QString("Model icon %1 has wrong window right").arg(id);
            QVERIFY2(model->getWindow()->getRight() == RIGHT, err.toStdString().c_str());
        }

        //test if all textboxes are there
        QNTextBoxListPtr tbs = this->netModel->getTextboxes();
        QVERIFY2(tbs != nullptr, "textboxes list is null");
        int size = tbs->size();
        QVERIFY2(size == NUM_TEXTBOXES, "textboxes number is wrong");
        for(int i=0; i<size; i++){
            QNTextBoxPtr textB = tbs->at(i);
            QString err = QString("Textbox at %1 is null").arg(i);
            QVERIFY2(textB != nullptr, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong caption").arg(i);
            QVERIFY2(textB->getCaption() == "New Note " + QString::number(i+1), err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong caption property").arg(i);
            QVERIFY2(textB->getProperty("caption") == "New Note " + QString::number(i+1), err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong comment").arg(i);
            QVERIFY2(textB->getComment() == COMMENT, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong comment property").arg(i);
            QVERIFY2(textB->getProperty("comment") == COMMENT, err.toStdString().c_str());
            err = QString("Textbox at %1 has null font").arg(i);
            QVERIFY2(textB->getFont() != nullptr, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong font name").arg(i);
            QVERIFY2(textB->getFont()->getFontName() == FONT, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong font color").arg(i);
            QVERIFY2(textB->getFont()->getFontColor() == FONT_COL, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong font size").arg(i);
            QVERIFY2(textB->getFont()->getFontSize() == FONT_SIZE, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong font bold").arg(i);
            QVERIFY2(textB->getFont()->isBold() == BOLD, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong font italic").arg(i);
            QVERIFY2(textB->getFont()->isItalic() == ITALIC, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong font align").arg(i);
            QVERIFY2(textB->getFont()->getAlign() == ALIGN, err.toStdString().c_str());
            err = QString("Textbox at %1 has null position").arg(i);
            QVERIFY2(textB->getPosition() != nullptr, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong top").arg(i);
            QVERIFY2(textB->getPosition()->getTop() == TOP, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong left").arg(i);
            QVERIFY2(textB->getPosition()->getLeft() == LEFT, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong bottom").arg(i);
            QVERIFY2(textB->getPosition()->getBottom() == BOTTOM, err.toStdString().c_str());
            err = QString("Textbox at %1 has wrong right").arg(i);
            QVERIFY2(textB->getPosition()->getRight() == RIGHT, err.toStdString().c_str());
        }
    }

    void persistTest(){
        const QString NET_ID = this->net->getId();
        const QString NET_VERSION = this->net->getVersion();

        //store net
        this->persistMgr->store(NET_FILE, this->net);

        //reload and compare
        ScopedQNNetwork newNet;
        this->persistMgr->load(NET_FILE, newNet);

        //test if net is null
        QVERIFY2(!newNet.isNull(), "Loaded network is null");
        QVERIFY2(newNet->getNetModel() != nullptr, "Loaded netModel is null");

        //test net properties
        QVERIFY2(newNet->getId() == NET_ID, "Loaded network ID is not correct");
        QVERIFY2(newNet->getProperty("id").toString() == NET_ID, "Loaded network ID property is not correct");
        QVERIFY2(newNet->getVersion() == NET_VERSION, "Loaded network version is not correct");
        QVERIFY2(newNet->getProperty("version").toString() == NET_VERSION, "Loaded network version property is not correct");
        QVERIFY2(newNet->getNetModel()->getId() == NET_ID, "Loaded netModel ID is not correct");
        QVERIFY2(newNet->getNetModel()->getProperty("id").toString() == NET_ID, "Loaded netModel ID property is not correct");
        QVERIFY2(newNet->getNetModel()->getVersion() == NET_VERSION, "Loaded netModel version is not correct");
        QVERIFY2(newNet->getNetModel()->getProperty("version").toString() == NET_VERSION, "Loaded netModel version property is not correct");

        //check net content
        if(*(newNet.data()) != *(this->net.data())){
            QVERIFY2(newNet->getAllNodesKeys() == this->net->getAllNodesKeys(), "Loaded nodes are not correct");
            foreach (QString id, this->nodeIds) {
                QString err = QString("Loaded node %1 is not correct").arg(id);
                QVERIFY2(*(newNet->getNode(id)) == *(this->net->getNode(id)), err.toStdString().c_str());
            }

            QVERIFY2(newNet->getAllEdgesKeys() == this->net->getAllEdgesKeys(), "Loaded edges are not correct");
            foreach (QString id, this->edgeIds) {
                QString err = QString("Loaded edge %1 is not correct").arg(id);
                QVERIFY2(*(newNet->getEdge(id)) == *(this->net->getEdge(id)), err.toStdString().c_str());
            }

            QVERIFY2(newNet->getNetModel()->getAllIconsKeys() == this->netModel->getAllIconsKeys(), "Loaded icons are not correct");
            foreach (QString id, this->netModel->getAllIconsKeys()) {
                QString err = QString("Loaded icon %1 is not correct").arg(id);
                QVERIFY2(*(newNet->getNetModel()->getIconG(id)) == *(this->netModel->getIconG(id)), err.toStdString().c_str());
            }

            QVERIFY2(newNet->getNetModel()->getAllEdgesKeys() == this->netModel->getAllEdgesKeys(), "Loaded edge styles are not correct");
            foreach (QString id, this->netModel->getAllEdgesKeys()) {
                QString err = QString("Loaded edgeStyle %1 is not correct").arg(id);
                QVERIFY2(*(newNet->getNetModel()->getEdge(id)) == *(this->netModel->getEdge(id)), err.toStdString().c_str());
            }
        }
        QFile(NET_FILE).remove();
        newNet.reset();
    }

    void edgePropertiesTest(){
        QString key = "key";
        QString value = stringRand();
        QString key2 = "key2";
        QString value2 = stringRand();

        QString id;

        //set edge property
        for(int i=0; i<NUM_EDGES; i++){
            id = this->edgeIds.at(i);
            QNEdgePtr tmp = this->net->getEdge(id);
            if(i < NUM_EDGES_PROP){
                tmp->setProperty(key, value);
            }
            else if(i >= NUM_EDGES_PROP && i < NUM_EDGES_PROP+20){
                tmp->setProperty(key, value);
                tmp->setProperty(key2, value2);
            }
            else if(i >= NUM_EDGES_PROP+20 && i < NUM_EDGES_PROP+40){
                tmp->setProperty(key2, value2);
            }
        }

        //test edge property
        QNEdgeList el_all = this->net->getEdgesByProperties();
        QNEdgeStyleList el_all_styles = this->netModel->getEdgesByProperties();
        QNPropertyList props;
        props.append(QNProperty(key, value));
        QNEdgeList el1 = this->net->getEdgesByProperties(props);
        QNEdgeStyleList el1_styles = this->netModel->getEdgesByProperties(props);
        props.append(QNProperty(key2, value2));
        QNEdgeList el_and = this->net->getEdgesByProperties(props, QNFilterStrategy::AND);
        QNEdgeStyleList el_and_styles = this->netModel->getEdgesByProperties(props, QNFilterStrategy::AND);
        QNEdgeList el_or = this->net->getEdgesByProperties(props, QNFilterStrategy::OR);
        QNEdgeStyleList el_or_styles = this->netModel->getEdgesByProperties(props, QNFilterStrategy::OR);

        for(int i=0; i<NUM_EDGES; i++){
            id = this->edgeIds.at(i);
            QNEdgePtr tmp = this->net->getEdge(id);
            QNEdgeStylePtr tmp_style = this->netModel->getEdge(id);
            QString err;
            if(i>=0 && i<NUM_EDGES_PROP){
                err = QString("getEdgesByProperties, ALL: Edge %1 missing").arg(id);
                QVERIFY2(el_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_all_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, 1 PROP: Edge %1 missing").arg(id);
                QVERIFY2(el1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el1_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, AND: Edge %1 must not be present").arg(id);
                QVERIFY2(!el_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!el_and_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, OR: Edge %1 missing").arg(id);
                QVERIFY2(el_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_or_styles.contains(tmp_style), err.toStdString().c_str());
            }
            else if(i>=NUM_EDGES_PROP && i<NUM_EDGES_PROP+20){
                err = QString("getEdgesByProperties, ALL: Edge %1 missing").arg(id);
                QVERIFY2(el_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_all_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, 1 PROP: Edge %1 missing").arg(id);
                QVERIFY2(el1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el1_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, AND: Edge %1 missing").arg(id);
                QVERIFY2(el_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_and_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, OR: Edge %1 missing").arg(id);
                QVERIFY2(el_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_or_styles.contains(tmp_style), err.toStdString().c_str());
            }
            else if(i>=NUM_EDGES_PROP+20 && i<NUM_EDGES_PROP+40){
                err = QString("getEdgesByProperties, ALL: Edge %1 missing").arg(id);
                QVERIFY2(el_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_all_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, 1 PROP: Edge %1 must not be present").arg(id);
                QVERIFY2(!el1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!el1_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, AND: Edge %1 must not be present").arg(id);
                QVERIFY2(!el_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!el_and_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, OR: Edge %1 missing").arg(id);
                QVERIFY2(el_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_or_styles.contains(tmp_style), err.toStdString().c_str());
            }
            else{
                err = QString("getEdgesByProperties, ALL: Edge %1 missing").arg(id);
                QVERIFY2(el_all.contains(tmp), err.toStdString().c_str());
                QVERIFY2(el_all_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, 1 PROP: Edge %1 must not be present").arg(id);
                QVERIFY2(!el1.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!el1_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, AND: Edge %1 must not be present").arg(id);
                QVERIFY2(!el_and.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!el_and_styles.contains(tmp_style), err.toStdString().c_str());

                err = QString("getEdgesByProperties, OR: Edge %1 must not be present").arg(id);
                QVERIFY2(!el_or.contains(tmp), err.toStdString().c_str());
                QVERIFY2(!el_or_styles.contains(tmp_style), err.toStdString().c_str());
            }
        }
    }

    void stylableTest(){
        QString class1 = DEFAULT_CLASS;
        QString class2 = CUSTOM_CLASS;

        QStringList stylables;
        stylables += this->nodeIds;
        stylables += this->edgeIds;
        stylables += this->modelIds;

        QString id;

        //set style classes
        for(int i=0; i<NUM_STYLABLES; i++){
            id = stylables.at(i);
            QNIconPtr icon = this->netModel->getIconG(id);
            QNEdgeStylePtr edge = this->netModel->getEdge(id);
            if(i < NUM_STYLABLES_PROP){
                if(icon) icon->addClass(class1);
                if(edge) edge->addClass(class1);
            }
            else if(i >= NUM_STYLABLES_PROP && i < NUM_STYLABLES_PROP+20){
                if(icon) icon->addClass(class1);
                if(icon) icon->addClass(class2);
                if(edge) edge->addClass(class1);
                if(edge) edge->addClass(class2);
            }
            else if(i >= NUM_STYLABLES_PROP+20 && i < NUM_STYLABLES_PROP+40){
                if(icon) icon->addClass(class2);
                if(edge) edge->addClass(class2);
            }
        }

        //test stylables
        QNStylableList style_all = this->netModel->getStylablesByClass();
        QStringList classes(class1);
        QNStylableList style1 = this->netModel->getStylablesByClass(classes);
        classes.append(class2);
        QNStylableList style_and = this->netModel->getStylablesByClass(classes, QNFilterStrategy::AND);
        QNStylableList style_or = this->netModel->getStylablesByClass(classes, QNFilterStrategy::OR);

        for(int i=0; i<NUM_STYLABLES; i++){
            id = stylables.at(i);
            QNIconPtr icon = this->netModel->getIconG(id);
            QNEdgeStylePtr edge = this->netModel->getEdge(id);
            QString err;
            if(i>=0 && i<NUM_STYLABLES_PROP){
                err = QString("getStylablesByClass, ALL: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_all.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_all.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, 1 CLASS: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style1.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style1.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, AND: Stylable %1 must not be present").arg(id);
                if(icon) QVERIFY2(!style_and.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(!style_and.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, OR: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_or.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_or.contains(edge), err.toStdString().c_str());
            }
            else if(i>=NUM_STYLABLES_PROP && i<NUM_STYLABLES_PROP+20){
                err = QString("getStylablesByClass, ALL: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_all.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_all.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, 1 CLASS: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style1.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style1.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, AND: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_and.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_and.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, OR: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_or.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_or.contains(edge), err.toStdString().c_str());
            }
            else if(i>=NUM_STYLABLES_PROP+20 && i<NUM_STYLABLES_PROP+40){
                err = QString("getStylablesByClass, ALL: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_all.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_all.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, 1 CLASS: Stylable %1 must not be present").arg(id);
                if(icon) QVERIFY2(!style1.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(!style1.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, AND: Stylable %1 must not be present").arg(id);
                if(icon) QVERIFY2(!style_and.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(!style_and.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, OR: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_or.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_or.contains(edge), err.toStdString().c_str());
            }
            else{
                err = QString("getStylablesByClass, ALL: Stylable %1 missing").arg(id);
                if(icon) QVERIFY2(style_all.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(style_all.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, 1 CLASS: Stylable %1 must not be present").arg(id);
                if(icon) QVERIFY2(!style1.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(!style1.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, AND: Stylable %1 must not be present").arg(id);
                if(icon) QVERIFY2(!style_and.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(!style_and.contains(edge), err.toStdString().c_str());

                err = QString("getStylablesByClass, OR: Stylable %1 must not be present").arg(id);
                if(icon) QVERIFY2(!style_or.contains(icon), err.toStdString().c_str());
                if(edge) QVERIFY2(!style_or.contains(edge), err.toStdString().c_str());
            }
        }
    }

    void styleTest(){
        //init style
        const QString ID = idRand();
        const QString VERSION = QString::number(doubleRand()/10, 'f', 2);
        this->styleMgr->newStyle(STYLE_FILE, ID, VERSION);
        QNStylePtr style = this->styleMgr->getStyle();

        QNStyleClassPtr c = new QNStyleClass(DEFAULT_CLASS);
        c->setProperty(STYLE_CLASS_BACKGROUND_COLOR, STYLE_DEFAULT_BG_COLOR);
        c->setProperty(STYLE_CLASS_BACKGROUND_BRUSH, BSBrushStyle::solid);
        c->setProperty(STYLE_CLASS_OUTLINE_COLOR, STYLE_DEFAULT_FG_COLOR);
        c->setProperty(STYLE_CLASS_OUTLINE_WIDTH, STYLE_DEFAULT_WIDTH);
        c->setProperty(STYLE_CLASS_OUTLINE_LINE, BSLineStyle::solidLine);
        c->setProperty(STYLE_CLASS_OUTLINE_CAP, BSCapStyle::flatCap);
        c->setProperty(STYLE_CLASS_OUTLINE_JOIN, BSJoinStyle::miterJoin);
        c->setProperty(STYLE_CLASS_SHAPE, BSNodeShape::rectagle);
        c->setProperty(STYLE_CLASS_ARROW, BSArrowStyle::full);
        c->setProperty(STYLE_CLASS_FONT_NAME, QFont(STYLE_DEFAULT_FONT_NAME));
        c->setProperty(STYLE_CLASS_FONT_COLOR, STYLE_DEFAULT_FONT_COLOR);
        c->setProperty(STYLE_CLASS_FONT_SIZE, STYLE_DEFAULT_FONT_SIZE);
        c->setProperty(STYLE_CLASS_FONT_ALIGN, BSTextAlign::textLeft);
        c->setProperty(STYLE_CLASS_FONT_BOLD, false);
        c->setProperty(STYLE_CLASS_FONT_ITALIC, false);
        c = style->addClass(c);

        QNStyleClassPtr c2 = new QNStyleClass(CUSTOM_CLASS);
        c2->setProperty(STYLE_CLASS_BACKGROUND_COLOR, "FF0000");
        c2->setProperty(STYLE_CLASS_BACKGROUND_BRUSH, BSBrushStyle::dense5);
        c2->setProperty(STYLE_CLASS_OUTLINE_COLOR, "00FF00");
        c2->setProperty(STYLE_CLASS_OUTLINE_WIDTH, 3);
        c2->setProperty(STYLE_CLASS_OUTLINE_LINE, BSLineStyle::dashLine);
        c2->setProperty(STYLE_CLASS_OUTLINE_CAP, BSCapStyle::roundCap);
        c2->setProperty(STYLE_CLASS_OUTLINE_JOIN, BSJoinStyle::roundJoin);
        c2->setProperty(STYLE_CLASS_SHAPE, BSNodeShape::circle);
        c2->setProperty(STYLE_CLASS_ARROW, BSArrowStyle::empty);
        c2->setProperty(STYLE_CLASS_FONT_NAME, QFont("Monospace"));
        c2->setProperty(STYLE_CLASS_FONT_COLOR, "0000FF");
        c2->setProperty(STYLE_CLASS_FONT_SIZE, 12);
        c2->setProperty(STYLE_CLASS_FONT_ALIGN, BSTextAlign::textCenter);
        c2->setProperty(STYLE_CLASS_FONT_BOLD, true);
        c2->setProperty(STYLE_CLASS_FONT_ITALIC, true);
        c2 = style->addClass(c2);

        //test style
        QVERIFY2(style != nullptr, "Style object is null");
        QVERIFY2(style->getId() == ID, "Style ID is not correct");
        QVERIFY2(style->getVersion() == VERSION, "Style version is not correct");

        //test style classes
        QVERIFY2(style->getAllClassesNames().size() == 2, "Wrong number of classes");
        QStringList cl;
        cl.push_back(CUSTOM_CLASS); cl.push_back(DEFAULT_CLASS);
        QVERIFY2(style->getAllClassesNames() == cl, "Wrong classes' names");

        //check classes
        QNStyleClassPtr defClass = style->getClass(DEFAULT_CLASS);
        QVERIFY2(defClass != nullptr, "Default class is null");
        QVERIFY2(*defClass == *c, "Default class is wrong");

        QNStyleClassPtr custClass = style->getClass(CUSTOM_CLASS);
        QVERIFY2(custClass != nullptr, "Custom class is null");
        QVERIFY2(*custClass == *c2, "Custom class is wrong");
    }

    void stylePersistTest(){
        //save style to file
        QNStylePtr style = this->styleMgr->getStyle();
        const QString ID = style->getId();
        const QString VERSION = style->getVersion();
        this->styleMgr->store(STYLE_FILE);

        QNStyleManagerPtr newStyleMgr = nullptr;
#ifdef QT_DEBUG
        newStyleMgr = new QNStyleManager(QDir("/home/angelo/Projects/QtNets/Projects/Plugin/BaseStylePlugin/Debug/"));
#else
        QDir pluginDir = QDir(QCoreApplication::applicationDirPath());
        pluginDir.cd("Plugins");
        newStyleMgr = new QNStyleManager(pluginDir);
#endif

        //reload style and test
        newStyleMgr->load(STYLE_FILE);
        QNStylePtr newStyle = newStyleMgr->getStyle();
        QVERIFY2(newStyle != nullptr, "Loaded style object is null");
        if(!(*style == *newStyle)){
            //find error
            QVERIFY2(newStyle->getId() == ID, "Loaded style is not correct");
            QVERIFY2(newStyle->getVersion() == VERSION, "Loaded style version is not correct");

            QStringList classes = style->getAllClassesNames();
            foreach (QString cl, classes) {
                QNStyleClassPtr c = newStyle->getClass(cl);
                QString err = QString("Class %1 is null").arg(cl);
                QVERIFY2(c != nullptr, err.toStdString().c_str());

                QNStyleClassPtr original = style->getClass(cl);
                err = QString("Class %1 is not correct").arg(cl);
                QVERIFY2(*original == *c, err.toStdString().c_str());
            }
        }

        QFile(STYLE_FILE).remove();
        delete newStyleMgr;
    }

    void sceneTest(){
        //init
        this->scene = new QNNetworkScene(this->styleMgr, this);
        this->view = new QNNetworkView(this->scene, nullptr);
        this->scene->setNetwork(this->netModel);

        //init menus and actions
        connect(this->selectAllAction, SIGNAL(triggered()), this->scene, SLOT(selectAll()));
        connect(this->deleteAction, SIGNAL(triggered()), this->scene, SLOT(deleteItems()));
        connect(this->toFrontAction, SIGNAL(triggered()), this->scene, SLOT(bringToFront()));
        connect(this->sendBackAction, SIGNAL(triggered()), this->scene, SLOT(sendToBack()));
        connect(this->newNodeAction, SIGNAL(triggered()), this->scene, SLOT(newNode()));
        connect(this->newModelAction, SIGNAL(triggered()), this->scene, SLOT(newModel()));
        connect(this->toggleShowContentAction, SIGNAL(triggered()), this->scene, SLOT(toggleShowContent()));
        connect(this->newTextboxAction, SIGNAL(triggered()), this->scene, SLOT(newTextbox()));
        connect(this->newCicleAction, SIGNAL(triggered()), this->scene, SLOT(newCycle()));

        //test
        QVERIFY2(this->view != nullptr, "Network View is null");
        QVERIFY2(this->scene != nullptr, "Network Scene is null");
        QVERIFY2(this->view->scene() == this->scene, "Network View scene is not correct");

        //test number of elements
        int origNum = NUM_NODES + NUM_EDGES + NUM_MODELS + NUM_TEXTBOXES;
        QVERIFY2(this->scene->items().size() == origNum, "Network Scene has a wrong number of elements");

        //test selectAll() + selected signal
        this->scene->selectAll();
        QVERIFY2(this->scene->items().size() == this->scene->selectedItems().size(),
                 "Wrong number of selected elements [Must be all items]");

        //init slots
        connect(this->scene, SIGNAL(modified()), &slot, SLOT(sceneModified()));
        connect(this->scene, SIGNAL(itemsSelected(QNNodeItemList, QNModelItemList, QNTextBoxItemList, QNEdgeItemList)),
                &slot, SLOT(registerNewItems(QNNodeItemList, QNModelItemList, QNTextBoxItemList, QNEdgeItemList)));

        //test new node
        int num = this->scene->items().size();
        QPointF n1_topLeft = QPointF(100, 100); this->scene->setMousePos(n1_topLeft);
        this->scene->newNode(); this->expectedModified++; this->expectedSelected++;

        QPointF n2_topLeft = QPointF(300, 100); this->scene->setMousePos(n2_topLeft);
        this->newNodeAction->trigger(); this->expectedModified++; this->expectedSelected++;
        QVERIFY2(this->scene->items().size() == num+2, "Network Scene has a wrong number of elements after nodes insertion");
        QVERIFY2(this->slot.selNodes.size() == 2, "newNodes are not as many as expected");

        //test new textbox
        num = this->scene->items().size();
        QPointF t1_topLeft = QPointF(550, 600); this->scene->setMousePos(t1_topLeft);
        this->scene->newTextbox(); this->expectedModified++; this->expectedSelected++;
        QPointF t2_topLeft = QPointF(650, 600); this->scene->setMousePos(t2_topLeft);
        this->newTextboxAction->trigger(); this->expectedModified++; this->expectedSelected++;
        QVERIFY2(this->scene->items().size() == num+2, "Network Scene has a wrong number of elements after textboxes insertion");
        QVERIFY2(this->slot.selTextboxes.size() == 2, "newTextboxes are not as many as expected");

        //test new model
        num = this->scene->items().size();
        QPointF m1_topLeft = QPointF(-150, -300); this->scene->setMousePos(m1_topLeft);
        this->scene->newModel(); this->expectedModified++; this->expectedSelected++;
        QPointF m2_topLeft = QPointF(800, 800); this->scene->setMousePos(m2_topLeft);
        this->newModelAction->trigger(); this->expectedModified++; this->expectedSelected++;
        QVERIFY2(this->scene->items().size() == num+2, "Network Scene has a wrong number of elements after models insertion");
        QVERIFY2(this->slot.selModels.size() == 2, "newModels are not as many as expected");

        //test new edges + newCycle
        QNNodeItemPtr n1 = this->slot.selNodes.at(0);
        n1->setSelected(true);
        QNNodeItemPtr n2 = this->slot.selNodes.at(1);
        n2->setSelected(true);
        num = this->scene->items().size();
        this->scene->newCycle(); this->expectedModified+=2; this->expectedSelected+=2;
        QVERIFY2(this->scene->items().size() == num+2, "Network Scene has a wrong number of elements after edges insertion");
        QVERIFY2(this->slot.selTextboxes.size() == 2, "newEdges are not as many as expected");
        QVERIFY2(n1->getEdges()->size() == 2, "new node 1 has wrong number of edges");
        QVERIFY2(n2->getEdges()->size() == 2, "new node 2 has wrong number of edges");
    }

    void moveItemsTest(){
        QString err;

        //test move elements inside/outside
        //move n1 into m1
        QNModelItemPtr m1 = this->slot.selModels.at(0);
        this->scene->clearSelection();
        QNNodeItemPtr n1 = this->slot.selNodes.at(0); n1->setSelected(true);
        n1->setPos(m1->scenePos()); this->expectedModified++;
        this->scene->manageMoveInside(); this->expectedModified++;
        QNModelItemPtr parent1 = qgraphicsitem_cast<QNModelItemPtr>(n1->parentItem());
        QVERIFY2(parent1 != nullptr, "Node 1 should have a parent item");
        QVERIFY2(*parent1 == *m1, "Model 1 should be Node 1's parent");

        //move n2 into m1
        this->scene->clearSelection();
        QNNodeItemPtr n2 = this->slot.selNodes.at(1); n2->setSelected(true);
        n2->setPos(m1->scenePos()); this->expectedModified++;
        this->scene->manageMoveInside(); this->expectedModified++;
        QNModelItemPtr parent2 = qgraphicsitem_cast<QNModelItemPtr>(n2->parentItem());
        QVERIFY2(parent2 != nullptr, "Node 1 should have a parent item");
        QVERIFY2(*parent2 == *m1, "Model 1 should be Node 2's parent");
        QVERIFY2(*parent1 == *parent2, "Model 1 should be parent for both node 1 and 2");

        //check n1's edges
        QNEdgeItemListPtr edges = n1->getEdges();
        QVERIFY2(edges != nullptr, "Node 1 should have edges");
        int size = edges->size();
        for(int i=0; i<size; i++){
            QNEdgeItemPtr e = edges->at(i);
            QNModelItemPtr parent = qgraphicsitem_cast<QNModelItemPtr>(e->parentItem());
            err = QString("Node 1's edge at %1 has null parent item").arg(i);
            QVERIFY2(parent != nullptr, err.toStdString().c_str());
            err = QString("Node 1's edge at %1 has wrong parent item").arg(i);
            QVERIFY2(*parent == *m1, err.toStdString().c_str());
        }
        //check n2's edges
        edges = n2->getEdges();
        QVERIFY2(edges != nullptr, "Node 2 should have edges");
        size = edges->size();
        for(int i=0; i<size; i++){
            QNEdgeItemPtr e = edges->at(i);
            QNModelItemPtr parent = qgraphicsitem_cast<QNModelItemPtr>(e->parentItem());
            err = QString("Node 2's edge at %1 has null parent item").arg(i);
            QVERIFY2(parent != nullptr, err.toStdString().c_str());
            err = QString("Node 2's edge at %1 has wrong parent item").arg(i);
            QVERIFY2(*parent == *m1, err.toStdString().c_str());
        }

        //move n1 out of m1
        this->scene->clearSelection();
        n1->setSelected(true);
        n1->moveBy(500, 500); this->expectedModified++;
        this->scene->manageMoveInside(); this->expectedModified++;
        parent1 = qgraphicsitem_cast<QNModelItemPtr>(n1->parentItem());
        QVERIFY2(parent1 == nullptr, "Node 1 should NOT have a parent item");

        //check n1's edges
        edges = n1->getEdges();
        QVERIFY2(edges != nullptr, "Node 1 should have edges");
        size = edges->size();
        for(int i=0; i<size; i++){
            QNEdgeItemPtr e = edges->at(i);
            err = QString("Node 1's edge at %1 should have null parent item").arg(i);
            QVERIFY2(e->parentItem() == nullptr, err.toStdString().c_str());
        }
        //check n2's edges
        edges = n2->getEdges();
        QVERIFY2(edges != nullptr, "Node 2 should have edges");
        size = edges->size();
        for(int i=0; i<size; i++){
            QNEdgeItemPtr e = edges->at(i);
            err = QString("Node 2's edge at %1 should have null parent item").arg(i);
            QVERIFY2(e->parentItem() == nullptr, err.toStdString().c_str());
        }
    }

    void scaleTest(){
        QString err;
        //test scale
        const double SCALE_F0 = 0.0;
        const double SCALE_F1 = 1.0;
        const double SCALE_F2 = 2.0;
        const double SCALE_F05 = 0.5;
        QNTextBoxItemPtr t1 = this->slot.selTextboxes.at(0);
        int origW = t1->getWidth();
        int origH = t1->getHeight();
        t1->scale(SCALE_F0);
        QVERIFY2(t1->getWidth() == origW, "new textbox 1 has wrong width after scale");
        QVERIFY2(t1->getHeight() == origH, "new textbox 1 has wrong height after scale");
        t1->scale(SCALE_F1);
        QVERIFY2(t1->getWidth() == origW, "new textbox 1 has wrong width after scale");
        QVERIFY2(t1->getHeight() == origH, "new textbox 1 has wrong height after scale");

        t1->scale(SCALE_F2); this->expectedModified++;
        QVERIFY2(t1->getWidth() == origW*SCALE_F2, "new textbox 1 has wrong width after scale");
        QVERIFY2(t1->getHeight() == origH*SCALE_F2, "new textbox 1 has wrong height after scale");

        origW = t1->getWidth();
        origH = t1->getHeight();
        t1->scale(SCALE_F05); this->expectedModified++;
        QVERIFY2(t1->getWidth() == origW*SCALE_F05, "new textbox 1 has wrong width after scale");
        QVERIFY2(t1->getHeight() == origH*SCALE_F05, "new textbox 1 has wrong height after scale");
    }

    void deleteTest(){
        //test deleteItems
        this->scene->clearSelection();
        QNNodeItemPtr n1 = this->slot.selNodes.at(0); n1->setSelected(true);
        QNNodeItemPtr n2 = this->slot.selNodes.at(1);
        QNModelItemPtr m2 = this->slot.selModels.at(1); m2->setSelected(true);
        QNTextBoxItemPtr t1 = this->slot.selTextboxes.at(0); t1->setSelected(true);
        this->slot.selNodes.removeOne(n1); this->slot.selEdges.clear();
        this->slot.selModels.removeOne(m2); this->slot.selTextboxes.removeOne(t1);
        int num = this->scene->items().size();
        this->scene->deleteItems(); this->expectedModified++; this->expectedSelected++;
        int tmp = this->scene->items().size();
        QVERIFY2(tmp == num-5, "Network Scene has a wrong number of elements after node removed");
        QVERIFY2(n2->getEdges() == nullptr || n2->getEdges()->size() == 0, "new node 2 has wrong number of edges");

        //test delete all
        this->scene->selectAll(); this->expectedSelected++;
        this->scene->deleteItems(); this->expectedModified++; this->expectedSelected++;
        this->scene->clearSelection();
        this->slot.selEdges.clear();
        this->slot.selNodes.clear();
        this->slot.selModels.clear();
        this->slot.selTextboxes.clear();
        QVERIFY2(this->scene->items().size() == 0, "Wrong number of selected elements after delete all");
    }

    void bringBackAndFrontTest(){
        //test bringToBack/bringToFront
        this->scene->newTextbox(); this->expectedModified++; this->expectedSelected++;
        this->scene->newTextbox(); this->expectedModified++; this->expectedSelected++;
        this->scene->newTextbox(); this->expectedModified++; this->expectedSelected++;
        this->scene->newTextbox(); this->expectedModified++; this->expectedSelected++;
        QNTextBoxItemPtr t1 = this->slot.selTextboxes.at(0);

        this->scene->clearSelection();
        t1->setSelected(true);
        this->toFrontAction->trigger();
        double Z = t1->zValue();
        for(int i=1; i<4; i++){
            double z = this->slot.selTextboxes.at(i)->zValue();
            QVERIFY2(z < Z,"New textbox 1 is not in front of others");
        }

        this->scene->clearSelection();
        t1->setSelected(true);
        this->sendBackAction->trigger();
        Z = t1->zValue();
        for(int i=1; i<4; i++){
            double z = this->slot.selTextboxes.at(i)->zValue();
            QVERIFY2(z > Z,"New textbox 1 is not behind all others");
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->deleteItems(); this->expectedModified++; this->expectedSelected++;
        this->scene->clearSelection();
        this->slot.selEdges.clear();
        this->slot.selNodes.clear();
        this->slot.selModels.clear();
        this->slot.selTextboxes.clear();
    }

    void showHideContentTest(){
        QString err;

        //populate scene
        this->scene->setMousePos(QPointF(-700, -200)); this->scene->newModel();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(-500, -500)); this->scene->newModel();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(-150, -300)); this->scene->newNode();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(0, 0)); this->scene->newNode();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(150, 300)); this->scene->newTextbox();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(500, 500)); this->scene->newTextbox();
        this->expectedModified++; this->expectedSelected++;

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->hideAllContents(); this->expectedModified+=6;
        foreach (QNNodeItemPtr n, this->slot.selNodes) {
            err = QString("Node %1 is showing content").arg(n->getNode()->getId());
            QVERIFY2(!(n->isShowingContent()), err.toStdString().c_str());
        }
        foreach (QNModelItemPtr m, this->slot.selModels) {
            err = QString("Model %1 is showing content").arg(m->getModel()->getId());
            QVERIFY2(!(m->isShowingContent()), err.toStdString().c_str());
        }
        foreach (QNTextBoxItemPtr t, this->slot.selTextboxes) {
            err = QString("Textbox %1 is showing content").arg(t->getTextbox()->getCaption());
            QVERIFY2(!(t->isShowingContent()), err.toStdString().c_str());
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->showAllContents(); this->expectedModified+=6;
        foreach (QNNodeItemPtr n, this->slot.selNodes) {
            err = QString("Node %1 is hidden").arg(n->getNode()->getId());
            QVERIFY2(n->isShowingContent(), err.toStdString().c_str());
        }
        foreach (QNModelItemPtr m, this->slot.selModels) {
            err = QString("Model %1 is hidden").arg(m->getModel()->getId());
            QVERIFY2(m->isShowingContent(), err.toStdString().c_str());
        }
        foreach (QNTextBoxItemPtr t, this->slot.selTextboxes) {
            err = QString("Textbox %1 is hidden").arg(t->getTextbox()->getCaption());
            QVERIFY2(t->isShowingContent(), err.toStdString().c_str());
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->toggleShowContentAction->trigger(); this->expectedModified+=6;
        foreach (QNNodeItemPtr n, this->slot.selNodes) {
            err = QString("Node %1 is showing content after toggle").arg(n->getNode()->getId());
            QVERIFY2(!(n->isShowingContent()), err.toStdString().c_str());
        }
        foreach (QNModelItemPtr m, this->slot.selModels) {
            err = QString("Model %1 is showing content after toggle").arg(m->getModel()->getId());
            QVERIFY2(!(m->isShowingContent()), err.toStdString().c_str());
        }
        foreach (QNTextBoxItemPtr t, this->slot.selTextboxes) {
            err = QString("Textbox %1 is showing content after toggle").arg(t->getTextbox()->getCaption());
            QVERIFY2(!(t->isShowingContent()), err.toStdString().c_str());
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->deleteItems(); this->expectedModified++; this->expectedSelected++;
        this->scene->clearSelection();
        this->slot.selEdges.clear();
        this->slot.selNodes.clear();
        this->slot.selModels.clear();
        this->slot.selTextboxes.clear();
    }

    void showHideContentInModelTest(){
        QString err;

        //populate scene
        this->scene->setMousePos(QPointF(-150, -300)); this->scene->newModel();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(-150, -300)); this->scene->newNode();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(-150, -300)); this->scene->newNode();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(-150, -300)); this->scene->newTextbox();
        this->expectedModified++; this->expectedSelected++;
        this->scene->setMousePos(QPointF(-150, -300)); this->scene->newTextbox();
        this->expectedModified++; this->expectedSelected++;

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->hideAllContents(); this->expectedModified++;
        foreach (QNNodeItemPtr n, this->slot.selNodes) {
            err = QString("Node %1 is hidden inside a model").arg(n->getNode()->getId());
            QVERIFY2(n->isShowingContent(), err.toStdString().c_str());
        }
        foreach (QNModelItemPtr m, this->slot.selModels) {
            err = QString("Model %1 is showing content").arg(m->getModel()->getId());
            QVERIFY2(!(m->isShowingContent()), err.toStdString().c_str());
        }
        foreach (QNTextBoxItemPtr t, this->slot.selTextboxes) {
            err = QString("Textbox %1 is hidden inside a model").arg(t->getTextbox()->getCaption());
            QVERIFY2(t->isShowingContent(), err.toStdString().c_str());
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->showAllContents(); this->expectedModified++;
        foreach (QNNodeItemPtr n, this->slot.selNodes) {
            err = QString("Node %1 is hidden").arg(n->getNode()->getId());
            QVERIFY2(n->isShowingContent(), err.toStdString().c_str());
        }
        foreach (QNModelItemPtr m, this->slot.selModels) {
            err = QString("Model %1 is hidden").arg(m->getModel()->getId());
            QVERIFY2(m->isShowingContent(), err.toStdString().c_str());
        }
        foreach (QNTextBoxItemPtr t, this->slot.selTextboxes) {
            err = QString("Textbox %1 is hidden").arg(t->getTextbox()->getCaption());
            QVERIFY2(t->isShowingContent(), err.toStdString().c_str());
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->toggleShowContentAction->trigger(); this->expectedModified++;
        foreach (QNNodeItemPtr n, this->slot.selNodes) {
            err = QString("Node %1 is hidden inside a model after toggle").arg(n->getNode()->getId());
            QVERIFY2(n->isShowingContent(), err.toStdString().c_str());
        }
        foreach (QNModelItemPtr m, this->slot.selModels) {
            err = QString("Model %1 is showing content after toggle").arg(m->getModel()->getId());
            QVERIFY2(!(m->isShowingContent()), err.toStdString().c_str());
        }
        foreach (QNTextBoxItemPtr t, this->slot.selTextboxes) {
            err = QString("Textbox %1 is hidden inside a model after toggle").arg(t->getTextbox()->getCaption());
            QVERIFY2(t->isShowingContent(), err.toStdString().c_str());
        }

        this->scene->selectAll(); this->expectedSelected++;
        this->scene->deleteItems(); this->expectedModified++; this->expectedSelected++;
        this->scene->clearSelection();
        this->slot.selEdges.clear();
        this->slot.selNodes.clear();
        this->slot.selModels.clear();
        this->slot.selTextboxes.clear();
    }

    void signalsTest(){
        //test modified and selected signal signal
        QString err = QString("modified signal: emitted %1 times, expected %2 times").arg(this->slot.numModified).arg(this->expectedModified);
        QVERIFY2(this->slot.numModified == this->expectedModified, err.toStdString().c_str());
        err = QString("itemsSelected signal: emitted %1 times, expected %2 times").arg(this->slot.numSelected).arg(this->expectedSelected);
        QVERIFY2(this->slot.numSelected == this->expectedSelected, err.toStdString().c_str());
    }
};
QTEST_MAIN(QtNetsTest)

#include "tst_qtnetstest.moc"
