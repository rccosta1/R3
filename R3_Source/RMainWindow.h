#ifndef RMAINWINDOW_H
#define RMAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QActionGroup>
#include <QDockWidget>

class RObjectWidget;
class RDomain;
class RPropertyWidget;
class QPlainTextEdit;
class QAction;
class QListWidget;
class QTreeWidget;
class QWidget;
class QToolBox;
class QPlainTextEdit;
class QTabBar;
class QMdiArea;
class QMdiSubWindow;
class QProgressBar;

QT_BEGIN_NAMESPACE
namespace Ui { class RMainWindow; }
QT_END_NAMESPACE

class RMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    RMainWindow(QWidget *parent = nullptr);
    ~RMainWindow();

    //void createCentralWidget();

public slots:
    void raiseDock(QString dockName);
    void updateTree(QString treeName);
    void collapseTree(QString treeName);
    void expandTree(QString treeName);
    void updatePropertyList();
    void clearAllObjects(bool showDialogBox = true);
    void openFile(QString fileName, bool StInputFile = false);
    QObjectList openRtsFile(QString fileName, bool StInputFile);
    void addSubWindow(QWidget *widget, QString windowTitle, bool tiled = false);
    QMdiSubWindow *getSubWindow(QWidget *widget);
    void setActiveSubWindow(QWidget *widget);
    void collapseAllTrees();
    void expandAllTrees();
    void closePropertyBox();
    void closeOutputBox();
    //QMdiSubWindow *getSubWindow(QWidget *widget);

signals:
    void subWindowAdded(QWidget *subWindow);

private slots:

    void closeEvent(QCloseEvent *event);
    void raisePropertieDock();
    void about();
    void togglePanes();
    //void showHideToolBars();
    void writeSettings(bool defaultFlag);
    void readSettings(bool defaultFlag);
    void setDefaultView();
    void resetView();
    void open();
    void save();
    void openRecentFile();
    void cutText();
    void copyText();
    void pasteText();
    void deleteAllText();
    void tileWindows();
    void cascadeWindows();
    void closeAll();
    void help();
    void createOpeningScreen();
    void closeActiveSubWindow();
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    //void updateStatusBarMouseCoordinates(float &x,float &y);

private:
    void createCentralWidget();
    void createActions();
    void createMenus();
    void createStatusBar();
    void createDockWindows();
    void createConnections();
    void resetDomain();
    void onFileOpenError(QObjectList *objectList, QString oldFilePath);
    void updateRecentFileList(QString newFileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

    QDockWidget *propertiesDock;
    QDockWidget *consoleDock;
    QDockWidget *objectsDock;
    RPropertyWidget *propertyList;
    RObjectWidget *objectsTree;
    QPlainTextEdit *console;

    Ui::RMainWindow *ui;
    RDomain *theDomain;

    QLabel *infoLabel;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *windowMenu;
    QMenu *helpMenu;
    QMenu *panesMenu;
    QMenu *unitsMenu;
    QMenu *toolBarsMenu;
    QMenu *recentFilesMenu;

    QAction *clearAllObjectAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *exitAction;
    QAction *cutAction;
    QAction *setUnitsNmmkgsAction;
    QAction *setUnitskNmkgsAction;
    QAction *setUnitslbsinsAction;
    QAction *setUnitskipsftsAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAllAction;
    QAction *toggleAllPanesAction;
    QAction *toggleAllToolBarsAction;
    QAction *resetViewAction;
    QAction *setDefaultViewAction;
    QAction *collapseTreesAction;
    QAction *expandTreesAction;
    //QAction *viewMapAction;
    QAction *overlayImageOnMapAction;
    QAction *createLocationsFromMapAction;
    QAction *variableInferenceAction;
    QAction *modelInferenceAction;
    QAction *tileWindowsAction;
    QAction *cascadeWindowsAction;
    QAction *closeAllAction;
    QAction *helpAction;
    QAction *aboutAction;
    enum {MaxRecentFiles = 10};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *viewToolBar;
    QToolBar *windowToolBar;
    QToolBar *helpToolBar;

    QMdiArea *mdiArea;
    QProgressBar *progressBar;

    bool closeWithoutWarning;

};
#endif // RMAINWINDOW_H
