/*
 *  Copyright 2013 Wang Luming <wlm199558@126.com>
 *  Copyright 2013 Miyanaga Saki <tomguts@126.com>
 *
 *  mainwindow.cpp is part of Kreogist-Cute-IDE.
 *
 *    Kreogist-Cute-IDE is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *    Kreogist-Cute-IDE is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Kreogist-Cute-IDE.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle(tr("Kreogist Cute IDE"));

    tabManager=new kciTabManager(this);
    setCentralWidget(tabManager);
    setContentsMargins(0,0,0,0);
    setMinimumSize(400,150);

    QPalette QPpal = palette();
    QPpal.setBrush(QPalette::Window, QBrush(QColor(83,83,83)));
    setPalette(QPpal);

    setWindowFlags(Qt::FramelessWindowHint);

    restoreSettings();

    createActions();
    createDocks();
    createTitlebar();
    createMenu();
    createStatusbar();
}

void MainWindow::createActions()
{
    //File -> New
    act[mnuFileNewFile]=new QAction(tr("new file"),this);
    act[mnuFileNewFile]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_N));
    actStatusTips[mnuFileNewFile]=QString(tr("Create a new document."));
    connect(act[mnuFileNewFile],SIGNAL(triggered()),
            tabManager,SLOT(new_file()));

    //File -> Open
    act[mnuFileOpen]=new QAction(tr("open"),this);
    act[mnuFileOpen]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_O));
    actStatusTips[mnuFileOpen]=QString(tr("Open an exsisting document."));
    connect(act[mnuFileOpen],SIGNAL(triggered()),tabManager,SLOT(open()));

    //File -> Save
    act[mnuFileSave]=new QAction(tr("save"),this);
    act[mnuFileSave]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_S));
    actStatusTips[mnuFileSave]=QString(tr("Save active document."));
    connect(act[mnuFileSave],SIGNAL(triggered()),tabManager,SLOT(save()));

    //File -> Save As
    act[mnuFileSaveAs]=new QAction(tr("save as"),this);
    act[mnuFileSaveAs]->setShortcut(QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_S));
    actStatusTips[mnuFileSaveAs]=QString(tr("Save as different file name."));
    connect(act[mnuFileSaveAs],SIGNAL(triggered()),tabManager,SLOT(save_as()));

    //File -> Save All
    act[mnuFileSaveAll]=new QAction(tr("save all"),this);
    act[mnuFileSaveAll]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_S));
    actStatusTips[mnuFileSaveAll]=QString(tr("Save all modified documents."));
    connect(act[mnuFileSaveAll],SIGNAL(triggered()),tabManager,SLOT(save_all()));

    //File -> Close
    act[mnuFileClose]=new QAction(tr("close"),this);
    act[mnuFileClose]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_F4));
    actStatusTips[mnuFileClose]=QString(tr("Close the active document."));
    connect(act[mnuFileClose],SIGNAL(triggered()),tabManager,SLOT(close_current_tab()));

    //File -> Close All
    act[mnuFileCloseAll]=new QAction(tr("close all"),this);
    act[mnuFileCloseAll]->setShortcut(QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_F4));
    actStatusTips[mnuFileCloseAll]=QString(tr("Close all files."));
    connect(act[mnuFileCloseAll],SIGNAL(triggered()),tabManager,SLOT(close_all_tab()));

    //File -> Close All Except This
    act[mnuFileCloseAllExceptThis]=new QAction(tr("close all other file"),this);
    act[mnuFileCloseAllExceptThis]->setShortcut(QKeySequence(Qt::CTRL+Qt::ALT+Qt::Key_F4));
    actStatusTips[mnuFileCloseAllExceptThis]=QString(tr("Close all files except active file."));
    connect(act[mnuFileCloseAllExceptThis],SIGNAL(triggered()),tabManager,SLOT(close_all_other_tab()));

    //File -> Exit
    act[mnuFileExit]=new QAction(tr("quit"),this);
    act[mnuFileExit]->setShortcut(QKeySequence(Qt::ALT+Qt::Key_F4));
    actStatusTips[mnuFileExit]=QString(tr("Quit applications; prompts to save documents."));
    connect(act[mnuFileExit],SIGNAL(triggered()),this,SLOT(close()));

    //Edit -> Undo
    act[mnuEditUndo]=new QAction(tr("undo"),this);
    act[mnuEditUndo]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Z));
    actStatusTips[mnuEditUndo]=QString(tr("Undo the last action."));
    connect(act[mnuEditUndo],SIGNAL(triggered()),tabManager,SLOT(undo()));

    //Edit -> Redo
    act[mnuEditRedo]=new QAction(tr("redo"),this);
    act[mnuEditRedo]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Y));
    actStatusTips[mnuEditRedo]=QString(tr("Redo the previously undone action."));
    connect(act[mnuEditRedo],SIGNAL(triggered()),tabManager,SLOT(redo()));

    //Edit -> Cut
    act[mnuEditCut]=new QAction(tr("cut"),this);
    act[mnuEditCut]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_X));
    actStatusTips[mnuEditCut]=QString(tr("Cut the selection and put it on the clipboard."));
    connect(act[mnuEditCut],SIGNAL(triggered()),tabManager,SLOT(cut()));

    //Edit -> Copy
    act[mnuEditCopy]=new QAction(tr("copy"),this);
    act[mnuEditCopy]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_C));
    actStatusTips[mnuEditCopy]=QString(tr("Copy the selection and put it on the clipboard."));
    connect(act[mnuEditCopy],SIGNAL(triggered()),tabManager,SLOT(copy()));

    //Edit -> Paste
    act[mnuEditPaste]=new QAction(tr("paste"),this);
    act[mnuEditPaste]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_V));
    actStatusTips[mnuEditPaste]=QString(tr("Paste clipboard contents."));
    connect(act[mnuEditPaste],SIGNAL(triggered()),tabManager,SLOT(paste()));

    //Edit -> Select All
    act[mnuEditSelectAll]=new QAction(tr("select all"),this);
    act[mnuEditSelectAll]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_A));
    actStatusTips[mnuEditSelectAll]=QString(tr("Select the entire document."));
    connect(act[mnuEditSelectAll],SIGNAL(triggered()),tabManager,SLOT(select_all()));

    //Edit -> Preference
    act[mnuEditPreference]=new QAction(tr("preference"),this);
    act[mnuEditPreference]->setShortcut(QKeySequence(Qt::CTRL+Qt::Key_Comma));
    actStatusTips[mnuEditPreference]=QString(tr("Customize your IDE."));

    //Search -> Search
    act[mnuSearchFind]=new QAction(tr("searchinfile"),this);
    actStatusTips[mnuSearchFind]=QString(tr("Find the specified text."));

    //Search -> Find In Files
    act[mnuSearchFindInFiles]=new QAction(tr("searchallfile"),this);
    actStatusTips[mnuSearchFindInFiles]=QString(tr("Find text in files."));

    //Search -> Replace
    act[mnuSearchReplace]=new QAction(tr("replaceinfile"),this);
    actStatusTips[mnuSearchReplace]=QString(tr("Replace specfic text with different text."));

    //Search -> Replace In Files
    act[mnuSearchReplaceInFiles]=new QAction(tr("replaceallfile"),this);
    actStatusTips[mnuSearchReplaceInFiles]=QString(tr("Replace in files."));

    //Search -> Search Online
    act[mnuSearchSearchOnline]=new QAction(tr("searchonline"),this);
    actStatusTips[mnuSearchSearchOnline]=QString(tr("Search text online."));

    //Search -> Go To Line
    act[mnuSearchGoto]=new QAction(tr("gotoline"),this);
    actStatusTips[mnuSearchGoto]=QString(tr("Goto line."));

    //Run -> Comile And Run
    act[mnuRunCompileAndRun]=new QAction(tr("Compile & Run"),this);
    actStatusTips[mnuRunCompileAndRun]=QString(tr("Compile the active file and run."));

    //Run -> Compile
    act[mnuRunCompile]=new QAction(tr("compile"),this);
    act[mnuRunCompile]->setShortcut(QKeySequence(Qt::Key_F9));
    actStatusTips[mnuRunCompile]=QString(tr("Compile the active file."));
    connect(act[mnuRunCompile],SIGNAL(triggered()),this,SLOT(compileCurrentFile()));

    //Run -> Run
    act[mnuRunRun]=new QAction(tr("Runexe"),this);
    actStatusTips[mnuRunRun]=QString(tr("Run the compiled execution."));

    //Run -> Parameters
    act[mnuRunParameters]=new QAction(tr("Parameters"),this);
    actStatusTips[mnuRunParameters]=QString(tr("Run the compiled execution with parameters."));

    //Run -> Set Input File
    act[mnuRunSetInputFile]=new QAction(tr("Set Input File"),this);
    actStatusTips[mnuRunSetInputFile]=QString(tr("Set the input file contents."));

    //Run -> Show Output File
    act[mnuRunShowOutputFile]=new QAction(tr("Show Output file"),this);
    actStatusTips[mnuRunShowOutputFile]=QString(tr("Show the output file contents."));

    //Run -> Run And Show Output File
    act[mnuRunRunAndShowOutputFile]=new QAction(tr("Run And Show Output File"),this);
    actStatusTips[mnuRunRunAndShowOutputFile]=QString(tr("Run the execution and show output file."));

    //Run -> Set Input, Run and Show Output
    act[mnuRunSetInputRunShowOutput]=new QAction(
                tr("Set Input, Run and show Output"), this);
    actStatusTips[mnuRunSetInputRunShowOutput]=
            QString(tr("Set the input file, compile and run the document, and show output file."));

    //Debug -> Debug Start
    act[mnuDebugStart]=new QAction(tr("Start Debug"),this);
    actStatusTips[mnuDebugStart]=QString(tr("Start debugging the active file."));

    //Debug -> Stop Execute
    act[mnuDebugStopExecute]=new QAction(tr("Stop execute"),this);
    actStatusTips[mnuDebugStopExecute]=QString(tr("Stop the active debugging."));

    //Debug -> Set Break Point
    act[mnuDebugSetBreakPoint]=new QAction(tr("set break point"),this);
    actStatusTips[mnuDebugSetBreakPoint]=QString(tr("Set a break point at the current line."));

    //Debug -> Next Line
    act[mnuDebugNextLine]=new QAction(tr("Next Line"),this);
    actStatusTips[mnuDebugNextLine]=QString(tr("Get into and run the next line."));

    //Debug -> Go Into line
    act[mnuDebugIntoLine]=new QAction(tr("Into Line"),this);
    actStatusTips[mnuDebugIntoLine]=QString(tr("Get into the next line."));

    //Debug -> Go Next Instruction
    act[mnuDebugNextInstruction]=new QAction(tr("Next Instruction"),this);
    actStatusTips[mnuDebugNextInstruction]=QString(tr("Get into and run the next instruction."));

    //Debug -> Go Into Instruction
    act[mnuDebugIntoInstruction]=new QAction(tr("Into Instruction"),this);
    actStatusTips[mnuDebugIntoInstruction]=QString(tr("Get into the next instruction."));

    //Debug -> Skip Line
    act[mnuDebugSkipLine]=new QAction(tr("skip line"),this);
    actStatusTips[mnuDebugSkipLine]=QString(tr("Skip the current line."));

    //Debug -> Skip Function
    act[mnuDebugSkipFunction]=new QAction(tr("skip function"),this);
    actStatusTips[mnuDebugSkipFunction]=QString(tr("Skip the current function."));

    //Debug -> Add Watch
    act[mnuDebugAddWatch]=new QAction(tr("Add Watch"),this);
    actStatusTips[mnuDebugAddWatch]=QString(tr("Add a new variable to debug watch."));

    //Debug -> Modify Watch
    act[mnuDebugModifyWatch]=new QAction(tr("Modify Watch"),this);
    actStatusTips[mnuDebugModifyWatch]=QString(tr("Modify a variable in debug watch list."));

    //Debug -> Remove Watch
    act[mnuDebugRemoveWatch]=new QAction(tr("Remove Watch"),this);
    actStatusTips[mnuDebugRemoveWatch]=QString(tr("Remove a variable in debug watch list."));

    //about
    act[mnuHelpAbout]=new QAction(tr("about"),this);
    actStatusTips[mnuHelpAbout]=QString(tr("Display the Kreogist Cuties information."));
    connect(act[mnuHelpAbout],SIGNAL(triggered()),this,SLOT(aboutKCI()));

    //about_qt
    act[mnuHelpAboutQt]=new QAction(tr("about Qt"),this);
    actStatusTips[mnuHelpAboutQt]=QString(tr("Display the Qt information, version number and copyright."));
    connect(act[mnuHelpAboutQt],SIGNAL(triggered()),this,SLOT(aboutQt()));
}

void MainWindow::aboutKCI()
{
    QMessageBox::about(this,tr("about"),
                       tr("Kreogist Cute IDE is an light IDE which is designed for ACMer/OIer"));
}

void MainWindow::aboutQt()
{
    QMessageBox::aboutQt(this,tr("about Qt"));
}

void MainWindow::createTitlebar()
{
    titlebar=new kciTitleBar(this);
    setMenuWidget(titlebar);
    titlebar->setMainButtonIcon(":/img/image/MainMenuButton.png");
}

void MainWindow::createDocks()
{
    compileDock=new kcicompiledock(this);
    addDockWidget(Qt::BottomDockWidgetArea,compileDock);
}

void MainWindow::createMenu()
{
    int i;

    QMenu *_mainMenu=new QMenu;
    _mainMenu->setWindowOpacity(50);
    QIcon *MenuIconAddor=new QIcon;
    //file menu
    MenuIconAddor->addFile(QString(":/img/image/FileMenuIcon.png"));
    menu[file] = _mainMenu->addMenu(tr("file"));
    menu[file]->setIcon(*MenuIconAddor);
    //from new_file to quit add into file menu
    for(i=mnuFileNewFile;i<=mnuFileExit;i++)
    {
        act[i]->setIcon(*MenuIconAddor);
        act[i]->setStatusTip(actStatusTips[i]);
        menu[file]->addAction(act[i]);
    }

    //edit menu
    MenuIconAddor->addFile(QString(":/img/image/EditMenuIcon.png"));
    menu[edit] = _mainMenu->addMenu(tr("edit"));
    menu[edit]->setIcon(*MenuIconAddor);
    //from redo to paste add into edit menu
    for(i=mnuEditUndo;i<=mnuEditPreference;i++)
    {
        act[i]->setIcon(*MenuIconAddor);
        act[i]->setStatusTip(actStatusTips[i]);
        menu[edit]->addAction(act[i]);
    }

    //view menu
    MenuIconAddor->addFile(QString(":/img/image/ViewMenuIcon.png"));
    menu[view] = _mainMenu->addMenu(tr("view"));
    menu[view]->setIcon(*MenuIconAddor);

    //search menu
    MenuIconAddor->addFile(QString(":/img/image/SearchMenuIcon.png"));
    menu[search] = _mainMenu->addMenu(tr("search"));
    menu[search]->setIcon(*MenuIconAddor);
    for(i=mnuSearchFind;i<=mnuSearchGoto;i++)
    {
        menu[search]->addAction(act[i]);
        act[i]->setStatusTip(actStatusTips[i]);
        act[i]->setIcon(*MenuIconAddor);
    }

    //insert menu
    MenuIconAddor->addFile(QString(":/img/image/InsertMenuIcon.png"));
    menu[insert] = _mainMenu->addMenu(tr("insert"));
    menu[insert]->setIcon(*MenuIconAddor);

    //run menu
    MenuIconAddor->addFile(QString(":/img/image/RunMenuIcon.png"));
    menu[run] = _mainMenu->addMenu(tr("run"));
    menu[run]->setIcon(*MenuIconAddor);
    for(i=mnuRunCompileAndRun;i<=mnuRunSetInputRunShowOutput;i++)
    {
        act[i]->setIcon(*MenuIconAddor);
        act[i]->setStatusTip(actStatusTips[i]);
        menu[run]->addAction(act[i]);
    }

    //debug menu
    MenuIconAddor->addFile(QString(":/img/image/DebugMenuIcon.png"));
    menu[debug] = _mainMenu->addMenu(tr("debug"));
    menu[debug]->setIcon(*MenuIconAddor);
    for(i=mnuDebugStart;i<=mnuDebugRemoveWatch;i++)
    {
        act[i]->setIcon(*MenuIconAddor);
        act[i]->setStatusTip(actStatusTips[i]);
        menu[debug]->addAction(act[i]);
    }

    //plugins menu
    MenuIconAddor->addFile(QString(":/img/image/PluginMenuIcon.png"));
    menu[plugins] = _mainMenu->addMenu(tr("plugins"));
    menu[plugins]->setIcon(*MenuIconAddor);

    //help menu
    MenuIconAddor->addFile(QString(":/img/image/HelpMenuIcon.png"));
    menu[help] = _mainMenu->addMenu(tr("help"));
    menu[help]->setIcon(*MenuIconAddor);
    //from about to about_qt add into help menu
    for(i=mnuHelpAbout;i<=mnuHelpAboutQt;i++)
    {
        act[i]->setIcon(*MenuIconAddor);
        act[i]->setStatusTip(actStatusTips[i]);
        menu[help]->addAction(act[i]);
    }

    titlebar->setMenu(_mainMenu);
}

void MainWindow::createStatusbar()
{
    QStatusBar *statusbar=statusBar();

    QPalette pal=statusbar->palette();
    pal.setColor(QPalette::Window,QColor(0x89,0x89,0x89));
    pal.setColor(QPalette::Foreground,QColor(255,255,255));
    statusbar->setPalette(pal);
}

void MainWindow::restoreSettings()
{
    QSettings settings(kciGlobal::settingsFileName,QSettings::IniFormat);

    settings.beginGroup("MainWindow");

    int n_WindowState;
    float n_X, n_Y, n_width, n_height;

    n_X     = (settings.value("x").toFloat() / settings.value("screenwidth").toFloat())
                * QApplication::desktop()->width();
    n_Y     = (settings.value("y").toFloat() / settings.value("screenwidth").toFloat())
                * QApplication::desktop()->width();
    n_width = (settings.value("width").toFloat() / settings.value("screenwidth").toFloat())
                * QApplication::desktop()->width();
    n_height= (settings.value("height").toFloat()/ settings.value("screenheight").toFloat())
                * QApplication::desktop()->height();
    setGeometry(static_cast<int>(n_X),
                static_cast<int>(n_Y),
                static_cast<int>(n_width),
                static_cast<int>(n_height));
    n_WindowState=settings.value("state").toInt();
    switch(n_WindowState)
    {
    case 1:
        setWindowState(Qt::WindowMinimized);
    case 2:
        setWindowState(Qt::WindowMaximized);
    }

    settings.endGroup();

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    if(this->isMaximized())
    {
        savedGeometry.setSize(e->oldSize());
    }
    else
    {
        savedGeometry.setSize(e->size());
        savedGeometry.setX(x());
        savedGeometry.setY(y());
    }
    QMainWindow::resizeEvent(e);
}

void MainWindow::saveSettings()
{
    QSettings settings(kciGlobal::settingsFileName,QSettings::IniFormat);

    if(!this->isMaximized())
    {
        savedGeometry.setX(x());
        savedGeometry.setY(y());
    }

    int n_WindowState;

    //Save ALL settings.
    settings.beginGroup("MainWindow");
    settings.setValue("width",savedGeometry.width());
    settings.setValue("height",savedGeometry.height());
    settings.setValue("x",savedGeometry.x());
    settings.setValue("y",savedGeometry.y());
    settings.setValue("screenwidth",QApplication::desktop()->width());
    settings.setValue("screenheight",QApplication::desktop()->height());
    switch(windowState())
    {
    case Qt::WindowMinimized:n_WindowState=1;break;
    case Qt::WindowMaximized:n_WindowState=2;break;
    default:n_WindowState=0;break;
    }
    settings.setValue("state",n_WindowState);
    settings.endGroup();
}


void MainWindow::closeEvent(QCloseEvent *e)
{
    if(tabManager->close())
    {
        saveSettings();
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::compileCurrentFile()
{
    /*kciTextEditor *currentEditor=qobject_cast<kciTextEditor *>(tabManager->currentWidget());
    if(currentEditor!=NULL)
    {
        if(Q_UNLIKELY(!currentEditor->save()))
        {
            QErrorMessage error(this);
            error.showMessage(tr("Saving file failed!"));
            error.exec();
            return;
        }

        compilerBase *currentCompiler=
                compilerBaseFactor::createCompilerBase(currentEditor->getFilePath(), currentEditor);
        connect(currentCompiler,&compilerBase::output,compileDock,&kcicompiledock::parseMessage);
        currentCompiler->startCompile(currentEditor->getFilePath());
        currentCompiler->waitForFinished();
        qDebug()<<currentCompiler->error();
        qDebug()<<currentCompiler->errorString();
    }*/
}
