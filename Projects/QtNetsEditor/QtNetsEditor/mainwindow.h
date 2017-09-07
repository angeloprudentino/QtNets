/**
 ** @file mainwindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMessageBox>
#include <QDockWidget>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QFontComboBox>
#include <QPushButton>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include "qnnetworkview.h"
#include "qnnetworkscene.h"
#include "qntypes.h"
#include "qndrawtypes.h"
#include "qnpersistencetypes.h"
#include "qnpersistencemanager.h"
#include "qnstyletypes.h"
#include "qnstylemanager.h"


class MainWindow : public QMainWindow{
    Q_OBJECT
private:
    ScopedQNNetwork net;
    QNNetworkScenePtr scene;
    QNNetworkViewPtr view;
    QNPersistenceManagerPtr persistMgr;
    QNStyleManagerPtr styleMgr;
    QString fileName = "";
    QString styleFileName = "";
    QString imgName = "";
    bool dirty = false;
    bool styleDirty = false;
    bool toRemeber = false;
    int choice = QMessageBox::Yes;
    double zoomLevel = 0;
    double rotateLevel = 0;
    QNNodeItemPtr rememberedNode = nullptr;

    //items
    QNNodeItemList nodeList;
    QNModelItemList modelList;
    QNTextBoxItemList textboxList;
    QNEdgeItemList edgeList;

    //class
    QNStyleClassPtr selClass = nullptr;
    int classCounter = 0;

    //menus
    QMenuPtr fileMenu = nullptr;
    QMenuPtr viewMenu = nullptr;
    QMenuPtr itemsMenu = nullptr;
    QMenuPtr styleMenu = nullptr;
    QMenuPtr settingsMenu = nullptr;
    QMenuPtr helpMenu = nullptr;
    QMenuPtr sceneMenu = nullptr;
    QMenuPtr nodeMenu = nullptr;
    QMenuPtr manyNodesMenu = nullptr;
    QMenuPtr manyModelsMenu = nullptr;
    QMenuPtr edgeMenu = nullptr;
    QMenuPtr modelMenu = nullptr;
    QMenuPtr textboxMenu = nullptr;
    QMenuPtr manyMenu = nullptr;

    //actions
    QActionPtr openAction = nullptr;
    QActionPtr reloadAction = nullptr;
    QActionPtr saveAction = nullptr;
    QActionPtr saveAsAction = nullptr;
    QActionPtr closeAction = nullptr;
    QActionPtr exportAction = nullptr;
    QActionPtr printAction = nullptr;
    QActionPtr exitAction = nullptr;
    QActionPtr restoreZoomAction = nullptr;
    QActionPtr zoomInAction = nullptr;
    QActionPtr zoomOutAction = nullptr;
    QActionPtr restoreRotateAction = nullptr;
    QActionPtr rotateLeftAction = nullptr;
    QActionPtr rotateRightAction = nullptr;
    QActionPtr rotateLeft90Action = nullptr;
    QActionPtr rotateRight90Action = nullptr;
    QActionPtr selectAllAction = nullptr;
    QActionPtr deleteAction = nullptr;
    QActionPtr toFrontAction = nullptr;
    QActionPtr sendBackAction = nullptr;
    QActionPtr newNodeAction = nullptr;
    QActionPtr newModelAction = nullptr;
    QActionPtr toggleContentAction = nullptr;
    QActionPtr newTextboxAction = nullptr;
    QActionPtr newEdgeAction = nullptr;
    QActionPtr rememberNodeAction = nullptr;
    QActionPtr newCicleAction = nullptr;
    QActionPtr openStyleAction = nullptr;
    QActionPtr reloadStyleAction = nullptr;
    QActionPtr saveStyleAction = nullptr;
    QActionPtr saveAsStyleAction = nullptr;
    QActionPtr closeStyleAction = nullptr;
    QActionPtr rememberOnDeleteAction = nullptr;
    QActionPtr aboutAction = nullptr;

    //docks
    QStringList colorNames = QColor::colorNames();
    QDockWidget* itemPropDock = nullptr;
    QDockWidget* styleEditDock = nullptr;
    bool itemEditorDisabled;
    bool styleEditorDisabled;
    QWidgetPtr itemPropWidget = nullptr;
    QWidgetPtr styleEditorWidget = nullptr;
    //itemPropDock
    QFormLayout* itemPropLayout = nullptr;
    QGridLayout* idLayout = nullptr;
    QLabel* idLabel = nullptr;
    QLineEdit* idEdit = nullptr;
    QPushButton* randIdButton = nullptr;
    QString currId;
    QLabel* nameLabel = nullptr;
    QLineEdit* nameEdit = nullptr;
    QString currName;
    QLabel* captionLabel = nullptr;
    QLineEdit* captionEdit = nullptr;
    QString currCaption;
    QLabel* propLabel = nullptr;
    QComboBox* propCombo = nullptr;
    QLabel* lengthLabel = nullptr;
    QSpinBox* lengthSpin = nullptr;
    QLabel* backColComboLabel = nullptr;
    QComboBox* backColCombo = nullptr;
    QLabel* outlineLabel = nullptr;
    QGridLayout* outlineLayout = nullptr;
    QComboBox* outColCombo = nullptr;
    QLabel* outWSpinLabel = nullptr;
    QSpinBox* outWSpin = nullptr;
    QLabel* fontLabel = nullptr;
    QGridLayout* fontLayout = nullptr;
    QFontComboBox* fontCombo = nullptr;
    QPushButton* boldButton = nullptr;
    QPushButton* italicButton = nullptr;
    QComboBox* fontColCombo = nullptr;
    QSpinBox* fontSizeSpin = nullptr;
    QComboBox* alignCombo = nullptr;
    QLabel* commentLabel = nullptr;
    QLineEdit* commentEdit = nullptr;
    QString currComment;
    //styleEditDock
    QFormLayout* styleEditLayout = nullptr;
    QGridLayout* classesLayout = nullptr;
    QLabel* classesLabel = nullptr;
    QComboBox* classesCombo = nullptr;
    QPushButton* newClassButton = nullptr;
    QPushButton* delClassButton = nullptr;
    QPushButton* addClassButton = nullptr;
    QPushButton* remClassButton = nullptr;
    QLabel* classDetailLabel = nullptr;
    QLabel* classNameLabel = nullptr;
    QLineEdit* classNameEdit = nullptr;
    QLabel* classBackLabel = nullptr;
    QGridLayout* classBackLayout = nullptr;
    QComboBox* classBackColCombo = nullptr;
    QComboBox* classBackBrushCombo = nullptr;
    QLabel* classOutlineLabel = nullptr;
    QGridLayout* classOutLayout = nullptr;
    QComboBox* classOutColCombo = nullptr;
    QSpinBox* classOutSizeSpin = nullptr;
    QComboBox* classOutLCombo = nullptr;
    QComboBox* classOutCapCombo = nullptr;
    QComboBox* classOutJoinCombo = nullptr;
    QLabel* classFontLabel = nullptr;
    QGridLayout* classFontLayout = nullptr;
    QFontComboBox* classFontCombo = nullptr;
    QPushButton* classBoldButton = nullptr;
    QPushButton* classItalicButton = nullptr;
    QComboBox* classFontColCombo = nullptr;
    QSpinBox* classFontSizeSpin = nullptr;
    QComboBox* classAlignCombo = nullptr;
    QLabel* classShapeLabel = nullptr;
    QGridLayout* classShapeLayout = nullptr;
    QComboBox* classShapeCombo = nullptr;
    QLineEdit* classShapePathEdit = nullptr;
    QPushButton* classShapePathButton = nullptr;
    QLabel* classArrowLabel = nullptr;
    QComboBox* classArrowCombo = nullptr;

    void initActions();
    void initMenus();
    void initDocks();
    void initItemEditor();
    void updateItemEditor();
    void initStyleEditor();

    QString getSupported(bool isStyle = false);
    void save(bool isStyle = false);
    void setDirty(const bool aDirty, bool isStyle = false);
    void initNet();
    void populateNetModel();
    void resetView();
    void resetNet();
    void resetStyle();
    void initClassesCombo();
    void initBackBrushCombo();
    void initOutL();
    void initOutCap();
    void initOutJoin();
    void initClassAlign();
    void initClassShapes();
    void initArrows();
    void updateSelClass();
    void updateAll();

private slots:
    void reload();
    void openFile();
    void saveFile();
    void saveAsFile();
    void closeFile();
    void print();
    void exportAs();
    void rotateLeft90();
    void rotateRight90();
    void rememberOnDelete();
    void deleteItems();
    void rememberNode();
    void newEdge();
    void showWarning(const QString& aWarn);
    void showError(const QString& aError);
    void showAbout();
    void sceneModified();
    void openStyle();
    void reloadStyle();
    void saveStyle();
    void saveAsStyle();
    void closeStyle();
    void updateSelected(QNNodeItemList aNodeList, QNModelItemList aModelList,
                        QNTextBoxItemList aTextboxList, QNEdgeItemList aEdgeList);

    //itemProp
    void updateId();
    void genRandId(bool aChecked);
    void updateNames();
    void updateComments();
    void updateCaptions();
    void updateBackColors(int aNewIndex);
    void updateOutlineColors(int aNewIndex);
    void updateFontColors(int aNewIndex);
    void updateOutlineWidths(int aNewValue);
    void updateFontSizes(int aNewValue);
    void updateFontBold(bool aChecked);
    void updateFontItalic(bool aChecked);
    void updateFontNames(QFont aNewFont);
    void updateAlign(int aNewIndex);
    void updateLengths(int aNewValue);
    void updatePropCombo(int aNewIndex);

    //style
    void newClass(bool aChecked);
    void delClass(bool aChecked);
    void addClass(bool aChecked);
    void remClass(bool aChecked);
    void updateClassCombo(int aIndex);
    void updateClassName();
    void updateClassBackCol(int aIndex);
    void updateClassBackBrush(int aIndex);
    void updateClassOutCol(int aIndex);
    void updateClassOutSize(int aValue);
    void updateClassOutL(int aIndex);
    void updateClassOutCap(int aIndex);
    void updateClassOutJoin(int aIndex);
    void updateClassFont(QFont aNewFont);
    void updateClassAlign(int aIndex);
    void updateClassFontBold(bool aChecked);
    void updateClassFontItalic(bool aChecked);
    void updateClassFontCol(int aIndex);
    void updateClassFontSize(int aValue);
    void updateClassShape(int aIndex);
    void updateImage();
    void openChooseImage(bool aChecked);
    void updateClassArrow(int aIndex);

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
