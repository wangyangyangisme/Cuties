/*
 *  Copyright 2013 Kreogist Dev Team
 *
 *  This file is part of Kreogist Cuties.
 *
 *    Kreogist Cuties is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *    Kreogist Cuties is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with Kreogist Cuties.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KCSIDEBAR_H
#define KCSIDEBAR_H

#include <QListView>
#include <QDockWidget>

#include "kchistoryconfigure.h"
#include "kcclipboard.h"

class QLabel;
class QTimeLine;
class QSignalMapper;
class QMouseEvent;
class QStackedWidget;
class QBoxLayout;
class QIcon;
class QPixmap;

class KCClipboardHistoryStack : public QListView
{
    Q_OBJECT
public:
    explicit KCClipboardHistoryStack(QWidget *parent = 0);

signals:
    void requiredInsertText(QString insertText);
    void requireSetCurrentEditorFocus();

private slots:
    void dblClickClipboardItems(QModelIndex ItemID);

private:

};

class KCHistoryStack : public QListView
{
    Q_OBJECT
public:
    explicit KCHistoryStack(QWidget *parent = 0);

signals:
    void requiredOpenFiles(QString filePath);

private slots:
    void dblClickHistoryItems(QModelIndex itemIndex);

private:

};

class KCSidebarButton : public QWidget
{
    Q_OBJECT
public:
    explicit KCSidebarButton(QWidget *parent = 0);
    void setIcon(QString IconPath);
    void setPressed(bool newValue, bool force);
    void setBackgroundColor();

signals:
    void mouseEnter();
    void mouseExit();
    void mousePressed();

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);
    void mousePressEvent(QMouseEvent *e);

private:
    QPalette pal;
    QColor buttonBackcolor;
    QBoxLayout *mainLayout;
    QLabel *iconDisplayer;

    bool buttonPressed;
    bool buttonLocked;
};

class KCSideBarContent : public QWidget
{
    Q_OBJECT
public:
    explicit KCSideBarContent(QWidget *parent = 0);
    ~KCSideBarContent();
    void setContentFixedWidth(int newContentWidth);
    void forceClearButtonState();
    void forceShowButtonState();

signals:
    void historyRequiredOpenFiles(QString filePath);
    void clipRequiredInsertText(QString insertText);
    void requireSetCurrentEditorFocus();
    void expandRequest();
    void foldRequest();

public slots:
    void retranslate();
    void retranslateAndSet();
    void showContent();
    void hideContent();
    void listButtonClicked(int Index);

private:
    QBoxLayout *mainLayout,
               *buttonGroupLayout;
    enum sidebarButtons
    {
        sideButtonHistory,
        sideButtonClipboard,
        sidebarButtonCount
    };

    QString historyButtonTips, clipboardButtonTips;

    KCSidebarButton *sidebarButton[sidebarButtonCount];
    QSignalMapper *sidebarButtonMapper;

    QStackedWidget *contents;
    KCHistoryStack *historyStack;
    KCClipboardHistoryStack *clipboardStack;

    int sidebarButtonIndex;
};

class KCSideBar : public QDockWidget
{
    Q_OBJECT
public:
    explicit KCSideBar(QWidget *parent = 0);
    void forceClearButtonState();
    void forceShowButtonState();

    bool getExpandState() const;
    void setExpandState(bool value);
    bool getUnlockState() const;
    void setUnlockState(bool value);

signals:
    void historyRequiredOpenFiles(QString filePath);
    void clipboardRequiredInsertText(QString insertText);
    void requireSetCurrentEditorFocus();

public slots:
    void retranslate();
    void retranslateAndSet();
    void expandAnime();
    void foldAnime();

protected:
    void enterEvent(QEvent *e);
    void leaveEvent(QEvent *e);

private slots:
    void resizeDock(int newWidth);
    void showDock();
    void hideDock();

private:
    bool expandState, unlockState;
    QTimeLine *expandAnimation, *foldAnimation;
    QTimeLine *showDockAnimation, *hideDockAnimation;
    KCSideBarContent *centralWidget;
    QMetaObject::Connection animationHide;
    QPalette pal;
    QString windowTitleString;
};

#endif // KCSIDEBAR_H
