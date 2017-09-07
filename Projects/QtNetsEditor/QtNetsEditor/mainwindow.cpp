/**
 ** @file mainwindow.cpp
 ** @author Angelo Prudentino
 ** @date 2017-01-11
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

#include "mainwindow.h"
#include "qnnetwork.h"
#include "qnnetworkmodel.h"
#include "qnnodeicon.h"
#include "qnedgestyle.h"
#include "qnnetworkmodel.h"
#include "qntextboxitem.h"
#include "qnnodeitem.h"
#include "qntextbox.h"
#include "qnmodel.h"
#include "qnmodelitem.h"
#include "qnedgeitem.h"
#include "basestyletypes.h"
#include "qnstyle.h"
#include "qnstyleclass.h"
#include "basestyleconst.h"
#include <qmath.h>
#include <QtGlobal>
#include <QDir>
#include <QMenuBar>
#include <QFileDialog>
#include <QIcon>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QStandardPaths>
#include <QCheckBox>
#include <QScrollBar>
#include <QLayout>
#include <QApplication>


#define Z_FACTOR 10
#define ZOOM_TIMES 100
#define R_FACTOR 10
#define R_MAX 360
#define R_MIN -360
#define WHITE_IDX 145
#define BLACK_IDX 8

void MainWindow::initActions(){
    //open
    this->openAction = new QAction(QIcon::fromTheme("document-open"), tr("&Open"), this);
    this->openAction->setShortcut(QKeySequence::Open);
    this->openAction->setToolTip(tr("Open a file"));
    connect(this->openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    //reload
    this->reloadAction = new QAction(QIcon::fromTheme("view-refresh"), tr("&Reload"), this);
    this->reloadAction->setShortcut(QKeySequence::Refresh);
    this->reloadAction->setToolTip(tr("Reload from disk"));
    connect(this->reloadAction, SIGNAL(triggered()), this, SLOT(reload()));

    //save
    this->saveAction = new QAction(QIcon::fromTheme("document-save"), tr("&Save"), this);
    this->saveAction->setShortcut(QKeySequence::Save);
    this->saveAction->setToolTip(tr("Save the current network"));
    connect(this->saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    //saveAs
    this->saveAsAction = new QAction(QIcon::fromTheme("document-save-as"), tr("Save&As"), this);
    this->saveAsAction->setShortcut(QKeySequence::SaveAs);
    this->saveAsAction->setToolTip(tr("Save the current network on an other file"));
    connect(this->saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    //close
    this->closeAction = new QAction(QIcon::fromTheme("window-close"), tr("&Close"), this);
    this->closeAction->setShortcut(QKeySequence::Close);
    this->closeAction->setToolTip(tr("Close the current network"));
    connect(this->closeAction, SIGNAL(triggered()), this, SLOT(closeFile()));

    //export
    this->exportAction = new QAction(QIcon::fromTheme("image-x-generic"), tr("&Export"), this);
    this->exportAction->setShortcut(tr("Ctrl+E"));
    this->exportAction->setToolTip(tr("Export the current network as image"));
    connect(this->exportAction, SIGNAL(triggered()), this, SLOT(exportAs()));

    //print
    this->printAction = new QAction(QIcon::fromTheme("document-print"), tr("&Print"), this);
    this->printAction->setShortcut(QKeySequence::Print);
    this->printAction->setToolTip(tr("Print the current network"));
    connect(this->printAction, SIGNAL(triggered()), this, SLOT(print()));

    //exit
    this->exitAction = new QAction(QIcon::fromTheme("application-exit"), tr("E&xit"), this);
    this->exitAction->setShortcut(QKeySequence::Quit);
    this->exitAction->setToolTip(tr("Quit QtNets Demo"));
    connect(this->exitAction, SIGNAL(triggered()), this, SLOT(close()));

    //restore zoom
    this->restoreZoomAction = new QAction(tr("Restore zoom"), this);
    this->restoreZoomAction->setShortcut(tr("Ctrl+0"));
    this->restoreZoomAction->setToolTip(tr("Restore the original zoom"));
    connect(this->restoreZoomAction, SIGNAL(triggered()), this->view, SLOT(restoreZoom()));

    //zoom-in
    this->zoomInAction = new QAction(QIcon::fromTheme("zoom-in"), tr("Zoom in"), this);
    this->zoomInAction->setShortcut(QKeySequence::ZoomIn);
    this->zoomInAction->setToolTip(tr("Zoom In"));
    connect(this->zoomInAction, SIGNAL(triggered()), this->view, SLOT(zoomIn()));

    //zoom-out
    this->zoomOutAction = new QAction(QIcon::fromTheme("zoom-out"), tr("Zoom out"), this);
    this->zoomOutAction->setShortcut(QKeySequence::ZoomOut);
    this->zoomOutAction->setToolTip(tr("Zoom Out"));
    connect(this->zoomOutAction, SIGNAL(triggered()), this->view, SLOT(zoomOut()));

    //restore rotate
    this->restoreRotateAction = new QAction(tr("Restore rotate"), this);
    this->restoreRotateAction->setShortcut(tr("Ctrl+Shift+0"));
    this->restoreRotateAction->setToolTip(tr("Restore the original orientation"));
    connect(this->restoreRotateAction, SIGNAL(triggered()), this->view, SLOT(restoreRotate()));

    //rotate left
    this->rotateLeftAction = new QAction(QIcon::fromTheme("object-rotate-left"), tr("Rotate left"), this);
    this->rotateLeftAction->setShortcut(QKeySequence::MoveToPreviousWord);
    this->rotateLeftAction->setToolTip(tr("Rotate view to left"));
    connect(this->rotateLeftAction, SIGNAL(triggered()), this->view, SLOT(rotateLeft()));

    //rotate right
    this->rotateRightAction = new QAction(QIcon::fromTheme("object-rotate-right"), tr("Rotate right"), this);
    this->rotateRightAction->setShortcut(QKeySequence::MoveToNextWord);
    this->rotateRightAction->setToolTip(tr("Rotate view to right"));
    connect(this->rotateRightAction, SIGNAL(triggered()), this->view, SLOT(rotateRight()));

    //rotate left
    this->rotateLeft90Action = new QAction(QIcon::fromTheme("object-rotate-left"), tr("Rotate left 90°"), this);
    this->rotateLeft90Action->setShortcut(QKeySequence::SelectPreviousWord);
    this->rotateLeft90Action->setToolTip(tr("Rotate view 90° to left"));
    connect(this->rotateLeft90Action, SIGNAL(triggered()), this, SLOT(rotateLeft90()));

    //rotate right
    this->rotateRight90Action = new QAction(QIcon::fromTheme("object-rotate-right"), tr("Rotate right 90°"), this);
    this->rotateRight90Action->setShortcut(QKeySequence::SelectNextWord);
    this->rotateRight90Action->setToolTip(tr("Rotate view 90° to right"));
    connect(this->rotateRight90Action, SIGNAL(triggered()), this, SLOT(rotateRight90()));

    //select all
    this->selectAllAction = new QAction(tr("Select &All"), this);
    this->selectAllAction->setShortcut(QKeySequence::SelectAll);
    this->selectAllAction->setToolTip(tr("Select all items in the scene"));
    connect(this->selectAllAction, SIGNAL(triggered()), this->scene, SLOT(selectAll()));

    //remeber on delete
    this->rememberOnDeleteAction = new QAction(tr("Do not ask for confirm on delete"), this);
    this->rememberOnDeleteAction->setCheckable(true);
    this->rememberOnDeleteAction->setChecked(this->toRemeber);
    this->rememberOnDeleteAction->setToolTip(tr("Do not ask for confirm on delete"));
    connect(this->rememberOnDeleteAction, SIGNAL(triggered()), this, SLOT(rememberOnDelete()));

    //delete items
    this->deleteAction = new QAction(QIcon::fromTheme("edit-delete"), tr("&Delete"), this);
    this->deleteAction->setShortcut(QKeySequence::Delete);
    this->deleteAction->setToolTip(tr("Delete selected"));
    connect(this->deleteAction, SIGNAL(triggered()), this, SLOT(deleteItems()));

    //bring to front items
    this->toFrontAction = new QAction(tr("Bring to &Front"), this);
    this->toFrontAction->setShortcut(tr("Ctrl+F"));
    this->toFrontAction->setToolTip(tr("Bring items to front"));
    connect(this->toFrontAction, SIGNAL(triggered()), this->scene, SLOT(bringToFront()));

    //send back items
    this->sendBackAction = new QAction(tr("Send to &Back"), this);
    this->sendBackAction->setShortcut(tr("Ctrl+B"));
    this->sendBackAction->setToolTip(tr("Send items to back"));
    connect(this->sendBackAction, SIGNAL(triggered()), this->scene, SLOT(sendToBack()));

    //new node
    this->newNodeAction = new QAction(QIcon::fromTheme("list-add"), tr("New Node"), this);
    this->newNodeAction->setToolTip(tr("Insert a new node in the scene"));
    connect(this->newNodeAction, SIGNAL(triggered()), this->scene, SLOT(newNode()));

    //new model
    this->newModelAction = new QAction(QIcon::fromTheme("list-add"), tr("New Model"), this);
    this->newModelAction->setToolTip(tr("Insert a new model in the scene"));
    connect(this->newModelAction, SIGNAL(triggered()), this->scene, SLOT(newModel()));

    //show/hide content
    this->toggleContentAction = new QAction(tr("Show/Hide details"), this);
    this->toggleContentAction->setShortcut(tr("Ctrl+H"));
    this->toggleContentAction->setToolTip(tr("show/hide selected items' details"));
    connect(this->toggleContentAction, SIGNAL(triggered()), this->scene, SLOT(toggleShowContent()));

    //new textbox
    this->newTextboxAction = new QAction(QIcon::fromTheme("list-add"), tr("New Textbox"), this);
    this->newTextboxAction->setToolTip(tr("Insert a new textbox in the scene"));
    connect(this->newTextboxAction, SIGNAL(triggered()), this->scene, SLOT(newTextbox()));

    //new edge
    this->newEdgeAction = new QAction(QIcon::fromTheme("list-add"), tr("New Edge"), this);
    this->newEdgeAction->setToolTip(tr("Enter in draw edges mode"));
    connect(this->newEdgeAction, SIGNAL(triggered()), this, SLOT(newEdge()));

    //new cycle between selected nodes
    this->newCicleAction = new QAction(QIcon::fromTheme("list-add"), tr("New Cycle"), this);
    this->newCicleAction->setToolTip(tr("Build a cycle beteween selected nodes"));
    connect(this->newCicleAction, SIGNAL(triggered()), this->scene, SLOT(newCycle()));

    //remeber node as source
    this->rememberNodeAction = new QAction(tr("Remember node"), this);
    this->rememberNodeAction->setToolTip(tr("Remember this node as source for an edge"));
    connect(this->rememberNodeAction, SIGNAL(triggered()), this, SLOT(rememberNode()));

    //open style
    this->openStyleAction = new QAction(QIcon::fromTheme("document-open"), tr("&Load style"), this);
    this->openStyleAction->setToolTip(tr("Open a style sheet"));
    connect(this->openStyleAction, SIGNAL(triggered()), this, SLOT(openStyle()));

    //reload style
    this->reloadStyleAction = new QAction(QIcon::fromTheme("view-refresh"), tr("Reload Style"), this);
    this->reloadStyleAction->setToolTip(tr("Reload style"));
    connect(this->reloadStyleAction, SIGNAL(triggered()), this, SLOT(reloadStyle()));

    //save style
    this->saveStyleAction = new QAction(QIcon::fromTheme("document-save"), tr("Save Style"), this);
    this->saveStyleAction->setToolTip(tr("Save the current style"));
    connect(this->saveStyleAction, SIGNAL(triggered()), this, SLOT(saveStyle()));

    //saveAs style
    this->saveAsStyleAction = new QAction(QIcon::fromTheme("document-save-as"), tr("SaveAs Style"), this);
    this->saveAsStyleAction->setToolTip(tr("Save the current style on an other file"));
    connect(this->saveAsStyleAction, SIGNAL(triggered()), this, SLOT(saveAsStyle()));

    //close style
    this->closeStyleAction = new QAction(QIcon::fromTheme("window-close"), tr("Close Style"), this);
    this->closeStyleAction->setToolTip(tr("Close the current style"));
    connect(this->closeStyleAction, SIGNAL(triggered()), this, SLOT(closeStyle()));

    //about
    this->aboutAction = new QAction(QIcon::fromTheme("dialog-information"), tr("A&bout"), this);
    this->aboutAction->setShortcut(QKeySequence::HelpContents);
    this->aboutAction->setToolTip(tr("Show info"));
    connect(this->aboutAction, SIGNAL(triggered()), this, SLOT(showAbout()));
}

void MainWindow::initMenus(){
    this->initActions();

    this->fileMenu = menuBar()->addMenu(tr("&File"));
    this->fileMenu->addAction(this->openAction);
    this->fileMenu->addAction(this->reloadAction);
    this->fileMenu->addAction(this->saveAction);
    this->fileMenu->addAction(this->saveAsAction);
    this->fileMenu->addAction(this->exportAction);
    this->fileMenu->addAction(this->printAction);
    this->fileMenu->addAction(this->closeAction);
    this->fileMenu->addSeparator();
    this->fileMenu->addAction(this->exitAction);

    this->viewMenu = menuBar()->addMenu(tr("&View"));
    this->viewMenu->addAction(this->itemPropDock->toggleViewAction());
    this->viewMenu->addAction(this->styleEditDock->toggleViewAction());
    this->viewMenu->addSeparator();
    this->viewMenu->addAction(this->zoomInAction);
    this->viewMenu->addAction(this->zoomOutAction);
    this->viewMenu->addAction(this->restoreZoomAction);
    this->viewMenu->addSeparator();
    this->viewMenu->addAction(this->rotateLeftAction);
    this->viewMenu->addAction(this->rotateLeft90Action);
    this->viewMenu->addAction(this->rotateRightAction);
    this->viewMenu->addAction(this->rotateRight90Action);
    this->viewMenu->addAction(this->restoreRotateAction);

    this->itemsMenu = menuBar()->addMenu(tr("&Items"));
    this->itemsMenu->addAction(this->selectAllAction);
    this->itemsMenu->addAction(this->toggleContentAction);
    this->itemsMenu->addAction(this->toFrontAction);
    this->itemsMenu->addAction(this->sendBackAction);
    this->itemsMenu->addAction(this->deleteAction);
    this->itemsMenu->addSeparator();
    this->itemsMenu->addAction(this->newNodeAction);
    this->itemsMenu->addAction(this->newModelAction);
    this->itemsMenu->addAction(this->newTextboxAction);
    this->itemsMenu->addAction(this->newEdgeAction);

    this->sceneMenu = new QMenu(this);
    this->sceneMenu->addAction(this->newNodeAction);
    this->sceneMenu->addAction(this->newModelAction);
    this->sceneMenu->addAction(this->newTextboxAction);
    this->sceneMenu->addAction(this->newEdgeAction);

    this->nodeMenu = new QMenu(this);
    this->nodeMenu->addAction(this->toFrontAction);
    this->nodeMenu->addAction(this->sendBackAction);
    this->nodeMenu->addAction(this->deleteAction);
    this->nodeMenu->addAction(this->rememberNodeAction);
    this->nodeMenu->addAction(this->newEdgeAction);

    this->manyNodesMenu = new QMenu(this);
    this->manyNodesMenu->addAction(this->toggleContentAction);
    this->manyNodesMenu->addAction(this->toFrontAction);
    this->manyNodesMenu->addAction(this->sendBackAction);
    this->manyNodesMenu->addAction(this->deleteAction);
    this->manyNodesMenu->addAction(this->newCicleAction);

    this->edgeMenu = new QMenu(this);
    this->edgeMenu->addAction(this->toFrontAction);
    this->edgeMenu->addAction(this->sendBackAction);
    this->edgeMenu->addAction(this->deleteAction);

    this->modelMenu = new QMenu(this);
    this->modelMenu->addAction(this->toFrontAction);
    this->modelMenu->addAction(this->sendBackAction);
    this->modelMenu->addAction(this->deleteAction);
    this->modelMenu->addSeparator();
    this->modelMenu->addAction(this->newNodeAction);
    this->modelMenu->addAction(this->newModelAction);
    this->modelMenu->addAction(this->newTextboxAction);
    this->modelMenu->addAction(this->newEdgeAction);

    this->manyModelsMenu = new QMenu(this);
    this->manyModelsMenu->addAction(this->toggleContentAction);
    this->manyModelsMenu->addAction(this->toFrontAction);
    this->manyModelsMenu->addAction(this->sendBackAction);
    this->manyModelsMenu->addAction(this->deleteAction);
    this->manyModelsMenu->addSeparator();
    this->manyModelsMenu->addAction(this->newNodeAction);
    this->manyModelsMenu->addAction(this->newModelAction);
    this->manyModelsMenu->addAction(this->newTextboxAction);
    this->manyModelsMenu->addAction(this->newEdgeAction);

    this->textboxMenu = new QMenu(this);
    this->textboxMenu->addAction(this->toFrontAction);
    this->textboxMenu->addAction(this->sendBackAction);
    this->textboxMenu->addAction(this->deleteAction);

    this->manyMenu = new QMenu(this);
    this->manyMenu->addAction(this->toggleContentAction);
    this->manyMenu->addAction(this->toFrontAction);
    this->manyMenu->addAction(this->sendBackAction);
    this->manyMenu->addAction(this->deleteAction);

    this->styleMenu = menuBar()->addMenu(tr("St&yle"));
    this->styleMenu->addAction(this->openStyleAction);
    this->styleMenu->addAction(this->reloadStyleAction);
    this->styleMenu->addAction(this->saveStyleAction);
    this->styleMenu->addAction(this->saveAsStyleAction);
    this->styleMenu->addAction(this->closeStyleAction);

    this->settingsMenu = menuBar()->addMenu(tr("Se&ttings"));
    this->settingsMenu->addAction(this->rememberOnDeleteAction);

    this->helpMenu = menuBar()->addMenu(tr("&Help"));
    this->helpMenu->addAction(this->aboutAction);
}

void MainWindow::initDocks(){
    //Editor dock
    this->itemPropDock = new QDockWidget(tr("Items editor"), this);
    this->itemPropDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->itemPropWidget = new QWidget(this->itemPropDock);
    this->itemPropLayout = new QFormLayout(this->itemPropWidget);
    this->itemPropLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    this->itemPropWidget->setLayout(this->itemPropLayout);
    this->initItemEditor();
    this->itemPropDock->setWidget(this->itemPropWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, this->itemPropDock);

    //Style dock
    this->styleEditDock = new QDockWidget(tr("Style editor"), this);
    this->styleEditDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    this->styleEditorWidget = new QWidget(this->styleEditDock);
    this->styleEditLayout = new QFormLayout(this->styleEditorWidget);
    this->styleEditLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    this->styleEditorWidget->setLayout(this->styleEditLayout);
    this->initStyleEditor();
    this->styleEditDock->setWidget(this->styleEditorWidget);
    this->addDockWidget(Qt::RightDockWidgetArea, this->styleEditDock);
    //    this->addDockWidget(Qt::LeftDockWidgetArea, this->styleEditDock);

    //    this->tabifyDockWidget(this->styleEditDock, this->itemPropDock);
}

void MainWindow::initItemEditor(){
    //general section
    QLabel* lbl = new QLabel("General", this->itemPropWidget);
    QFont f = lbl->font(); f.setBold(true); lbl->setFont(f);
    this->itemPropLayout->addRow(lbl);

    //property displayed
    this->propLabel = new QLabel("Property: ", this->itemPropWidget);
    this->propCombo = new QComboBox(this->itemPropWidget);
    connect(this->propCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updatePropCombo(int)));
    this->itemPropLayout->addRow(this->propLabel, this->propCombo);

    //label lenght
    this->lengthLabel = new QLabel("Content Length: ", this->itemPropWidget);
    this->lengthSpin = new QSpinBox(this->itemPropWidget);
    this->lengthSpin->setMinimum(-1);
    connect(this->lengthSpin, SIGNAL(valueChanged(int)), this, SLOT(updateLengths(int)));
    this->itemPropLayout->addRow(this->lengthLabel, this->lengthSpin);

    //font
    this->fontLabel = new QLabel("Font: ", this->itemPropWidget);
    this->fontLayout = new QGridLayout(this->itemPropWidget);
    this->fontLayout->setSpacing(6);

    this->fontCombo = new QFontComboBox(this->itemPropWidget);
    connect(this->fontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(updateFontNames(QFont)));
    this->fontLayout->addWidget(this->fontCombo, 1, 1, 1, 3);

    this->alignCombo = new QComboBox(this->itemPropWidget);
    this->alignCombo->insertItem(QNAlignment::undefAlign, "");
    this->alignCombo->insertItem(QNAlignment::leftAlign, "left");
    this->alignCombo->insertItem(QNAlignment::centerAlign, "center");
    this->alignCombo->insertItem(QNAlignment::rightAlign, "right");
    connect(this->alignCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateAlign(int)));
    this->fontLayout->addWidget(this->alignCombo, 2, 1, 1, 1);

    this->boldButton = new QPushButton(QIcon::fromTheme("format-text-bold"), tr(""), this->itemPropWidget);
    this->boldButton->setToolTip("Bold font");
    this->boldButton->setCheckable(true);
    connect(this->boldButton, SIGNAL(toggled(bool)), this, SLOT(updateFontBold(bool)));
    this->fontLayout->addWidget(this->boldButton, 2, 2, 1, 1);

    this->italicButton = new QPushButton(QIcon::fromTheme("format-text-italic"), tr(""), this->itemPropWidget);
    this->italicButton->setToolTip("Italic font");
    this->italicButton->setCheckable(true);
    connect(this->italicButton, SIGNAL(toggled(bool)), this, SLOT(updateFontItalic(bool)));
    this->fontLayout->addWidget(this->italicButton, 2, 3, 1, 1);

    this->fontColCombo = new QComboBox(this->itemPropWidget);
    this->colorNames.push_front("");
    for (int i = 0; i < this->colorNames.size(); i++) {
        QColor color(this->colorNames[i]);
        this->fontColCombo->insertItem(i, this->colorNames[i]);
        if(this->colorNames[i] != "")
            this->fontColCombo->setItemData(i, color, Qt::DecorationRole);
    }
    connect(this->fontColCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateFontColors(int)));
    this->fontLayout->addWidget(this->fontColCombo, 3, 1, 1, 2);

    this->fontSizeSpin = new QSpinBox(this->itemPropWidget);
    this->fontSizeSpin->setRange(0, 100);
    connect(this->fontSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(updateFontSizes(int)));
    this->fontLayout->addWidget(this->fontSizeSpin, 3, 3, 1, 1);

    this->itemPropLayout->addRow(this->fontLabel, this->fontLayout);

    //comment
    this->commentLabel = new QLabel("Comment: ", this->itemPropWidget);
    this->commentEdit = new QLineEdit(this->itemPropWidget);
    connect(this->commentEdit, SIGNAL(editingFinished()), this, SLOT(updateComments()));
    this->itemPropLayout->addRow(this->commentLabel, this->commentEdit);

    //nodes or models section
    this->itemPropLayout->addRow(new QLabel("", this->itemPropWidget));
    lbl = new QLabel("Nodes or Models", this->itemPropWidget);
    f = lbl->font(); f.setBold(true); lbl->setFont(f);
    this->itemPropLayout->addRow(lbl);

    //id
    this->idLabel = new QLabel("Id: ", this->itemPropWidget);
    this->idLayout = new QGridLayout(this->itemPropWidget);
    this->idLayout->setHorizontalSpacing(6);

    this->idEdit = new QLineEdit(this->itemPropWidget);
    connect(this->idEdit, SIGNAL(editingFinished()), this, SLOT(updateId()));
    this->idLayout->addWidget(this->idEdit, 1, 1, 1, 2);

    this->randIdButton = new QPushButton(QIcon::fromTheme("view-refresh"), tr(""), this->itemPropWidget);
    this->randIdButton->setToolTip("Click to randomly generate a new ID");
    connect(this->randIdButton, SIGNAL(clicked(bool)), this, SLOT(genRandId(bool)));
    this->idLayout->addWidget(this->randIdButton, 1, 3, 1, 1);
    this->itemPropLayout->addRow(this->idLabel, this->idLayout);

    //name
    this->nameLabel = new QLabel("Name: ", this->itemPropWidget);
    this->nameEdit = new QLineEdit(this->itemPropWidget);
    connect(this->nameEdit, SIGNAL(editingFinished()), this, SLOT(updateNames()));
    this->itemPropLayout->addRow(this->nameLabel, this->nameEdit);

    //background color
    this->backColComboLabel = new QLabel("Background: ", this->itemPropWidget);
    this->backColCombo = new QComboBox(this->itemPropWidget);
    for (int i = 0; i < this->colorNames.size(); i++) {
        QColor color(this->colorNames[i]);
        this->backColCombo->insertItem(i, this->colorNames[i]);
        if(this->colorNames[i] != "")
            this->backColCombo->setItemData(i, color, Qt::DecorationRole);
    }
    connect(this->backColCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBackColors(int)));
    this->itemPropLayout->addRow(this->backColComboLabel, this->backColCombo);

    //outline
    this->outlineLabel = new QLabel("Outline: ", this->itemPropWidget);
    this->outlineLayout = new QGridLayout(this->itemPropWidget);
    this->outlineLayout->setHorizontalSpacing(6);

    this->outColCombo = new QComboBox(this->itemPropWidget);
    for (int i = 0; i < this->colorNames.size(); i++) {
        QColor color(this->colorNames[i]);
        this->outColCombo->insertItem(i, this->colorNames[i]);
        if(this->colorNames[i] != "")
            this->outColCombo->setItemData(i, color, Qt::DecorationRole);
    }
    connect(this->outColCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateOutlineColors(int)));
    this->outlineLayout->addWidget(this->outColCombo, 1, 1, 1, 2);

    this->outWSpin = new QSpinBox(this->itemPropWidget);
    this->outWSpin->setRange(0, 50);
    connect(this->outWSpin, SIGNAL(valueChanged(int)), this, SLOT(updateOutlineWidths(int)));
    this->outlineLayout->addWidget(this->outWSpin, 1, 3, 1, 1);
    this->itemPropLayout->addRow(this->outlineLabel, this->outlineLayout);
    this->itemPropLayout->addRow(new QLabel("", this->itemPropWidget));

    //textboxes section
    lbl = new QLabel("Texboxes only", this->itemPropWidget);
    f = lbl->font(); f.setBold(true); lbl->setFont(f);
    this->itemPropLayout->addRow(lbl);

    //caption
    this->captionLabel = new QLabel("Caption: ", this->itemPropWidget);
    this->captionEdit = new QLineEdit(this->itemPropWidget);
    connect(this->captionEdit, SIGNAL(editingFinished()), this, SLOT(updateCaptions()));
    this->itemPropLayout->addRow(this->captionLabel, this->captionEdit);

    this->updateItemEditor();
}

void MainWindow::updateItemEditor(){
    this->itemEditorDisabled = true;

    //determine what has to be shown and/or active
    bool noSelection = this->nodeList.isEmpty() && this->modelList.isEmpty() &&
                       this->textboxList.isEmpty() && this->edgeList.isEmpty();

    bool onlyOneNode = this->nodeList.size() == 1 && this->modelList.isEmpty() &&
                       this->textboxList.isEmpty() && this->edgeList.isEmpty();

    bool onlyOneModel = this->nodeList.isEmpty() && this->modelList.size() == 1 &&
                        this->textboxList.isEmpty() && this->edgeList.isEmpty();

    bool onlyOneTextB = this->nodeList.isEmpty() && this->modelList.isEmpty() &&
                        this->textboxList.size() == 1 && this->edgeList.isEmpty();

    bool onlyOneEdge = this->nodeList.isEmpty() && this->modelList.isEmpty() &&
                       this->textboxList.isEmpty() && this->edgeList.size() == 1;

    bool onlyTextBs = this->nodeList.isEmpty() && this->modelList.isEmpty() &&
                      this->textboxList.size() > 0 /*&& this->edgeList.isEmpty()*/;

    bool nodesOrModels = (this->nodeList.size() > 0 || this->modelList.size() > 0) &&
                         this->textboxList.isEmpty() /*&& this->edgeList.isEmpty()*/;

    bool NorMorTB = this->nodeList.size() > 0 || this->modelList.size() > 0 ||
                    this->textboxList.size() > 0;

    //id
    if(onlyOneNode){
        this->currId = this->nodeList.at(0)->getNode()->getId();
        this->idEdit->setText(this->currId);
        this->idEdit->setToolTip("ID of the selected node");
        this->idEdit->setEnabled(true);
        this->randIdButton->setEnabled(true);
    }
    else if(onlyOneModel){
        this->currId = this->modelList.at(0)->getModel()->getId();
        this->idEdit->setText(this->currId);
        this->idEdit->setToolTip("ID of the selected model");
        this->idEdit->setEnabled(true);
        this->randIdButton->setEnabled(true);
    }
    else{
        this->currId = "";
        this->idEdit->setText("");
        this->idEdit->setEnabled(false);
        this->idEdit->setToolTip("ID of the selected item");
        this->randIdButton->setEnabled(false);
    }

    //name
    this->nameEdit->setEnabled(nodesOrModels);
    if(onlyOneNode){
        this->currName = this->nodeList.at(0)->getNode()->getName();
        this->nameEdit->setText(this->currName);
        this->nameEdit->setToolTip("The name of the selected node");
    }
    else if(onlyOneModel){
        this->currName = this->modelList.at(0)->getModel()->getName();
        this->nameEdit->setText(this->currName);
        this->nameEdit->setToolTip("The name of the selected model");
    }
    else{
        this->currName = "";
        this->nameEdit->setText("");
        this->nameEdit->setToolTip("The name of all the selected items");
    }

    //caption
    this->captionEdit->setEnabled(onlyTextBs);
    if(onlyOneTextB){
        this->currCaption = this->textboxList.at(0)->getTextbox()->getCaption();
        this->captionEdit->setText(this->currCaption);
        this->captionEdit->setToolTip("The content of the selected textbox");
    }
    else{
        this->currCaption = "";
        this->captionEdit->setText("");
        this->captionEdit->setToolTip("The content of all the selected textboxes");
    }

    //property
    this->propCombo->clear();
    QStringList props;
    QNProperties* propsPtr = nullptr;
    QString propSel;
    if(onlyOneNode){
        this->propCombo->setEnabled(true);
        this->propCombo->setToolTip("Property to be displayed by the selected node");
        propsPtr = this->nodeList.at(0)->getNode();
        propSel = this->nodeList.at(0)->getPropToShow();
    }
    else if(onlyOneModel){
        this->propCombo->setEnabled(true);
        this->propCombo->setToolTip("Property to be displayed by the selected model");
        propsPtr = this->modelList.at(0)->getModel();
        propSel = this->modelList.at(0)->getPropToShow();
    }
    else if(onlyOneTextB){
        this->propCombo->setEnabled(true);
        this->propCombo->setToolTip("Property to be displayed by the selected textbox");
        propsPtr = this->textboxList.at(0)->getTextbox();
        propSel = this->textboxList.at(0)->getPropToShow();
    }
    else if(!noSelection){
        this->propCombo->setEnabled(true);
        this->propCombo->setToolTip("Property to be displayed by all the selected items");

        foreach(QNNodeItemPtr n, this->nodeList){
            props += n->getNode()->getAllKeys();
        }

        foreach(QNModelItemPtr m, this->modelList){
            props += m->getModel()->getAllKeys();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            props += t->getTextbox()->getAllKeys();
        }
        props.removeDuplicates();
        this->propCombo->insertItem(0, "");
        this->propCombo->setCurrentIndex(0);
        int i=1;
        foreach (QString k, props) {
            this->propCombo->insertItem(i++, k);
        }
    }
    else{
        this->propCombo->setEnabled(false);
        this->propCombo->setToolTip("Property to be displayed by all the selected items");
    }

    if(propsPtr != nullptr){
        int i = 0;
        int selected = 0;
        props = propsPtr->getAllKeys();
        foreach (QString k, props) {
            QVariant p = propsPtr->getProperty(k);
            if(!p.isValid() || (p.isValid() && p.canConvert(QMetaType::QString))){
                this->propCombo->insertItem(i, k);
                if(k == propSel)
                    selected = i;
                i++;
            }
        }
        this->propCombo->setCurrentIndex(selected);
    }

    //label length
    this->lengthSpin->setEnabled(NorMorTB);
    if(onlyOneNode){
        this->lengthSpin->setValue(this->nodeList.at(0)->getLabelLen());
        this->lengthSpin->setToolTip("Lenght of the selected node's label");
    }
    else if(onlyOneModel){
        this->lengthSpin->setValue(this->modelList.at(0)->getLabelLen());
        this->lengthSpin->setToolTip("Lenght of the selected model's label");
    }
    else if(onlyOneTextB){
        this->lengthSpin->setValue(this->textboxList.at(0)->getLabelLen());
        this->lengthSpin->setToolTip("Lenght of the selected textbox's content");
    }
    else{
        this->lengthSpin->setValue(-1);
        this->lengthSpin->setToolTip("Lenght of the label displayed by all the selected items");
    }

    //background color
    this->backColCombo->setEnabled(nodesOrModels);
    QColor color;
    if(onlyOneNode){
        color = QColor("#"+this->nodeList.at(0)->getNode()->getColor());
        this->backColCombo->setToolTip("Background color of the selected node");
    }
    else if(onlyOneModel){
        color = QColor("#"+this->modelList.at(0)->getModel()->getColor());
        this->backColCombo->setToolTip("Background color of the selected model");
    }
    else{
        this->backColCombo->setToolTip("Background color of all selected items");
    }

    int c = 0;
    if(color.isValid()){
        for (int i = 1; i < this->colorNames.size(); i++) {
            QString tmpName = QColor(this->colorNames[i]).name();
            if(tmpName == color.name()){
                c = i;
                break;
            }
        }
    }
    this->backColCombo->setCurrentIndex(c);

    //outline
    this->outColCombo->setEnabled(nodesOrModels);
    this->outWSpin->setEnabled(nodesOrModels);
    QColor outColor;
    int w = 0;
    if(onlyOneNode){
        QNNodeIconPtr node = this->nodeList.at(0)->getNode();
        outColor = QColor("#" + node->getOutlineColor());
        w = node->getOutlineWidth();
        this->outColCombo->setToolTip("Outline color of the selected node");
        this->outWSpin->setToolTip("Outline width of the selected node");
    }
    else if(onlyOneModel){
        QNModelPtr model = this->modelList.at(0)->getModel();
        outColor = QColor("#" + model->getOutlineColor());
        w = model->getOutlineWidth();
        this->outColCombo->setToolTip("Outline color of the selected model");
        this->outWSpin->setToolTip("Outline width of the selected model");
    }
    else{
        this->outColCombo->setToolTip("Outline color of all selected items");
        this->outWSpin->setToolTip("Outline width of all selected items");
    }

    int oc = 0;
    if(outColor.isValid()){
        for (int i = 1; i < this->colorNames.size(); i++) {
            QString tmpName = QColor(this->colorNames[i]).name();
            if(tmpName == outColor.name()){
                oc = i;
                break;
            }
        }
    }
    this->outColCombo->setCurrentIndex(oc);
    this->outWSpin->setValue(w);

    //font
    this->fontCombo->setEnabled(NorMorTB);
    this->fontColCombo->setEnabled(NorMorTB);
    this->fontSizeSpin->setEnabled(NorMorTB);
    this->boldButton->setEnabled(NorMorTB);
    this->italicButton->setEnabled(NorMorTB);
    this->alignCombo->setEnabled(NorMorTB);

    QNFontPtr font = nullptr;
    if(onlyOneNode){
        font = this->nodeList.at(0)->getNode()->getFont();
        this->fontCombo->setToolTip("Font name of the selected node");
        this->fontColCombo->setToolTip("Font color of the selected node");
        this->fontSizeSpin->setToolTip("Font size of the selected node");
        this->alignCombo->setToolTip("Font align of the selected node");
    }
    else if(onlyOneModel){
        font = this->modelList.at(0)->getModel()->getFont();
        this->fontCombo->setToolTip("Font name of the selected model");
        this->fontColCombo->setToolTip("Font color of the selected model");
        this->fontSizeSpin->setToolTip("Font size of the selected model");
        this->alignCombo->setToolTip("Font align of the selected model");
    }
    else if(onlyOneTextB){
        font = this->textboxList.at(0)->getTextbox()->getFont();
        this->fontCombo->setToolTip("Font name of the selected textbox");
        this->fontColCombo->setToolTip("Font color of the selected textbox");
        this->fontSizeSpin->setToolTip("Font size of the selected textbox");
        this->alignCombo->setToolTip("Font align of the selected textbox");
    }
    else{
        this->fontCombo->setToolTip("Font name of all selected items");
        this->fontColCombo->setToolTip("Font color of all selected items");
        this->fontSizeSpin->setToolTip("Font size of all selected items");
        this->alignCombo->setToolTip("Font align of all selected items");
    }

    if(font != nullptr){
        QFont fontName(font->getFontName());
        this->fontCombo->setCurrentFont(fontName);
        QColor fontColor = QColor("#"+font->getFontColor());
        int c = 0;
        if(fontColor.isValid()){
            for (int i = 1; i < this->colorNames.size(); i++) {
                QString tmpName = QColor(this->colorNames[i]).name();
                if(tmpName == fontColor.name()){
                    c = i;
                    break;
                }
            }
        }
        this->fontColCombo->setCurrentIndex(c);
        this->fontSizeSpin->setValue(font->getFontSize());
        this->boldButton->setChecked(font->isBold());
        this->italicButton->setChecked(font->isItalic());
        this->alignCombo->setCurrentIndex(font->getAlign());
    }
    else{
        this->fontCombo->setCurrentFont(QFont());
        this->fontColCombo->setCurrentIndex(0);
        this->fontSizeSpin->setValue(0);
        this->boldButton->setChecked(false);
        this->italicButton->setChecked(false);
        this->alignCombo->setCurrentIndex(QNAlignment::undefAlign);
    }

    //comment
    this->commentEdit->setEnabled(!noSelection);
    if(onlyOneNode){
        this->currComment = this->nodeList.at(0)->getNode()->getComment();
        this->commentEdit->setText(this->currComment);
        this->commentEdit->setToolTip("A comment associated to the selected node");
    }
    else if(onlyOneModel){
        this->currComment = this->modelList.at(0)->getModel()->getComment();
        this->commentEdit->setText(this->currComment);
        this->commentEdit->setToolTip("A comment associated to the selected model");
    }
    else if(onlyOneTextB){
        this->currComment = this->textboxList.at(0)->getTextbox()->getComment();
        this->commentEdit->setText(this->currComment);
        this->commentEdit->setToolTip("A comment associated to the selected textbox");
    }
    else if(onlyOneEdge){
        this->currComment = this->edgeList.at(0)->getEdge()->getComment();
        this->commentEdit->setText(this->currComment);
        this->commentEdit->setToolTip("A comment associated to the selected edge");
    }
    else{
        this->currComment = "";
        this->commentEdit->setText("");
        this->commentEdit->setToolTip("A comment associated to all the selected items");
    }

    this->itemEditorDisabled = false;
}

void MainWindow::initStyleEditor(){
    //classes section
    this->classesLabel = new QLabel("Classes", this->styleEditorWidget);
    QFont f = this->classesLabel->font(); f.setBold(true); this->classesLabel->setFont(f);
    this->styleEditLayout->addRow(this->classesLabel);
    this->classesLayout = new QGridLayout(this->styleEditorWidget);
    this->classesLayout->setSpacing(6);
    this->classesCombo = new QComboBox(this->styleEditorWidget);
    this->classesCombo->setToolTip("The list of available classes");
    connect(this->classesCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassCombo(int)));
    this->classesLayout->addWidget(this->classesCombo, 1, 1, 1, 2);

    this->newClassButton = new QPushButton(QIcon::fromTheme("edit-new"), tr("New class"), this->styleEditorWidget);
    this->newClassButton->setToolTip("Click here to create a new class");
    connect(this->newClassButton, SIGNAL(clicked(bool)), this, SLOT(newClass(bool)));
    this->classesLayout->addWidget(this->newClassButton, 2, 1, 1, 1);

    this->delClassButton = new QPushButton(QIcon::fromTheme("edit-delete"), tr("Delete class"), this->styleEditorWidget);
    this->delClassButton->setToolTip("Click here to delete current class");
    connect(this->delClassButton, SIGNAL(clicked(bool)), this, SLOT(delClass(bool)));
    this->classesLayout->addWidget(this->delClassButton, 2, 2, 1, 1);

    this->addClassButton = new QPushButton(QIcon::fromTheme("list-add"), tr("Add to selected"), this->styleEditorWidget);
    this->addClassButton->setToolTip("Click here to add the class to all selected items");
    connect(this->addClassButton, SIGNAL(clicked(bool)), this, SLOT(addClass(bool)));
    this->classesLayout->addWidget(this->addClassButton, 3, 1, 1, 1);

    this->remClassButton = new QPushButton(QIcon::fromTheme("list-remove"), tr("Remove from selected"), this->styleEditorWidget);
    this->remClassButton->setToolTip("Click here to remove the class from all selected items");
    connect(this->remClassButton, SIGNAL(clicked(bool)), this, SLOT(remClass(bool)));
    this->classesLayout->addWidget(this->remClassButton, 3, 2, 1, 1);

    this->styleEditLayout->addRow(this->classesLayout);
    this->styleEditLayout->addRow(new QLabel("", this->styleEditorWidget));

    //class detail
    QLabel* lbl = new QLabel("Class detail", this->styleEditorWidget);
    f = lbl->font(); f.setBold(true); lbl->setFont(f);
    this->styleEditLayout->addRow(lbl);
    //class name
    this->classNameLabel = new QLabel("Name: ", this->styleEditorWidget);
    this->classNameEdit = new QLineEdit(this->styleEditorWidget);
    this->classNameEdit->setToolTip("The name of the selected class");
    connect(this->classNameEdit, SIGNAL(editingFinished()), this, SLOT(updateClassName()));
    this->styleEditLayout->addRow(this->classNameLabel, this->classNameEdit);

    //background
    this->classBackLabel = new QLabel("Background: ", this->styleEditorWidget);
    this->classBackLayout = new QGridLayout(this->styleEditorWidget);
    this->classBackColCombo = new QComboBox(this->styleEditorWidget);
    this->classBackColCombo->setToolTip("Background color for the selected class");
    for (int i = 0; i < this->colorNames.size(); i++) {
        QColor color(this->colorNames[i]);
        this->classBackColCombo->insertItem(i, this->colorNames[i]);
        if(this->colorNames[i] != "")
            this->classBackColCombo->setItemData(i, color, Qt::DecorationRole);
    }
    connect(this->classBackColCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassBackCol(int)));
    this->classBackLayout->addWidget(this->classBackColCombo, 1, 1, 1, 3);

    this->classBackBrushCombo = new QComboBox(this->styleEditorWidget);
    this->classBackBrushCombo->setToolTip("Background brush for the selected class");
    this->initBackBrushCombo();
    connect(this->classBackBrushCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassBackBrush(int)));
    this->classBackLayout->addWidget(this->classBackBrushCombo, 2, 1, 1, 3);
    this->styleEditLayout->addRow(this->classBackLabel, this->classBackLayout);

    //outline
    this->outlineLabel = new QLabel("Outline: ", this->styleEditorWidget);
    this->classOutLayout = new QGridLayout(this->styleEditorWidget);
    this->classOutColCombo = new QComboBox(this->styleEditorWidget);
    this->classOutColCombo->setToolTip("Outline color for the selected class");
    for (int i = 0; i < this->colorNames.size(); i++) {
        QColor color(this->colorNames[i]);
        this->classOutColCombo->insertItem(i, this->colorNames[i]);
        if(this->colorNames[i] != "")
            this->classOutColCombo->setItemData(i, color, Qt::DecorationRole);
    }
    connect(this->classOutColCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassOutCol(int)));
    this->classOutLayout->addWidget(this->classOutColCombo, 1, 1, 1, 2);

    this->classOutSizeSpin = new QSpinBox(this->styleEditorWidget);
    this->classOutSizeSpin->setToolTip("Outline width for the selected class");
    this->classOutSizeSpin->setRange(0, 50);
    connect(this->classOutSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(updateClassOutSize(int)));
    this->classOutLayout->addWidget(this->classOutSizeSpin, 1, 3, 1, 1);

    this->classOutLCombo = new QComboBox(this->styleEditorWidget);
    this->classOutLCombo->setToolTip("Outline line style for the selected class");
    this->initOutL();
    connect(this->classOutLCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassOutL(int)));
    this->classOutLayout->addWidget(this->classOutLCombo, 2, 1, 1, 3);

    this->classOutCapCombo = new QComboBox(this->styleEditorWidget);
    this->classOutCapCombo->setToolTip("Outline cap style for the selected class");
    this->initOutCap();
    connect(this->classOutCapCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassOutCap(int)));
    this->classOutLayout->addWidget(this->classOutCapCombo, 3, 1, 1, 3);

    this->classOutJoinCombo = new QComboBox(this->styleEditorWidget);
    this->classOutJoinCombo->setToolTip("Outline join style for the selected class");
    this->initOutJoin();
    connect(this->classOutJoinCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassOutJoin(int)));
    this->classOutLayout->addWidget(this->classOutJoinCombo, 4, 1, 1, 3);
    this->styleEditLayout->addRow(this->outlineLabel, this->classOutLayout);

    //font
    this->classFontLabel = new QLabel("Font: ", this->styleEditorWidget);
    this->classFontLayout = new QGridLayout(this->styleEditorWidget);
    this->classFontLayout->setSpacing(6);

    this->classFontCombo = new QFontComboBox(this->styleEditorWidget);
    this->classFontCombo->setToolTip("The font name for the selected class");
    connect(this->classFontCombo, SIGNAL(currentFontChanged(QFont)), this, SLOT(updateClassFont(QFont)));
    this->classFontLayout->addWidget(this->classFontCombo, 1, 1, 1, 3);

    this->classAlignCombo = new QComboBox(this->styleEditorWidget);
    this->classAlignCombo->setToolTip("Font align for the selected class");
    this->initClassAlign();
    connect(this->classAlignCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassAlign(int)));
    this->classFontLayout->addWidget(this->classAlignCombo, 2, 1, 1, 1);

    this->classBoldButton = new QPushButton(QIcon::fromTheme("format-text-bold"), tr(""), this->styleEditorWidget);
    this->classBoldButton->setToolTip("Bold font for the selected class");
    this->classBoldButton->setCheckable(true);
    connect(this->classBoldButton, SIGNAL(toggled(bool)), this, SLOT(updateClassFontBold(bool)));
    this->classFontLayout->addWidget(this->classBoldButton, 2, 2, 1, 1);

    this->classItalicButton = new QPushButton(QIcon::fromTheme("format-text-italic"), tr(""), this->styleEditorWidget);
    this->classItalicButton->setToolTip("Italic font for the selected class");
    this->classItalicButton->setCheckable(true);
    connect(this->classItalicButton, SIGNAL(toggled(bool)), this, SLOT(updateClassFontItalic(bool)));
    this->classFontLayout->addWidget(this->classItalicButton, 2, 3, 1, 1);

    this->classFontColCombo = new QComboBox(this->styleEditorWidget);
    for (int i = 0; i < this->colorNames.size(); i++) {
        QColor color(this->colorNames[i]);
        this->classFontColCombo->insertItem(i, this->colorNames[i]);
        if(this->colorNames[i] != "")
            this->classFontColCombo->setItemData(i, color, Qt::DecorationRole);
    }
    connect(this->classFontColCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassFontCol(int)));
    this->classFontLayout->addWidget(this->classFontColCombo, 3, 1, 1, 2);

    this->classFontSizeSpin = new QSpinBox(this->styleEditorWidget);
    this->classFontSizeSpin->setToolTip("Font size for the selected class");
    this->classFontSizeSpin->setRange(0, 100);
    connect(this->classFontSizeSpin, SIGNAL(valueChanged(int)), this, SLOT(updateClassFontSize(int)));
    this->classFontLayout->addWidget(this->classFontSizeSpin, 3, 3, 1, 1);
    this->styleEditLayout->addRow(this->classFontLabel, this->classFontLayout);

    //shape
    this->classShapeLabel = new QLabel("Node shape: ", this->styleEditorWidget);
    this->classShapeLayout = new QGridLayout(this->styleEditorWidget);
    this->classShapeLayout->setHorizontalSpacing(6);
    this->classShapeCombo = new QComboBox(this->styleEditorWidget);
    this->classShapeCombo->setToolTip("Nodes' shape for the selected class");
    this->initClassShapes();
    connect(this->classShapeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassShape(int)));
    this->classShapeLayout->addWidget(this->classShapeCombo, 1, 1, 1, 3);

    this->classShapePathEdit = new QLineEdit(this->styleEditorWidget);
    this->classShapePathEdit->setToolTip("Nodes' background image for the selected class");
    this->classShapePathEdit->setText("");
    connect(this->classShapePathEdit, SIGNAL(editingFinished()), this, SLOT(updateImage()));
    this->classShapeLayout->addWidget(this->classShapePathEdit, 2, 1, 1, 2);

    this->classShapePathButton = new QPushButton(QIcon::fromTheme("document-open"), tr(""), this->styleEditorWidget);
    this->classShapePathButton->setToolTip("Click to choose a new background image");
    connect(this->classShapePathButton, SIGNAL(clicked(bool)), this, SLOT(openChooseImage(bool)));
    this->classShapeLayout->addWidget(this->classShapePathButton, 2, 3, 1, 1);

    this->styleEditLayout->addRow(this->classShapeLabel, this->classShapeLayout);

    //arrow
    this->classArrowLabel = new QLabel("Arrow: ", this->styleEditorWidget);
    this->classArrowCombo = new QComboBox(this->styleEditorWidget);
    this->classShapeCombo->setToolTip("Edges' arrow for the selected class");
    this->initArrows();
    connect(this->classArrowCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(updateClassArrow(int)));
    this->styleEditLayout->addRow(this->classArrowLabel, this->classArrowCombo);

    this->initClassesCombo();
    this->classesCombo->setCurrentIndex(0);
}

QString MainWindow::getSupported(bool isStyle){
    QString result = "";

    QRegExpList list;
    if(!isStyle)
        list = this->persistMgr->supportedFormats();
    else
        list = this->styleMgr->supportedFormats();

    int size = list.size();
    for(int i=0; i<size; i++){
        result.append(list.at(i).pattern());
        if(i != size-1)
            result.append(" ");
    }

    return QString("%1 (%2)").arg(!isStyle? "QtNets": "Style").arg(result);
}

void MainWindow::save(bool isStyle){
    if(!isStyle){
        if(!this->fileName.isEmpty()){
            QNError err = this->persistMgr->store(this->fileName, this->net);
            if(err)
                this->showError("Error saving network: " + err.getDetail());
            else
                this->setDirty(false);
        }
    }
    else{
        if(!this->styleFileName.isEmpty()){
            QNError err = this->styleMgr->store(this->styleFileName);
            if(err)
                this->showError("Error saving style: " + err.getDetail());
            else
                this->setDirty(false, true);
        }
    }
}

void MainWindow::setDirty(const bool aDirty, bool isStyle){
    if(!isStyle){
        this->dirty = aDirty;
        if(this->dirty){
            if(!this->fileName.isEmpty())
                this->setWindowTitle(QString("QtNetsEditor [ %1* ]").arg(this->fileName));
            else
                this->setWindowTitle(QString("QtNetsEditor [ * ]"));
        }
        else{
            if(!this->fileName.isEmpty())
                this->setWindowTitle(QString("QtNetsEditor [ %1 ]").arg(this->fileName));
            else
                this->setWindowTitle(QString("QtNetsEditor"));
        }
    }
    else{
        this->styleDirty = aDirty;
        if(this->styleDirty){
            if(!styleFileName.isEmpty())
                this->classesLabel->setText(QString("Classes [ %1* ]").arg(this->styleFileName));
            else
                this->classesLabel->setText(QString("Classes [ * ]"));
        }
        else{
            if(!styleFileName.isEmpty())
                this->classesLabel->setText(QString("Classes [ %1 ]").arg(this->styleFileName));
            else
                this->classesLabel->setText(QString("Classes"));
        }
    }
}

void MainWindow::initNet(){
    this->net.reset(new QNNetwork("Network 1", "1.0"));
    this->net->setNetModel(new QNNetworkModel(this->net.data()));
    this->scene->setSceneRect(QRectF());
    this->scene->setNetwork(this->net->getNetModel());
}

void MainWindow::populateNetModel(){
    if(this->net->getNetModel() == nullptr)
        this->net->setNetModel(new QNNetworkModel(this->net.data()));

    //populate nodes
    QStringList keys = this->net->getAllNodesKeys();
    if(!keys.isEmpty()){
        foreach(QString k, keys){
            QNNodePtr n = this->net->getNode(k);
            if(n->getIcon() == nullptr){
                //add missing icons
                QNNodeIconPtr nodeIcon = new QNNodeIcon(this->net->getNetModel());
                nodeIcon->setData(n);
                n->setIcon(nodeIcon);
                nodeIcon->setId(n->getId());
                nodeIcon->setName(n->getId());
                nodeIcon->setFont(new QNFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, DEFAULT_FONT_COLOR));
                nodeIcon->setColor(DEFAULT_BG_COLOR);
                nodeIcon->setOutlineColor(DEFAULT_FG_COLOR);
                nodeIcon->setOutlineWidth(DEFAULT_WIDTH);
                nodeIcon->setPosition(new QNPosition(0,0,0,0));
                this->net->getNetModel()->addIcon(nodeIcon);
            }
        }
    }

    //populate edges
    keys = this->net->getAllEdgesKeys();
    if(!keys.isEmpty()){
        for(QString k: keys){
            QNEdgePtr edge = this->net->getEdge(k);
            if(edge->getIcon() == nullptr){
                //create the missing icon
                QNEdgeStylePtr a = new QNEdgeStyle(edge);
                edge->setIcon(a);
                this->net->getNetModel()->addEdge(a);
            }
        }
    }
}

void MainWindow::resetView(){
    this->view->setSceneRect(QRectF());
    this->view->resetCachedContent();
    this->view->restoreZoom();
    this->view->restoreRotate();
    this->view->horizontalScrollBar()->setValue(this->view->horizontalScrollBar()->minimum());
    this->view->verticalScrollBar()->setValue(this->view->verticalScrollBar()->minimum());
}

void MainWindow::resetNet(){
    this->dirty = false;
    this->fileName = "";
    this->setWindowTitle("QtNetsEditor");
    this->scene->setNetwork(nullptr);
    this->net.reset();
    this->resetView();
    this->initNet();
    this->updateSelected(QNNodeItemList(), QNModelItemList(),
                         QNTextBoxItemList(), QNEdgeItemList());
}

void MainWindow::resetStyle(){
    this->styleFileName = "";
    this->styleMgr->resetStyle();
    this->setDirty(false, true);
    this->initClassesCombo();
    this->classesCombo->setCurrentIndex(0);
    this->updateAll();
}

void MainWindow::initClassesCombo(){
    int i=0;
    this->classesCombo->clear();
    this->classesCombo->insertItem(i++, "");

    QNStylePtr style = this->styleMgr->getStyle();
    if(style != nullptr){
        QStringList classes = style->getAllClassesNames();
        foreach(QString c, classes){
            this->classesCombo->insertItem(i++, c);
        }
    }
}

void MainWindow::initBackBrushCombo(){
    this->classBackBrushCombo->insertItem(BSBrushStyle::transparent, "transparent");
    this->classBackBrushCombo->insertItem(BSBrushStyle::solid, "solid");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense1, "dense 1");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense2, "dense 2");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense3, "dense 3");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense4, "dense 4");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense5, "dense 5");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense6, "dense 6");
    this->classBackBrushCombo->insertItem(BSBrushStyle::dense7, "dense 7");
    this->classBackBrushCombo->insertItem(BSBrushStyle::horizontal, "horizontal");
    this->classBackBrushCombo->insertItem(BSBrushStyle::vertiacal, "vertiacal");
    this->classBackBrushCombo->insertItem(BSBrushStyle::cross, "cross");
    this->classBackBrushCombo->insertItem(BSBrushStyle::backDiagonal, "back diagonal");
    this->classBackBrushCombo->insertItem(BSBrushStyle::forwardDiagonal, "forward diagonal");
    this->classBackBrushCombo->insertItem(BSBrushStyle::crossDiagonal, "cross diagonal");
    this->classBackBrushCombo->insertItem(BSBrushStyle::linearGradient, "linear gradient");
    this->classBackBrushCombo->insertItem(BSBrushStyle::radialGradient, "radial gradient");
}

void MainWindow::initOutL(){
    this->classOutLCombo->insertItem(BSLineStyle::noLine, "no line");
    this->classOutLCombo->insertItem(BSLineStyle::solidLine, "solid line");
    this->classOutLCombo->insertItem(BSLineStyle::dashLine, "dash line");
    this->classOutLCombo->insertItem(BSLineStyle::dotLine, "dot line");
    this->classOutLCombo->insertItem(BSLineStyle::dashDotLine, "dash dot line");
    this->classOutLCombo->insertItem(BSLineStyle::dashDotDotLine, "dash dot dot line");
}

void MainWindow::initOutCap(){
    QVariant qv;
    qv.setValue(BSCapStyle::flatCap);
    this->classOutCapCombo->insertItem(0, "flat cap", qv);
    qv.setValue(BSCapStyle::squareCap);
    this->classOutCapCombo->insertItem(1, "square cap", qv);
    qv.setValue(BSCapStyle::roundCap);
    this->classOutCapCombo->insertItem(2, "round cap", qv);
}

void MainWindow::initOutJoin(){
    QVariant qv;
    qv.setValue(BSJoinStyle::miterJoin);
    this->classOutJoinCombo->insertItem(0, "miter join", qv);
    qv.setValue(BSJoinStyle::bevelJoin);
    this->classOutJoinCombo->insertItem(1, "bevel join", qv);
    qv.setValue(BSJoinStyle::roundJoin);
    this->classOutJoinCombo->insertItem(2, "round join", qv);
}

void MainWindow::initClassAlign(){
    this->classAlignCombo->insertItem(BSTextAlign::textLeft, "left");
    this->classAlignCombo->insertItem(BSTextAlign::textCenter, "center");
    this->classAlignCombo->insertItem(BSTextAlign::textRight, "right");
}

void MainWindow::initClassShapes(){
    this->classShapeCombo->insertItem(BSNodeShape::square, "square");
    this->classShapeCombo->insertItem(BSNodeShape::roundedSquare, "rounded square");
    this->classShapeCombo->insertItem(BSNodeShape::rectagle, "rectagle");
    this->classShapeCombo->insertItem(BSNodeShape::roundedRect, "rounded rectangle");
    this->classShapeCombo->insertItem(BSNodeShape::rhombus, "rhombus");
    this->classShapeCombo->insertItem(BSNodeShape::triangle, "triangle");
    this->classShapeCombo->insertItem(BSNodeShape::reverseTriangle, "reverse triangle");
    this->classShapeCombo->insertItem(BSNodeShape::ellipse, "ellipse");
    this->classShapeCombo->insertItem(BSNodeShape::circle, "circle");
    this->classShapeCombo->insertItem(BSNodeShape::hexagon, "hexagon");
    this->classShapeCombo->insertItem(BSNodeShape::image, "image");
}

void MainWindow::initArrows(){
    this->classArrowCombo->insertItem(BSArrowStyle::none, "none");
    this->classArrowCombo->insertItem(BSArrowStyle::empty, "empty");
    this->classArrowCombo->insertItem(BSArrowStyle::full, "full");
    this->classArrowCombo->insertItem(BSArrowStyle::circular, "circular");
    this->classArrowCombo->insertItem(BSArrowStyle::diamond, "diamond");
}

void MainWindow::updateSelClass(){
    QNStylePtr style = this->styleMgr->getStyle();
    int pos = this->classesCombo->currentIndex();
    if(style != nullptr && pos > 0)
        this->selClass = style->getClass(this->classesCombo->itemText(pos));
    else
        this->selClass = nullptr;
}

void MainWindow::updateAll(){
    if(this->scene != nullptr){
        QGraphicsItemList items = this->scene->items();
        foreach (QGraphicsItemPtr it, items) {
            switch (it->type()) {
                case QNNodeItem::Type:{
                    QNNodeItemPtr nodeIt = qgraphicsitem_cast<QNNodeItemPtr>(it);
                    if(this->styleMgr != nullptr && this->styleMgr->getStyle() != nullptr)
                        this->styleMgr->updateFont(nodeIt);
                    else
                        nodeIt->updateNode();

                    break;
                }
                case QNModelItem::Type:{
                    QNModelItemPtr modelIt = qgraphicsitem_cast<QNModelItemPtr>(it);
                    if(this->styleMgr != nullptr && this->styleMgr->getStyle() != nullptr)
                        this->styleMgr->updateFont(modelIt);
                    else
                        modelIt->updateModel();

                    break;
                }
                case QNTextBoxItem::Type:{
                    QNTextBoxItemPtr textIt = qgraphicsitem_cast<QNTextBoxItemPtr>(it);
                    if(this->styleMgr != nullptr && this->styleMgr->getStyle() != nullptr)
                        this->styleMgr->updateFont(textIt);
                    else
                        textIt->updateTextbox();

                    break;
                }
                case QNEdgeItem::Type:{
                    QNEdgeItemPtr edgeIt = qgraphicsitem_cast<QNEdgeItemPtr>(it);
                    //                  if(this->styleMgr != nullptr && this->styleMgr->getStyle() != nullptr)
                    //                      this->styleMgr->updateFont(edgeIt);
                    //                  else
                    edgeIt->updateEdge();

                    break;
                }
                default:
                    break;
            }
        }

        this->scene->update();
    }
}

void MainWindow::reload(){
    if(!this->fileName.isEmpty()){
        QNError err = this->persistMgr->load(this->fileName, this->net);
        if(err){
            this->showError("Error loading network: " + err.getDetail());
        }
        else{
            this->populateNetModel();
            this->scene->setSceneRect(QRectF());
            this->scene->setNetwork(this->net->getNetModel());
            this->resetView();
            this->updateSelected(QNNodeItemList(), QNModelItemList(),
                                 QNTextBoxItemList(), QNEdgeItemList());
            this->setDirty(false);
        }
    }
}

void MainWindow::openFile(){
    this->closeFile();

    this->fileName = QFileDialog::getOpenFileName(this, tr("Choose a File"), "", this->getSupported());
    this->reload();
}

void MainWindow::saveFile(){
    if(this->fileName.isEmpty())
        this->saveAsFile();
    else
        this->save();
}

void MainWindow::saveAsFile(){
    this->fileName = QFileDialog::getSaveFileName(this, tr("Choose a File"), this->fileName, this->getSupported());
    this->save();
}

void MainWindow::closeFile(){
    if(!this->dirty){
        this->resetNet();
    }
    else{
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("The network has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:{
                this->saveFile();
                this->resetNet();
                break;
            }
            case QMessageBox::Discard:
                this->resetNet();
                break;
                //            case QMessageBox::Cancel:
                //                // Cancel was clicked
                //                break;
            default:
                // do nothing
                break;
        }
    }
}

void MainWindow::print(){
#if !defined(QT_NO_PRINTER) && !defined(QT_NO_PRINTDIALOG)
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog dialog(&printer, this);
    if (dialog.exec() == QDialog::Accepted) {
        QPainter painter(&printer);
        this->scene->clearFocus();
        this->scene->clearSelection();
        this->scene->render(&painter);
    }
#endif
}

void MainWindow::exportAs(){
    this->imgName = QFileDialog::getSaveFileName(this, tr("Choose an Image"), this->imgName, tr("Images (*.png *.xpm *.jpg *.jpeg)"));
    if(!this->imgName.isEmpty()){
        QImage image(this->scene->sceneRect().size().toSize(), QImage::Format_ARGB32);
        image.fill(Qt::white);
        QPainter painter(&image);
        this->scene->clearFocus();
        this->scene->clearSelection();
        this->scene->render(&painter);
        if(!image.save(this->imgName))
            this->showError("Error exporting the current network to " + this->imgName);
    }
}

void MainWindow::rotateLeft90(){
    if(this->view != nullptr)
        this->view->rotateLeft(90);
}

void MainWindow::rotateRight90(){
    if(this->view != nullptr)
        this->view->rotateRight(90);
}

void MainWindow::rememberOnDelete(){
    this->toRemeber = this->rememberOnDeleteAction->isChecked();
}

void MainWindow::deleteItems(){
    int num = this->scene->selectedItems().size();
    if(num > 1 || (num == 1 &&
                   this->scene->selectedItems().at(0)->type() == QNModelItem::Type &&
                   this->scene->selectedItems().at(0)->childItems().size() > 0)){
        if(!this->toRemeber){
            QString msg = QString("Are you sure you want to delete %1 elements?").arg(num);
            if(num == 1){
                QNModelItemPtr model = qgraphicsitem_cast<QNModelItemPtr>(this->scene->selectedItems().at(0));
                msg = QString("Are you sure you want to delete %1 model?").arg(model->getLabel().toPlainText());
            }
            QMessageBox msgBox;
            msgBox.setIcon(QMessageBox::Warning);
            msgBox.setText(msg);
            QCheckBox cb("Don't ask again");
            cb.setChecked(this->toRemeber);
            msgBox.setCheckBox(&cb);
            msgBox.setStandardButtons(QMessageBox::No | QMessageBox::Yes);
            msgBox.setDefaultButton(QMessageBox::No);
            this->choice = msgBox.exec();
            this->toRemeber = cb.isChecked();
            this->rememberOnDeleteAction->setChecked(this->toRemeber);
        }

        if(this->choice == QMessageBox::Yes)
            this->scene->deleteItems();
    }
    else
        this->scene->deleteItems();
}

void MainWindow::rememberNode(){
    if(this->scene != nullptr){
        QGraphicsItemList list = this->scene->selectedItems();
        if(list.size() == 1)
            this->rememberedNode = qgraphicsitem_cast<QNNodeItemPtr>(list.at(0));
        else{
            list = this->scene->items(this->scene->getMousePos(), Qt::ContainsItemBoundingRect);
            if(list.size() == 1)
                this->rememberedNode = qgraphicsitem_cast<QNNodeItemPtr>(list.at(0));
            else
                this->rememberedNode = nullptr;
        }
    }
    else
        this->rememberedNode = nullptr;
}

void MainWindow::newEdge(){
    if(this->rememberedNode != nullptr){
        QNNodeItemPtr dst = nullptr;
        if(this->scene != nullptr){
            QGraphicsItemList list = this->scene->selectedItems();
            if(list.size() == 1)
                dst = qgraphicsitem_cast<QNNodeItemPtr>(list.at(0));
            else{
                list = this->scene->items(this->scene->getMousePos(), Qt::ContainsItemBoundingRect);
                if(list.size() == 1)
                    dst = qgraphicsitem_cast<QNNodeItemPtr>(list.at(0));
                else
                    dst = nullptr;
            }
        }
        else
            dst = nullptr;

        if(dst != nullptr){
            this->scene->newEdge(this->rememberedNode, dst);
            this->rememberedNode = nullptr;
        }
        else
            this->scene->newEdgeMode();
    }
    else
        this->scene->newEdgeMode();
}

void MainWindow::showWarning(const QString& aWarn){
    QMessageBox::warning(this, tr("Warning"), aWarn);
}

void MainWindow::showError(const QString& aError){
    QMessageBox::critical(this, tr("Error"), aError);
}

void MainWindow::showAbout(){
    QMessageBox::information(this, tr("QtNets Demo"),
                             tr("this is a demo application that shows the base "
                                "functionalities of the QtNets set of libraries"));
}

void MainWindow::sceneModified(){
    this->setDirty(true);
}

void MainWindow::openStyle(){
    this->closeStyle();

    this->styleFileName = QFileDialog::getOpenFileName(this, tr("Choose a Style sheet"), "", this->getSupported(true));
    this->reloadStyle();
}

void MainWindow::reloadStyle(){
    if(!this->styleFileName.isEmpty()){
        QNError err = this->styleMgr->load(this->styleFileName);
        if(err){
            this->showError("Error loading style: " + err.getDetail());
        }
        else{
            this->initClassesCombo();
            this->classesCombo->setCurrentIndex(1);
            this->setDirty(false, true);
            this->updateAll();
        }
    }
}

void MainWindow::saveStyle(){
    if(this->styleFileName.isEmpty())
        this->saveAsStyle();
    else
        this->save(true);
}

void MainWindow::saveAsStyle(){
    this->styleFileName = QFileDialog::getSaveFileName(this, tr("Choose a Style sheet"), "", this->getSupported(true));
    this->save(true);
}

void MainWindow::closeStyle(){
    if(!this->styleDirty){
        this->resetStyle();
    }
    else{
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("The style sheet has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Save);
        int ret = msgBox.exec();

        switch (ret) {
            case QMessageBox::Save:{
                this->saveStyle();
                this->resetStyle();
                break;
            }
            case QMessageBox::Discard:
                this->resetStyle();
                break;
                //            case QMessageBox::Cancel:
                //                // Cancel was clicked
                //                break;
            default:
                // do nothing
                break;
        }
    }
}

void MainWindow::updateSelected(QNNodeItemList aNodeList, QNModelItemList aModelList, QNTextBoxItemList aTextboxList, QNEdgeItemList aEdgeList){
    this->nodeList = aNodeList;
    this->modelList = aModelList;
    this->textboxList = aTextboxList;
    this->edgeList = aEdgeList;

    this->updateItemEditor();
}

void MainWindow::updateId(){
    if(!this->itemEditorDisabled){
        QString newId = this->idEdit->text();

        if(!newId.isEmpty() && newId != this->currId){
            this->currId = newId;

            QNError err;
            if(this->nodeList.size() == 1){
                QNNodePtr n = this->nodeList.at(0)->getNode()->getData();
                if(n->getId() != newId){
                    err = n->setId(newId);
                    this->nodeList.at(0)->updateNode();
                }
            }
            else if(this->modelList.size() == 1){
                QNModelPtr m = this->modelList.at(0)->getModel();
                if(m->getId() != newId){
                    err = m->setId(newId);
                    this->modelList.at(0)->updateModel();
                }
            }

            if(err)
                this->showError("Error updating ID: " + err.getDetail());
            else
                this->setDirty(true);
        }
        else{
            this->itemEditorDisabled = true;
            this->idEdit->setText(this->currId);
            this->itemEditorDisabled = false;
        }
    }
}

void MainWindow::genRandId(bool aChecked){
    Q_UNUSED(aChecked);
    if(!this->itemEditorDisabled){
        QNError err;
        if(this->nodeList.size() == 1){
            QNNodeIconPtr n = this->nodeList.at(0)->getNode();
            err = n->initId();
            if(!err){
                this->currId = n->getId();
                this->idEdit->setText(this->currId);
                this->nodeList.at(0)->updateNode();
            }
        }
        else if(this->modelList.size() == 1){
            QNModelPtr m = this->modelList.at(0)->getModel();
            err = m->initId();
            if(!err){
                this->currId = m->getId();
                this->idEdit->setText(this->currId);
                this->modelList.at(0)->updateModel();
            }
        }

        if(err)
            this->showError("Error generating ID: " + err.getDetail());
        else
            this->setDirty(true);
    }
}

void MainWindow::updateNames(){
    if(!this->itemEditorDisabled){
        QString newName = this->nameEdit->text();

        if(newName != this->currName){
            this->currName = newName;

            foreach(QNNodeItemPtr n, this->nodeList){
                n->getNode()->setName(newName);
                n->updateNode();
            }

            foreach(QNModelItemPtr m, this->modelList){
                m->getModel()->setName(newName);
                m->updateModel();
            }

            this->setDirty(true);
        }
    }
}

void MainWindow::updateComments(){
    if(!this->itemEditorDisabled){
        QString newComment = this->commentEdit->text();

        if(newComment != this->currComment){
            this->currComment = newComment;

            foreach(QNNodeItemPtr n, this->nodeList){
                n->getNode()->setComment(newComment);
                n->updateNode();
            }

            foreach(QNModelItemPtr m, this->modelList){
                m->getModel()->setComment(newComment);
                m->updateModel();
            }

            foreach(QNTextBoxItemPtr t, this->textboxList){
                t->getTextbox()->setComment(newComment);
                t->updateTextbox();
            }

            foreach (QNEdgeItemPtr e, this->edgeList) {
                e->getEdge()->setComment(newComment);
                e->updateEdge();
            }

            this->setDirty(true);
        }
    }
}

void MainWindow::updateCaptions(){
    if(!this->itemEditorDisabled){
        QString newCaption = this->captionEdit->text();

        if(newCaption != this->currCaption){
            this->currCaption = newCaption;

            foreach(QNTextBoxItemPtr t, this->textboxList){
                t->getTextbox()->setCaption(newCaption);
                t->updateTextbox();
            }

            this->setDirty(true);
        }
    }
}

void MainWindow::updateBackColors(int aNewIndex){
    if(!this->itemEditorDisabled){
        if(aNewIndex != 0){
            QColor col = QColor(this->colorNames[aNewIndex]);
            QString newColor = col.name().mid(1);

            foreach(QNNodeItemPtr n, this->nodeList){
                n->getNode()->setColor(newColor);
                n->updateNode();
            }

            foreach(QNModelItemPtr m, this->modelList){
                m->getModel()->setColor(newColor);
                m->updateModel();
            }
        }

        this->setDirty(true);
    }
}

void MainWindow::updateOutlineColors(int aNewIndex){
    if(!this->itemEditorDisabled){
        if(aNewIndex != 0){
            QColor col = QColor(this->colorNames[aNewIndex]);
            QString newColor = col.name().mid(1);

            foreach(QNNodeItemPtr n, this->nodeList){
                n->getNode()->setOutlineColor(newColor);
                n->updateNode();
            }

            foreach(QNModelItemPtr m, this->modelList){
                m->getModel()->setOutlineColor(newColor);
                m->updateModel();
            }
        }

        this->setDirty(true);
    }
}

void MainWindow::updateFontColors(int aNewIndex){
    if(!this->itemEditorDisabled){
        if(aNewIndex != 0){
            QColor col = QColor(this->colorNames[aNewIndex]);
            QString newColor = col.name().mid(1);

            foreach(QNNodeItemPtr n, this->nodeList){
                n->getNode()->getFont()->setFontColor(newColor);
                n->updateNode();
            }

            foreach(QNModelItemPtr m, this->modelList){
                m->getModel()->getFont()->setFontColor(newColor);
                m->updateModel();
            }

            foreach(QNTextBoxItemPtr t, this->textboxList){
                t->getTextbox()->getFont()->setFontColor(newColor);
                t->updateTextbox();
            }
        }

        this->setDirty(true);
    }
}

void MainWindow::updateOutlineWidths(int aNewValue){
    if(!this->itemEditorDisabled){
        foreach(QNNodeItemPtr n, this->nodeList){
            n->getNode()->setOutlineWidth(aNewValue);
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->getModel()->setOutlineWidth(aNewValue);
            m->updateModel();
        }

        this->setDirty(true);
    }
}

void MainWindow::updateFontSizes(int aNewValue){
    if(!this->itemEditorDisabled){
        foreach(QNNodeItemPtr n, this->nodeList){
            n->getNode()->getFont()->setFontSize(aNewValue);
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->getModel()->getFont()->setFontSize(aNewValue);
            m->updateModel();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->getTextbox()->getFont()->setFontSize(aNewValue);
            t->updateTextbox();
        }

        this->setDirty(true);
    }
}

void MainWindow::updateFontBold(bool aChecked){
    if(!this->itemEditorDisabled){
        foreach(QNNodeItemPtr n, this->nodeList){
            n->getNode()->getFont()->setBold(aChecked);
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->getModel()->getFont()->setBold(aChecked);
            m->updateModel();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->getTextbox()->getFont()->setBold(aChecked);
            t->updateTextbox();
        }

        this->setDirty(true);
    }
}

void MainWindow::updateFontItalic(bool aChecked){
    if(!this->itemEditorDisabled){
        foreach(QNNodeItemPtr n, this->nodeList){
            n->getNode()->getFont()->setItalic(aChecked);
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->getModel()->getFont()->setItalic(aChecked);
            m->updateModel();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->getTextbox()->getFont()->setItalic(aChecked);
            t->updateTextbox();
        }

        this->setDirty(true);
    }
}

void MainWindow::updateFontNames(QFont aNewFont){
    if(!this->itemEditorDisabled){
        foreach(QNNodeItemPtr n, this->nodeList){
            n->getNode()->getFont()->setFontName(aNewFont.family());
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->getModel()->getFont()->setFontName(aNewFont.family());
            m->updateModel();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->getTextbox()->getFont()->setFontName(aNewFont.family());
            t->updateTextbox();
        }

        this->setDirty(true);
    }
}

void MainWindow::updateAlign(int aNewIndex){
    if(!this->itemEditorDisabled){
        QNAlignment a = (QNAlignment)aNewIndex;

        foreach(QNNodeItemPtr n, this->nodeList){
            n->getNode()->getFont()->setAlign(a);
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->getModel()->getFont()->setAlign(a);
            m->updateModel();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->getTextbox()->getFont()->setAlign(a);
            t->updateTextbox();
        }

        this->setDirty(true);
    }
}

void MainWindow::updateLengths(int aNewValue){
    if(!this->itemEditorDisabled){
        foreach(QNNodeItemPtr n, this->nodeList){
            n->setLabelLen(aNewValue);
            n->updateNode();
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->setLabelLen(aNewValue);
            m->updateModel();
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->setLabelLen(aNewValue);
            t->updateTextbox();
        }

        if(!this->nodeList.isEmpty() || !this->modelList.isEmpty() ||
           !this->textboxList.isEmpty() || !this->edgeList.isEmpty())
            this->setDirty(true, false);
    }
}

void MainWindow::updatePropCombo(int aNewIndex){
    if(!this->itemEditorDisabled){
        QString p = this->propCombo->itemText(aNewIndex);
        foreach(QNNodeItemPtr n, this->nodeList){
            n->updatePropToShow(p);
        }

        foreach(QNModelItemPtr m, this->modelList){
            m->updatePropToShow(p);
        }

        foreach(QNTextBoxItemPtr t, this->textboxList){
            t->updatePropToShow(p);
        }

        if(!this->nodeList.isEmpty() || !this->modelList.isEmpty() ||
           !this->textboxList.isEmpty() || !this->edgeList.isEmpty())
            this->setDirty(true, false);
    }
}

void MainWindow::newClass(bool aChecked){
    Q_UNUSED(aChecked);
    QNStylePtr style = this->styleMgr->getStyle();
    if(style == nullptr){
        this->styleMgr->newStyle("style.xqsf");
        this->initClassesCombo();
        style = this->styleMgr->getStyle();
    }

    if(!this->styleEditorDisabled){
        QString name = (this->classCounter>0)? QString("New Class %1").arg(++this->classCounter): "New Class";
        QNStyleClassPtr c = nullptr;
        do{
            c = style->getClass(name);
            if(c != nullptr)
                name = QString("New Class %1").arg(++this->classCounter);
        }while(c != nullptr);

        c = new QNStyleClass(name);
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
        int pos = this->classesCombo->count();
        this->classesCombo->insertItem(pos, c->getName());
        this->classesCombo->setCurrentIndex(pos);
        this->setDirty(true, true);
    }
}

void MainWindow::delClass(bool aChecked){
    Q_UNUSED(aChecked);
    QNStylePtr style = this->styleMgr->getStyle();
    if(!this->styleEditorDisabled && style != nullptr){
        int pos = this->classesCombo->currentIndex();
        if(pos > 0){
            QString name = this->classesCombo->itemText(pos);
            style->removeClass(name);
            this->classesCombo->setCurrentIndex(pos-1);
            this->classesCombo->removeItem(pos);
            this->setDirty(true, true);

            if(!this->nodeList.isEmpty() || !this->modelList.isEmpty() ||
               !this->textboxList.isEmpty() || !this->edgeList.isEmpty())
                this->setDirty(true, false);

            this->updateAll();
        }
    }
}

void MainWindow::addClass(bool aChecked){
    Q_UNUSED(aChecked);
    if(!this->styleEditorDisabled){
        int pos = this->classesCombo->currentIndex();
        QString name = this->classesCombo->itemText(pos);

        foreach(QNNodeItemPtr n, this->nodeList)
            n->getNode()->addClass(name);

        foreach(QNModelItemPtr m, this->modelList)
            m->getModel()->addClass(name);

        foreach(QNTextBoxItemPtr t, this->textboxList)
            t->getTextbox()->addClass(name);

        foreach (QNEdgeItemPtr e, this->edgeList)
            e->getEdge()->addClass(name);

        if(!this->nodeList.isEmpty() || !this->modelList.isEmpty() ||
           !this->textboxList.isEmpty() || !this->edgeList.isEmpty())
            this->setDirty(true, false);

        this->updateAll();
    }
}

void MainWindow::remClass(bool aChecked){
    Q_UNUSED(aChecked);
    if(!this->styleEditorDisabled){
        int pos = this->classesCombo->currentIndex();
        QString name = this->classesCombo->itemText(pos);

        foreach(QNNodeItemPtr n, this->nodeList)
            n->getNode()->remClass(name);

        foreach(QNModelItemPtr m, this->modelList)
            m->getModel()->remClass(name);

        foreach(QNTextBoxItemPtr t, this->textboxList)
            t->getTextbox()->remClass(name);

        foreach (QNEdgeItemPtr e, this->edgeList)
            e->getEdge()->remClass(name);

        this->updateAll();
    }
}

void MainWindow::updateClassCombo(int aIndex){
    this->styleEditorDisabled = true;
    QNStylePtr style = this->styleMgr->getStyle();
    QString name = "";
    if(style != nullptr && aIndex > 0){
        name = this->classesCombo->itemText(aIndex);
        this->selClass = style->getClass(name);
    }
    else
        this->selClass = nullptr;

    if(this->selClass != nullptr){
        //name
        this->classNameEdit->setText(name);
        this->classNameEdit->setEnabled(true);

        //background color
        QColor color = QColor("#"+this->selClass->getProperty(STYLE_CLASS_BACKGROUND_COLOR).toString());
        int c = 0;
        if(color.isValid()){
            for (int i = 1; i < this->colorNames.size(); i++) {
                QString tmpName = QColor(this->colorNames[i]).name();
                if(tmpName == color.name()){
                    c = i;
                    break;
                }
            }
        }
        this->classBackColCombo->setCurrentIndex(c);
        this->classBackColCombo->setEnabled(true);

        //back brush
        this->classBackBrushCombo->setCurrentIndex(this->selClass->getProperty(STYLE_CLASS_BACKGROUND_BRUSH).value<BSBrushStyle>());
        this->classBackBrushCombo->setEnabled(true);

        //outline color
        color = QColor("#"+this->selClass->getProperty(STYLE_CLASS_OUTLINE_COLOR).toString());
        c = 0;
        if(color.isValid()){
            for (int i = 1; i < this->colorNames.size(); i++) {
                QString tmpName = QColor(this->colorNames[i]).name();
                if(tmpName == color.name()){
                    c = i;
                    break;
                }
            }
        }
        this->classOutColCombo->setCurrentIndex(c);
        this->classOutColCombo->setEnabled(true);

        //outline size
        this->classOutSizeSpin->setValue(this->selClass->getProperty(STYLE_CLASS_OUTLINE_WIDTH).toInt());
        this->classOutSizeSpin->setEnabled(true);

        //outline line style
        this->classOutLCombo->setCurrentIndex(this->selClass->getProperty(STYLE_CLASS_OUTLINE_LINE).value<BSLineStyle>());
        this->classOutLCombo->setEnabled(true);

        //outline cap
        int index = 0;
        BSCapStyle cap = this->selClass->getProperty(STYLE_CLASS_OUTLINE_CAP).value<BSCapStyle>();
        switch (cap) {
            case BSCapStyle::squareCap:
                index = 1;
                break;
            case BSCapStyle::roundCap:
                index = 2;
                break;
            default:
                index = 0;
                break;
        }
        this->classOutCapCombo->setCurrentIndex(index);
        this->classOutCapCombo->setEnabled(true);

        //outline join
        BSJoinStyle join = this->selClass->getProperty(STYLE_CLASS_OUTLINE_JOIN).value<BSJoinStyle>();
        switch (join) {
            case BSJoinStyle::bevelJoin:
                index = 1;
                break;
            case BSJoinStyle::roundJoin:
                index = 2;
                break;
            default:
                index = 0;
                break;
        }
        this->classOutJoinCombo->setCurrentIndex(index);
        this->classOutJoinCombo->setEnabled(true);

        //shape
        BSNodeShape ns = this->selClass->getProperty(STYLE_CLASS_SHAPE).value<BSNodeShape>();
        this->classShapeCombo->setCurrentIndex(ns);
        this->classShapeCombo->setEnabled(true);
        if(ns == BSNodeShape::image){
            this->classShapePathEdit->setText(this->selClass->getProperty(STYLE_CLASS_SHAPE_PATH).toString());
            this->classShapePathEdit->setEnabled(true);
            this->classShapePathButton->setEnabled(true);
            this->classBackColCombo->setEnabled(false);
            this->classBackBrushCombo->setEnabled(false);
            this->classOutColCombo->setEnabled(false);
            this->classOutSizeSpin->setEnabled(false);
            this->classOutLCombo->setEnabled(false);
            this->classOutCapCombo->setEnabled(false);
            this->classOutJoinCombo->setEnabled(false);
        }
        else{
            this->classShapePathEdit->setText("");
            this->classShapePathEdit->setEnabled(false);
            this->classShapePathButton->setEnabled(false);
            this->classBackColCombo->setEnabled(true);
            this->classBackBrushCombo->setEnabled(true);
            this->classOutColCombo->setEnabled(true);
            this->classOutSizeSpin->setEnabled(true);
            this->classOutLCombo->setEnabled(true);
            this->classOutCapCombo->setEnabled(true);
            this->classOutJoinCombo->setEnabled(true);
        }

        //arrow
        this->classArrowCombo->setCurrentIndex(this->selClass->getProperty(STYLE_CLASS_ARROW).value<BSArrowStyle>());
        this->classArrowCombo->setEnabled(true);

        //font name
        QFont f(this->selClass->getProperty(STYLE_CLASS_FONT_NAME).toString());
        this->classFontCombo->setCurrentFont(f);
        this->classFontCombo->setEnabled(true);

        //font color
        color = QColor("#"+this->selClass->getProperty(STYLE_CLASS_FONT_COLOR).toString());
        c = 0;
        if(color.isValid()){
            for (int i = 1; i < this->colorNames.size(); i++) {
                QString tmpName = QColor(this->colorNames[i]).name();
                if(tmpName == color.name()){
                    c = i;
                    break;
                }
            }
        }
        this->classFontColCombo->setCurrentIndex(c);
        this->classFontColCombo->setEnabled(true);

        //font size
        this->classFontSizeSpin->setValue(this->selClass->getProperty(STYLE_CLASS_FONT_SIZE).toInt());
        this->classFontSizeSpin->setEnabled(true);

        //font align
        this->classAlignCombo->setCurrentIndex(this->selClass->getProperty(STYLE_CLASS_FONT_ALIGN).value<BSTextAlign>());
        this->classAlignCombo->setEnabled(true);

        //font bold
        this->classBoldButton->setChecked(this->selClass->getProperty(STYLE_CLASS_FONT_BOLD).toBool());
        this->classBoldButton->setEnabled(true);

        //font italic
        this->classItalicButton->setChecked(this->selClass->getProperty(STYLE_CLASS_FONT_ITALIC).toBool());
        this->classItalicButton->setEnabled(true);
    }
    else{
        this->classNameEdit->setText(name);
        this->classNameEdit->setEnabled(false);
        this->classBackColCombo->setCurrentIndex(WHITE_IDX);
        this->classBackColCombo->setEnabled(false);
        this->classBackBrushCombo->setCurrentIndex(BSBrushStyle::solid);
        this->classBackBrushCombo->setEnabled(false);
        this->classOutColCombo->setCurrentIndex(BLACK_IDX);
        this->classOutColCombo->setEnabled(false);
        this->classOutSizeSpin->setValue(STYLE_DEFAULT_WIDTH);
        this->classOutSizeSpin->setEnabled(false);
        this->classOutLCombo->setCurrentIndex(BSLineStyle::solidLine);
        this->classOutLCombo->setEnabled(false);
        this->classOutCapCombo->setCurrentIndex(0);
        this->classOutCapCombo->setEnabled(false);
        this->classOutJoinCombo->setCurrentIndex(0);
        this->classOutJoinCombo->setEnabled(false);
        this->classShapeCombo->setCurrentIndex(BSNodeShape::rectagle);
        this->classShapeCombo->setEnabled(false);
        this->classShapePathEdit->setText("");
        this->classShapePathEdit->setEnabled(false);
        this->classShapePathButton->setEnabled(false);
        this->classArrowCombo->setCurrentIndex(BSArrowStyle::full);
        this->classArrowCombo->setEnabled(false);
        this->classFontCombo->setCurrentFont(QFont(STYLE_DEFAULT_FONT_NAME));
        this->classFontCombo->setEnabled(false);
        this->classFontColCombo->setCurrentIndex(BLACK_IDX);
        this->classFontColCombo->setEnabled(false);
        this->classFontSizeSpin->setValue(STYLE_DEFAULT_FONT_SIZE);
        this->classFontSizeSpin->setEnabled(false);
        this->classAlignCombo->setCurrentIndex(BSTextAlign::textLeft);
        this->classAlignCombo->setEnabled(false);
        this->classBoldButton->setChecked(false);
        this->classBoldButton->setEnabled(false);
        this->classItalicButton->setChecked(false);
        this->classItalicButton->setEnabled(false);
    }
    this->styleEditorDisabled = false;
}

void MainWindow::updateClassName(){
    if(!this->styleEditorDisabled){
        QNStylePtr style = this->styleMgr->getStyle();
        int pos = this->classesCombo->currentIndex();
        if(style != nullptr && pos > 0)
            this->selClass = style->getClass(this->classesCombo->itemText(pos));
        else
            this->selClass = nullptr;

        QString newName = this->classNameEdit->text();
        if(this->selClass != nullptr && this->selClass->getName() != newName){
            this->selClass = style->moveClass(this->selClass->getName(), newName);
            this->classesCombo->setItemText(pos, newName);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassBackCol(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr && aIndex != 0){
            QColor col = QColor(this->colorNames[aIndex]);
            QString newColor = col.name().mid(1);
            this->selClass->setProperty(STYLE_CLASS_BACKGROUND_COLOR, newColor);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassBackBrush(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_BACKGROUND_BRUSH, (BSBrushStyle)aIndex);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassOutCol(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            if(this->selClass != nullptr && aIndex != 0){
                QColor col = QColor(this->colorNames[aIndex]);
                QString newColor = col.name().mid(1);
                this->selClass->setProperty(STYLE_CLASS_OUTLINE_COLOR, newColor);
                this->updateAll();
                this->setDirty(true, true);
            }
        }
    }
}

void MainWindow::updateClassOutSize(int aValue){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_OUTLINE_WIDTH, aValue);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassOutL(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_OUTLINE_LINE, (BSLineStyle)aIndex);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassOutCap(int aIndex){
    if(!this->styleEditorDisabled){
        BSCapStyle cap = this->classOutCapCombo->itemData(aIndex).value<BSCapStyle>();
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_OUTLINE_CAP, cap);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassOutJoin(int aIndex){
    if(!this->styleEditorDisabled){
        BSJoinStyle join = this->classOutJoinCombo->itemData(aIndex).value<BSJoinStyle>();
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_OUTLINE_JOIN, join);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassFont(QFont aNewFont){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_FONT_NAME, aNewFont.family());
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassAlign(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_FONT_ALIGN, (BSTextAlign)aIndex);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassFontBold(bool aChecked){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_FONT_BOLD, aChecked);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassFontItalic(bool aChecked){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_FONT_ITALIC, aChecked);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassFontCol(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr && aIndex != 0){
            QColor col = QColor(this->colorNames[aIndex]);
            QString newColor = col.name().mid(1);
            this->selClass->setProperty(STYLE_CLASS_FONT_COLOR, newColor);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassFontSize(int aValue){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_FONT_SIZE, aValue);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::updateClassShape(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_SHAPE, (BSNodeShape)aIndex);
            this->updateAll();
            this->setDirty(true, true);
            if((BSNodeShape)aIndex == BSNodeShape::image){
                this->classShapePathEdit->setText(this->selClass->getProperty(STYLE_CLASS_SHAPE_PATH).toString());
                this->classShapePathEdit->setEnabled(true);
                this->classShapePathButton->setEnabled(true);
                this->classBackColCombo->setEnabled(false);
                this->classBackBrushCombo->setEnabled(false);
                this->classOutColCombo->setEnabled(false);
                this->classOutSizeSpin->setEnabled(false);
                this->classOutLCombo->setEnabled(false);
                this->classOutCapCombo->setEnabled(false);
                this->classOutJoinCombo->setEnabled(false);
            }
            else{
                this->classShapePathEdit->setText("");
                this->classShapePathEdit->setEnabled(false);
                this->classShapePathButton->setEnabled(false);
                this->classBackColCombo->setEnabled(true);
                this->classBackBrushCombo->setEnabled(true);
                this->classOutColCombo->setEnabled(true);
                this->classOutSizeSpin->setEnabled(true);
                this->classOutLCombo->setEnabled(true);
                this->classOutCapCombo->setEnabled(true);
                this->classOutJoinCombo->setEnabled(true);
            }
        }
    }
}

void MainWindow::updateImage(){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        QString path = this->classShapePathEdit->text();
        if(this->selClass != nullptr && !path.isEmpty()){
            this->selClass->setProperty(STYLE_CLASS_SHAPE_PATH, path);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

void MainWindow::openChooseImage(bool aChecked){
    Q_UNUSED(aChecked);
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            QString path = this->selClass->getProperty(STYLE_CLASS_SHAPE_PATH).toString();
            path = QFileDialog::getOpenFileName(this, tr("Choose an Image"), path, tr("Images (*.png *.xpm *.jpg *.jpeg)"));
            this->classShapePathEdit->setText(path);
            this->updateImage();
        }
    }
}

void MainWindow::updateClassArrow(int aIndex){
    if(!this->styleEditorDisabled){
        this->updateSelClass();
        if(this->selClass != nullptr){
            this->selClass->setProperty(STYLE_CLASS_ARROW, (BSArrowStyle)aIndex);
            this->updateAll();
            this->setDirty(true, true);
        }
    }
}

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent){
#ifdef QT_DEBUG
    this->persistMgr = new QNPersistenceManager(QDir("../../Plugin/SmilePersistencyPlugin/Debug/"));
    this->styleMgr = new QNStyleManager(QDir("../../Plugin/BaseStylePlugin/Debug/"));
#else
    QDir pluginDir = QDir(QCoreApplication::applicationDirPath());
    pluginDir.cd("Plugins");
    this->persistMgr = new QNPersistenceManager(pluginDir);
    this->styleMgr = new QNStyleManager(pluginDir);
#endif
    this->styleFileName = "";
    this->styleMgr->resetStyle();

    this->scene = new QNNetworkScene(this->styleMgr, this);
    this->view = new QNNetworkView(this->scene, this);

    connect(this->scene, SIGNAL(modified()), this, SLOT(sceneModified()));
    connect(this->scene, SIGNAL(itemsSelected(QNNodeItemList, QNModelItemList, QNTextBoxItemList, QNEdgeItemList)),
            this, SLOT(updateSelected(QNNodeItemList, QNModelItemList, QNTextBoxItemList, QNEdgeItemList)));

    this->initDocks();
    this->initMenus();

    this->scene->updateSceneCtxMenu(this->sceneMenu);
    this->scene->updateOneNodeSelectedCtxMenu(this->nodeMenu);
    this->scene->updateManyNodesSelectedCtxMenu(this->manyNodesMenu);
    this->scene->updateOneEdgeSelectedCtxMenu(this->edgeMenu);
    this->scene->updateManyEdgesSelectedCtxMenu(this->edgeMenu);
    this->scene->updateOneModelSelectedCtxMenu(this->modelMenu);
    this->scene->updateManyModelsSelectedCtxMenu(this->manyModelsMenu);
    this->scene->updateOneTextboxSelectedCtxMenu(this->textboxMenu);
    this->scene->updateManyTextboxesSelectedCtxMenu(this->manyMenu);
    this->scene->updateManySelectedCtxMenu(this->manyMenu);

    this->initNet();

    this->setCentralWidget(this->view);
    this->setUnifiedTitleAndToolBarOnMac(true);
}

MainWindow::~MainWindow(){
    if(this->dirty){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("The network has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Save);
        if(msgBox.exec() == QMessageBox::Save)
            this->saveFile();
    }

    if(this->styleDirty){
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Question);
        msgBox.setText("The style sheet has been modified.");
        msgBox.setInformativeText("Do you want to save your changes?");
        msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard);
        msgBox.setDefaultButton(QMessageBox::Save);
        if(msgBox.exec() == QMessageBox::Save)
            this->saveStyle();
    }

    delete this->persistMgr;
    delete this->styleMgr;
}
