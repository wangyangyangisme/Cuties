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

    //editor = new kciTextEditor(this);
    tabManager=new kciTabManager(this);
    setCentralWidget(tabManager);
    setContentsMargins(0,0,0,0);

    QPalette QPpal = palette();
    QPpal.setBrush(QPalette::Window, QBrush(QColor(83,83,83)));
    setPalette(QPpal);

    setWindowFlags(Qt::FramelessWindowHint);

    restoreSettings();

    createActions();
    createTitlebar();
    createMenu();
    createStatusbar();
}

void MainWindow::createActions()
{
    //File -> New
    act[mnuFileNewFile]=new QAction(tr("new file"),this);
    actStatusTips[mnuFileNewFile]=QString(tr("Create a new document."));
    connect(act[mnuFileNewFile],SIGNAL(triggered()),
            tabManager,SLOT(new_file()));

    //File -> Open
    act[mnuFileOpen]=new QAction(tr("open"),this);
    actStatusTips[mnuFileOpen]=QString(tr("Open an exsisting document."));
    connect(act[mnuFileOpen],SIGNAL(triggered()),tabManager,SLOT(open()));

    //File -> Save
    act[mnuFileSave]=new QAction(tr("save"),this);
    actStatusTips[mnuFileSave]=QString(tr("Save active document."));
    connect(act[mnuFileSave],SIGNAL(triggered()),tabManager,SLOT(save()));

    //File -> Save As
    act[mnuFileSaveAs]=new QAction(tr("save as"),this);
    actStatusTips[mnuFileSaveAs]=QString(tr("Save as different file name."));
    connect(act[mnuFileSaveAs],SIGNAL(triggered()),tabManager,SLOT(save_as()));

    //File -> Save All
    act[mnuFileSaveAll]=new QAction(tr("save all"),this);
    actStatusTips[mnuFileSaveAll]=QString(tr("Save all modified documents."));

    //File -> Close
    act[mnuFileClose]=new QAction(tr("close"),this);
    actStatusTips[mnuFileClose]=QString(tr("Close the active document."));

    //File -> Close All
    act[mnuFileCloseAll]=new QAction(tr("close all"),this);
    actStatusTips[mnuFileCloseAll]=QString(tr("Close all files."));

    //File -> Close All Except This
    act[mnuFileCloseAllExceptThis]=new QAction(tr("close all other file"),this);
    actStatusTips[mnuFileCloseAllExceptThis]=QString(tr("Close all files except active file."));

    //File -> Exit
    act[mnuFileExit]=new QAction(tr("quit"),this);
    actStatusTips[mnuFileExit]=QString(tr("Quit applications; prompts to save documents."));
    connect(act[mnuFileExit],SIGNAL(triggered()),this,SLOT(close()));

    //Edit -> Undo
    act[mnuEditUndo]=new QAction(tr("undo"),this);
    actStatusTips[mnuEditUndo]=QString(tr("Undo the last action."));

    //Edit -> Redo
    act[mnuEditRedo]=new QAction(tr("redo"),this);
    actStatusTips[mnuEditRedo]=QString(tr("Redo the previously undone action."));

    //Edit -> Cut
    act[mnuEditCut]=new QAction(tr("cut"),this);
    actStatusTips[mnuEditCut]=QString(tr("Cut the selection and put it on the clipboard."));

    //Edit -> Copy
    act[mnuEditCopy]=new QAction(tr("copy"),this);
    actStatusTips[mnuEditCopy]=QString(tr("Copy the selection and put it on the clipboard."));

    //Edit -> Paste
    act[mnuEditPaste]=new QAction(tr("paste"),this);
    actStatusTips[mnuEditPaste]=QString(tr("Paste clipboard contents."));

    //Edit -> Select All
    act[mnuEditSelectAll]=new QAction(tr("select all"),this);
    actStatusTips[mnuEditSelectAll]=QString(tr("Select the entire document."));

    //Edit -> Preference
    act[mnuEditPreference]=new QAction(tr("preference"),this);
    actStatusTips[mnuEditPreference]=QString(tr("Customize your IDE."));

    //search in file
    act[searchinfile]=new QAction(tr("searchinfile"),this);

    //search all file
    act[searchallfile]=new QAction(tr("searchallfile"),this);

    //replace in file
    act[replaceinfile]=new QAction(tr("replaceinfile"),this);

    //replace all file
    act[replaceallfile]=new QAction(tr("replaceallfile"),this);

    //search online
    act[searchonline]=new QAction(tr("searchonline"),this);

    //go to line
    act[gotoline]=new QAction(tr("gotoline"),this);

    //compile and run
    act[compileandrun]=new QAction(tr("Compile & Run"),this);

    //compile
    act[compile]=new QAction(tr("compile"),this);

    //compile all file
    act[compileallfile]=new QAction(tr("Complie all file"),this);

    //run
    act[runexe]=new QAction(tr("Runexe"),this);

    //Parameters
    act[parameters]=new QAction(tr("Parameters"),this);

    //Set Input file
    act[setinputfile]=new QAction(tr("Set Input File"),this);

    //Show Output file
    act[showoutputfile]=new QAction(tr("Show Output file"),this);

    //Run And show
    act[runshowoutputfile]=new QAction(tr("Run And Show Output File"),this);

    //Set Input, Run and Show Output
    act[setinputrunshowoutput]=new QAction(
                tr("Set Input, Run and show Output"), this);

    //debug start
    act[debugstart]=new QAction(tr("Start Debug"),this);

    //debug pause
    act[debugcontinue]=new QAction(tr("Pause Debug"),this);

    //Stop Execute
    act[stopexecute]=new QAction(tr("Stop execute"),this);

    //set_break_point
    act[set_break_point]=new QAction(tr("set break point"),this);
    actStatusTips[set_break_point]=QString(tr("Set a break point at the current line."));

    //next line
    act[nextline]=new QAction(tr("Next Line"),this);

    //Go Into line
    act[intoline]=new QAction(tr("Into Line"),this);

    //Go Next Instruction
    act[nextinstruction]=new QAction(tr("Next Instruction"),this);

    //Go Into Instruction
    act[mnuDebugIntoInstruction]=new QAction(tr("Into Instruction"),this);

    //Skip Function
    act[skipfunction]=new QAction(tr("skip function"),this);

    //Add Watch
    act[addwatch]=new QAction(tr("Add Watch"),this);

    //Modify Watch
    act[modifywatch]=new QAction(tr("Modify Watch"),this);

    //Remove Watch
    act[removewatch]=new QAction(tr("Remove Watch"),this);

    //about
    act[about]=new QAction(tr("about"),this);
    connect(act[about],SIGNAL(triggered()),this,SLOT(aboutKCI()));

    //about_qt
    act[about_qt]=new QAction(tr("about Qt"),this);
    connect(act[about_qt],SIGNAL(triggered()),this,SLOT(aboutQt()));
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
    for(i=searchinfile;i<=gotoline;i++)
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
    for(i=compileandrun;i<=setinputrunshowoutput;i++)
    {
        act[i]->setIcon(*MenuIconAddor);
        act[i]->setStatusTip(actStatusTips[i]);
        menu[run]->addAction(act[i]);
    }

    //debug menu
    MenuIconAddor->addFile(QString(":/img/image/DebugMenuIcon.png"));
    menu[debug] = _mainMenu->addMenu(tr("debug"));
    menu[debug]->setIcon(*MenuIconAddor);
    for(i=debugstart;i<=removewatch;i++)
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
    for(i=about;i<=about_qt;i++)
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
    //statusbar->
}

void MainWindow::restoreSettings()
{
    QSettings settings(kciGlobal::settingsFileName,QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    restoreGeometry(settings.value("geometry").toByteArray());
    restoreState(settings.value("state").toByteArray());
    settings.endGroup();

}

void MainWindow::saveSettings()
{
    QSettings settings(kciGlobal::settingsFileName,QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    settings.setValue("geometry",saveGeometry());
    settings.setValue("state",saveState());
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
