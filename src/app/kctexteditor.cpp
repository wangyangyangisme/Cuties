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

#include <QScrollBar>
#include <QPainter>
#include <QPalette>
#include <QFont>
#include <QFontMetrics>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QMenu>
#include <QStyleFactory>
#include <QApplication>
#include <QDebug>
#include <QShowEvent>
#include <QResizeEvent>
#include <QTextBlock>
#include <QTextCursor>

#include "kctextblockdata.h"
#include "kcclipboard.h"
#include "kcfloattoolbar.h"
#include "kcfontconfigure.h"
#include "kccolorconfigure.h"
#include "kctexteditor.h"
#include "kctextpanelmanager.h"
#include "kclinenumberpanel.h"
#include "kcunibodypanel.h"
#include "kcdebugmarkpanel.h"

/*!
 * \brief Constructs a KCTextEditor with the given parent and the specified
 * widget flags.
 * \param parent Specifies the parent QWidget.
 */
KCTextEditor::KCTextEditor(QWidget *parent) :
    QPlainTextEdit(parent)
{
    //Initialize properties.
    setObjectName("KCTextEditor");
    setAcceptDrops(false);

    //Initialize visual properties.
    setContentsMargins(0,0,0,0);
    setFrameShape(QFrame::NoFrame);
    QFont editorFont=KCFontConfigure::getInstance()->getFont("code");
    setFont(editorFont);
    QPalette pal=palette();
    KCColorConfigure::getInstance()->getPalette(pal,objectName());
    setPalette(pal);

    //Initialize variables.
    /***Search code***/
    searchCode=0;
    /***Clipboard Menu***/
    /*clipboard=KCClipboard::getInstance();
    clipboardHistoryMenuSignalMapper=new QSignalMapper(this);
    connect(clipboardHistoryMenuSignalMapper,SIGNAL(mapped(QString)),
            this,SLOT(insertPlainText(QString)));*/

    //Initial child widgets
    /***Float toolbar***/
    //textFloatToolBar=new KCFloatToolBar(this);
    /***Panel Manager & Panels***/
    panelManager=new KCTextPanelManager(this);
    debugMarkPanel=new KCDebugMarkPanel(this);
    panelManager->addPanel(debugMarkPanel);
    lineNumberPanel=new KCLineNumberPanel(this);
    panelManager->addPanel(lineNumberPanel);
    unibodyPanel=new KCUnibodyPanel(this);
    panelManager->addPanel(unibodyPanel);

    //Connect signals and signals/slots.
    /****Highlight****/
    connect(this, &KCTextEditor::cursorPositionChanged,
            this, &KCTextEditor::updateHighlights);
    /*    connect(this, &KCTextEditor::textChanged,
                this, &KCTextEditor::updateHighlights);*/

    /****Search & Replace****/
    connect(verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(updateSearchResults()));
    /*connect(verticalScrollBar(),SIGNAL(valueChanged(int)),
            this,SLOT(updateHighlights()));*/

    /****Panels****/
    connect(panelManager, &KCTextPanelManager::requireUpdateAllPanel,
            this, &KCTextEditor::updateAllPanels);
    connect(panelManager, &KCTextPanelManager::requirePaintPanel,
            this, &KCTextEditor::panelPaintEvent);
    connect(unibodyPanel, &KCUnibodyPanel::requireFoldStartAt,
            this, &KCTextEditor::foldCode);
    connect(unibodyPanel, &KCUnibodyPanel::requireUnfoldStartAt,
            this, &KCTextEditor::unfoldCode);
    connect(lineNumberPanel, &KCLineNumberPanel::requireSelectBlock,
            this, &KCTextEditor::selectBlock);

    //Update the high lights
    updateHighlights();
}

/***********Properies settings************/
/*!
 * \brief Set the vertical scroll bar value.
 * \param value The scroll bar value.
 */
void KCTextEditor::setVerticalScrollValue(int value)
{
    verticalScrollBar()->setValue(value);
}

/*!
 * \brief Set the horizontal scroll bar value.
 * \param value The scroll bar value.
 */
void KCTextEditor::setHorizontalScrollValue(int value)
{
    horizontalScrollBar()->setValue(value);
}

/*!
 * \brief Get the vertical scroll bar value.
 */
int KCTextEditor::verticalScrollValue()
{
    return verticalScrollBar()->value();
}

void KCTextEditor::setFilePath(const QString &value)
{
    filePath=value;
}

/*!
 * \brief Get the horizontal scroll bar value.
 */
int KCTextEditor::horizontalScrollValue()
{
    return horizontalScrollBar()->value();
}

/*!
 * \brief Set the tab width of the current text editor.
 * \param width The number of the space width of the tab width.
 */
void KCTextEditor::setTabWidth(int width)
{
    tabSpace=width;
    setTabStopWidth(fontMetrics().width(' ')*tabSpace);
}

void KCTextEditor::setMatchedParenthesesColor(const QColor &value)
{
    matchedParenthesesColor = value;
}

bool KCTextEditor::readFile(const QString &filePath,
                            QString codecName,
                            bool cacheOpenMode)
{
    QFile _file(filePath);

    if(_file.open(QIODevice::ReadOnly |QIODevice::Text))
    {
        QTextStream _textIn(&_file);
        _textIn.setCodec(QTextCodec::codecForName(codecName.toUtf8()));
        QString titleText=documentTitle();

        clear();
        setPlainText(QString(_textIn.readAll()));

        if(cacheOpenMode)
        {
            this->setDocumentTitle(titleText);
            document()->setModified(true);
        }
        else
        {
            fileInfoChanged(QFileInfo(_file));
            emit requiredAddRecentRecord(getFilePath());
        }
        _file.close();
        return true;
    }
    setFileError(_file.error());
    return false;
}

bool KCTextEditor::writeToFile(const QString &filePath,
                               QString codecName,
                               bool cacheOpenMode)
{
    QFile _file(filePath);

    if(_file.open(QIODevice::WriteOnly |QIODevice::Text))
    {
        QTextStream _textOut(&_file);
        _textOut.setCodec(QTextCodec::codecForName(codecName.toUtf8()));
        _textOut<<toPlainText()<<flush;
        if(!cacheOpenMode)
        {
            fileInfoChanged(QFileInfo(_file));
        }
        _file.close();
        return true;
    }
    setFileError(_file.error());
    return false;
}

void KCTextEditor::setDocumentTitle(const QString &title)
{
    QPlainTextEdit::setDocumentTitle(title);
    emit fileNameChanged(title);
}

void KCTextEditor::setNoMatchedParenthesesColor(const QColor &value)
{
    noMatchedParenthesesColor = value;
}

void KCTextEditor::setSearchResultColor(const QColor &value)
{
    searchResultColor = value;
}

void KCTextEditor::setLineColor(const QColor &value)
{
    lineColor = value;
}

void KCTextEditor::setSpacePerTab(int value)
{
    spacePerTab = value;
}

void KCTextEditor::setUsingBlankInsteadTab(bool value)
{
    usingBlankInsteadTab = value;
}

/***********Search & Replace************/
void KCTextEditor::checkWhetherBlockSearchedAndDealWith(const QTextBlock &block,
        KCTextBlockData *data)
{
    //check whether the block has been searched
    data->beginUsingSearchDatas();
    if(!data->isSearched(searchCode))
    {
        data->endUsingSearchDatas();
        generalSearch(block,
                      height()/fontMetrics().lineSpacing()+2,
                      true);    //search a screen lines.
        data->beginUsingSearchDatas();
    }
    data->endUsingSearchDatas();
}

bool KCTextEditor::showPreviousSearchResult(const QTextCursor &cursor)
{
    //Backup current cursor
    QTextCursor searchCursor=cursor;
    //Simbol to match a string
    bool hasMatch=false, isCursorLine;
    //Check search position
    int currentCursorPostion=searchCursor.selectionStart(),
        matchedCount;
    searchCursor.setPosition(currentCursorPostion);
    KCTextBlockData::matchedInfo currentMatch;
    KCTextBlockData *blockData;
    //Search from current place to next place
    for(QTextBlock i=searchCursor.block();   //From current block
        i.isValid() && !hasMatch;           //to the destination block
        i=i.previous())  //If search forward, to previous
    {
        //otherwise to the next.
        blockData=getBlockData(i);
        checkWhetherBlockSearchedAndDealWith(i, blockData);
        blockData->beginUsingSearchDatas();
        if(blockData->hasMatched())         //If current have search result
        {
            isCursorLine=i.blockNumber() == searchCursor.blockNumber();
            //If position is forward
            matchedCount=blockData->matchedCount();
            for(int j=matchedCount-1; j>=0; j--)
            {
                currentMatch=blockData->getMatchedInfo(j);
                if((isCursorLine && currentMatch.pos<searchCursor.positionInBlock())
                   || !isCursorLine)
                {
                    hasMatch=true;
                    searchCursor.setPosition(i.position()+currentMatch.pos);
                    searchCursor.movePosition(QTextCursor::NextCharacter,
                                              QTextCursor::KeepAnchor,
                                              currentMatch.matchedLength);
                    break;
                }
            }
        }
        blockData->endUsingSearchDatas();
    }
    if(hasMatch)
    {
        setTextCursor(searchCursor);
        return true;
    }
    return false;
}

bool KCTextEditor::showNextSearchResult(const QTextCursor &cursor)
{
    QTextCursor searchCursor=cursor;
    //Simbol to match a string
    bool hasMatch=false, isCursorLine;
    //Backup current cursor
    //Check search position
    int currentCursorPostion=searchCursor.position(),
        matchedCount, comparePosition;
    searchCursor.setPosition(currentCursorPostion);
    KCTextBlockData *blockData;
    KCTextBlockData::matchedInfo currentMatch;
    //Search from current place to next place
    for(QTextBlock i=searchCursor.block();
        i.isValid() && !hasMatch;
        i=i.next())
    {
        blockData=getBlockData(i);
        checkWhetherBlockSearchedAndDealWith(i, blockData);
        blockData->beginUsingSearchDatas();
        if(blockData->hasMatched())         //If current have search result
        {
            isCursorLine=i.blockNumber() == searchCursor.blockNumber();
            matchedCount=blockData->matchedCount();
            comparePosition=isCursorLine?searchCursor.position()-i.position():0;
            for(int j=0; j<matchedCount; j++)
            {
                currentMatch=blockData->getMatchedInfo(j);
                if(currentMatch.pos>=comparePosition)
                {
                    hasMatch=true;
                    searchCursor.setPosition(i.position()+currentMatch.pos);
                    searchCursor.movePosition(QTextCursor::NextCharacter,
                                              QTextCursor::KeepAnchor,
                                              currentMatch.matchedLength);
                    break;
                }
            }
        }
        blockData->endUsingSearchDatas();
    }
    if(hasMatch)
    {
        setTextCursor(searchCursor);
        return true;
    }
    return false;
}

bool KCTextEditor::findNextSearchResult()
{
    if(!showNextSearchResult(textCursor()))
    {
        return findFirstSeachResult();
    }
    return true;
}

bool KCTextEditor::findPreviousSearchResult()
{
    if(!showPreviousSearchResult(textCursor()))
    {
        return findLastSearchResult();
    }
    return true;
}

bool KCTextEditor::findFirstSeachResult()
{
    QTextCursor searchCursor=textCursor();
    searchCursor.movePosition(QTextCursor::Start);
    return showNextSearchResult(searchCursor);
}

bool KCTextEditor::findLastSearchResult()
{
    QTextCursor searchCursor=textCursor();
    searchCursor.movePosition(QTextCursor::End);
    return showPreviousSearchResult(searchCursor);
}

void KCTextEditor::searchString(QString searchTextSets,
                                bool regularExpressionSets,
                                bool caseSensitivelySets,
                                bool wholeWordSets)
{
    searchText=searchTextSets;
    searchRegularExpression=regularExpressionSets;
    searchCaseSensitively=caseSensitivelySets;
    searchWholeWord=wholeWordSets;
    searchCode++;

    if(searchText.isEmpty())
    {
        updateSearchResults();
        searchBackupCursor.clearSelection();
        setTextCursor(searchBackupCursor);
    }
    else
    {
        setTextCursor(searchBackupCursor);
        if(wholeWordSets)
        {
            if(!searchRegularExpression)
            {
                searchText=QRegularExpression::escape(searchTextSets);
            }
            searchText.prepend(QString("\\b("));
            searchText.append(QString(")\\b"));
        }
        updateSearchResults();
        searchOnOtherThread(searcherForNext,threadNext,firstVisibleBlock(),true);
        searchOnOtherThread(searcherForPrev,threadPrev,firstVisibleBlock(),false);
        findNextSearchResult();
    }
}

void KCTextEditor::updateSearchResults()
{
    generalSearch(firstVisibleBlock(),
                  height()/fontMetrics().lineSpacing()+2,
                  true);
    updateHighlights();
}

void KCTextEditor::generalSearch(const QTextBlock &block,
                                 const int &lines,
                                 const bool forward)
{
    QScopedPointer<KCTextSearcher> searcher;
    initTextSearcher(searcher);
    searcher->search(block,
                     lines,
                     searchCode,
                     forward);
}

void KCTextEditor::searchOnOtherThread(QScopedPointer<KCTextSearcher> &searcher,
                                       QFuture<void> &thread,
                                       const QTextBlock &block,
                                       const bool forward)
{
    if(!searcher.isNull())
    {
        searcher->requireStop();
        thread.waitForFinished();
    }

    initTextSearcher(searcher);

    thread=QtConcurrent::run(searcher.data(),
                             &KCTextSearcher::search,
                             block,
                             SEARCH_UNTIL_END_MARK,
                             searchCode,
                             forward);
}

void KCTextEditor::initTextSearcher(QScopedPointer<KCTextSearcher> &searcher)
{
    if(searchRegularExpression || searchWholeWord)
    {
        searcher.reset(new KCTextSearcherRegexp);
    }
    else
    {
        searcher.reset(new KCTextSearcherStringMatcher);
    }

    searcher->setPatternString(searchText);
    searcher->setIsCaseSensitive(searchCaseSensitively);
}

bool KCTextEditor::replace(const QString &oldText, const QString &newText)
{
    QTextCursor _cursor=textCursor();

    if(_cursor.hasSelection() && _cursor.selectedText() == oldText)
    {
        _cursor.beginEditBlock();
        _cursor.removeSelectedText();
        _cursor.insertText(newText);
        _cursor.endEditBlock();
        setTextCursor(_cursor);
        return true;
    }
    return false;
}

bool KCTextEditor::replaceAndFind(const QString &oldText,
                                  const QString &newText)
{
    return replace(oldText,newText)|findNextSearchResult();
}

bool KCTextEditor::replaceAll(const QString &oldText, const QString &newText)
{
    int matchedCount=0;
    //Search from the very beginning to the last
    for(QTextBlock i=document()->begin();
        i.isValid();
        i=i.next())
    {
        KCTextBlockData *blockData=getBlockData(i);
        checkWhetherBlockSearchedAndDealWith(i, blockData);
        blockData->beginUsingSearchDatas();
        matchedCount+=blockData->matchedCount();
        blockData->endUsingSearchDatas();
    }
    while(replaceAndFind(oldText,newText) && --matchedCount)
    {
        ;
    }
    return true;
}

/***********Text process************/
//Tab process
void KCTextEditor::autoIndent(const QTextBlock &b)
{
    if(!b.previous().isValid())
    {
        //First block, no need to judge.
        return;
    }

    QTextCursor indentCursor(b);
    int pos=findFirstCharacter(b);
    indentCursor.setPosition(b.position()+pos);
    indentCursor.movePosition(QTextCursor::StartOfBlock,
                              QTextCursor::KeepAnchor);
    indentCursor.removeSelectedText();
    KCTextBlockData *currData=getBlockData(b);
    int currLevel=currData!=NULL?currData->getCodeLevel():0;
    if(b.next().isValid())
    {
        KCTextBlockData *nextData=getBlockData(b.next());
        int nextLevel=nextData!=NULL?nextData->getCodeLevel():0;
        if(nextLevel<currLevel)
        {
            insertTab(indentCursor,nextLevel);
            return;
        }
    }
    else
    {
        //Last block, no need to judge.
        return;
    }

    QTextBlock currBlock=b;
    QTextBlock prevBlock=currBlock.previous();
    indentCursor.setPosition(currBlock.position());
    KCTextBlockData *prevData=getBlockData(prevBlock);
    int basePos=findFirstCharacter(prevBlock),
        baseLevel=prevData!=NULL?prevData->getCodeLevel():0;

    QTextCursor prevCursor(prevBlock);
    prevCursor.movePosition(QTextCursor::Right,
                            QTextCursor::KeepAnchor,
                            basePos);
    QString tabs=prevCursor.selectedText();
    indentCursor.setPosition(currBlock.position()+pos);
    indentCursor.movePosition(QTextCursor::StartOfBlock,
                              QTextCursor::KeepAnchor);
    indentCursor.removeSelectedText();
    indentCursor.insertText(tabs);

    insertTab(indentCursor,currLevel-baseLevel);
}

void KCTextEditor::insertTab(QTextCursor insertTabCursor, int tabCount)
{
    if(tabCount>0)
    {
        insertTabCursor.clearSelection();
        QString spaceChar=usingBlankInsteadTab?
                          QString(" ").repeated(spacePerTab):
                          "\t";
        insertTabCursor.insertText(spaceChar.repeated(tabCount));
    }
}

void KCTextEditor::removeTab(QTextCursor removeTabCursor, int tabCount)
{
    removeTabCursor.clearSelection();

    //We have to judge whether we are using tab or space.
    int expectLength=usingBlankInsteadTab?tabCount*spacePerTab:tabCount;
    if(expectLength>removeTabCursor.positionInBlock())
    {
        /*
         * Here means: we don't have so much space to remove,
         * just remove all of them
         */
        removeTabCursor.movePosition(QTextCursor::Left,
                                     QTextCursor::KeepAnchor,
                                     removeTabCursor.positionInBlock());
    }
    else
    {
        //Remove the expect length
        removeTabCursor.movePosition(QTextCursor::Left,
                                     QTextCursor::KeepAnchor,
                                     expectLength);
    }
    removeTabCursor.removeSelectedText();
}

void KCTextEditor::tabPressEvent(QTextCursor tabPressCursor)
{
    tabPressCursor.beginEditBlock();

    if(tabPressCursor.hasSelection())
    {
        // Indent or unindent the selected lines
        int pos = tabPressCursor.position();
        int anchor = tabPressCursor.anchor();
        int start = qMin(anchor, pos);
        int end = qMax(anchor, pos);

        QTextDocument *doc = document();
        QTextBlock startBlock = doc->findBlock(start);
        QTextBlock endBlock = doc->findBlock(end-1).next();

        if(startBlock.next() == endBlock
           && (start > startBlock.position() || end < endBlock.position() - 1))
        {
            // Only one line partially selected.
            tabPressCursor.removeSelectedText();
        }
        else
        {
            QString spaceChar=usingBlankInsteadTab?
                              QString(" ").repeated(spacePerTab):
                              "\t";
            for(QTextBlock block = startBlock; block != endBlock; block = block.next())
            {
                int indentPosition = findFirstCharacter(block);
                tabPressCursor.setPosition(block.position() + indentPosition);
                tabPressCursor.insertText(spaceChar);
            }
            tabPressCursor.endEditBlock();
            return;
        }
    }
    insertTab(tabPressCursor, 1);
    tabPressCursor.endEditBlock();
}

QString KCTextEditor::getFilePath() const
{
    return filePath;
}

void KCTextEditor::setFileError(const QFileDevice::FileError &error)
{
    fileError=error;
}

QFileDevice::FileError KCTextEditor::getFileError() const
{
    return fileError;
}

int KCTextEditor::findFirstCharacter(const QTextBlock &block)
{
    int firstCharIndex=block.text().indexOf(QRegularExpression("\\S"));
    return firstCharIndex==-1?block.text().length():firstCharIndex;
}

/*!
 * \brief This function will set the text cursor to the position located by line
 *        number and column number.
 * \param lineNumber Line number of the destionation position.
 * \param columnNumber Column number of the destionation position.
 */
void KCTextEditor::setCursorPosition(int lineNumber,
                                     int columnNumber)
{
    QTextCursor cursor = textCursor();
    cursor.setPosition(document()->findBlockByNumber(lineNumber).position());
    cursor.movePosition(QTextCursor::NextCharacter,
                        QTextCursor::MoveAnchor,
                        columnNumber);
    setTextCursor(cursor);
}

/*!
 * \brief This function will backup the current cursor. The backuped cursor will
 *        be set at the time the search content changed. This function is used
 *        to let the search function begin to search at the same place.
 */
void KCTextEditor::backupSearchTextCursor()
{
    searchBackupCursor=textCursor();
}

int KCTextEditor::highlightParentheses(QList<QTextEdit::ExtraSelection> &selections)
{
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    int pos=cursor.position()-cursor.block().position();

    char all[]="{[()]}";
    int len=strlen(all);

    KCTextBlockData *blockData=getBlockData(cursor.block());
    if(blockData!=NULL)
    {
        int matchedParentheses=-2;

        for(auto i=blockData->getFirstParenthesesInfo(),
            l=blockData->getEndParenthesesInfo();
            i<l;
            i++)
        {
            if(i->pos == pos-1)
            {
                for(int j=0; j<(len>>1); j++)
                {
                    if(i->character == all[j])
                    {
                        cursor.movePosition(QTextCursor::Left,
                                            QTextCursor::KeepAnchor);
                        matchedParentheses=matchParentheses(
                                               all[j],
                                               all[len-j-1],
                                               i,
                                               cursor.block(),
                                               true);
                        break;
                    }
                }
            }
            else if(i->pos == pos)
            {
                for(int j=(len>>1); j<len; j++)
                {
                    if(i->character == all[j])
                    {
                        cursor.movePosition(QTextCursor::Right,
                                            QTextCursor::KeepAnchor);
                        matchedParentheses=matchParentheses(
                                               all[j],
                                               all[len-j-1],
                                               i,
                                               cursor.block(),
                                               false);
                        break;
                    }
                }
            }
            else
            {
                continue;
            }

            highlightParenthesisPairs(selections,
                                      matchedParentheses,
                                      cursor);

            cursor=textCursor();
            cursor.clearSelection();
        }
        return matchedParentheses;
    }
    return -2;
}

void KCTextEditor::highlightParenthesisPairs(QList<QTextEdit::ExtraSelection> &selections,
        int matchedParentheses,
        QTextCursor cursor)
{
    switch(matchedParentheses)
    {
    case -2:
        //no parenthesis in the block
        break;
    case -1:
    {
        QTextEdit::ExtraSelection selection;

        selection.cursor = cursor;
        selection.format.setBackground(noMatchedParenthesesColor);
        selections.append(selection);

        break;
    }
    default:
    {
        QTextEdit::ExtraSelection selection;

        selection.cursor = cursor;
        selection.format.setBackground(matchedParenthesesColor);
        selections.append(selection);

        cursor.setPosition(matchedParentheses);
        cursor.movePosition(QTextCursor::Right,
                            QTextCursor::KeepAnchor);
        selection.cursor = cursor;
        selection.format.setBackground(matchedParenthesesColor);
        selections.append(selection);

        break;
    }
    }
}


int KCTextEditor::matchParentheses(const char &parenthesesA,
                                   const char &parenthesesB,
                                   QList<parenthesesInfo>::iterator startPos,
                                   QTextBlock block,
                                   bool forward)
{
    int count=0;
    KCTextBlockData *blockData=getBlockData(block);
    if(blockData!=NULL)
    {
        auto i=startPos,
             l= forward?
                blockData->getEndParenthesesInfo() :
                blockData->getFirstParenthesesInfo() - 1;
        while(block.isValid())
        {
            for(;
                i!=l;
                i+= forward?1:-1)
            {
                if(i->character == parenthesesA)
                {
                    count++;
                }
                else if(i->character == parenthesesB)
                {
                    count--;
                }
                if(count == 0)
                {
                    return block.position() + i->pos;
                }
            }
            block= forward? block.next() : block.previous();
            blockData=getBlockData(block);
            if(blockData == NULL)
            {
                break;
            }
            if(forward)
            {
                i= blockData->getFirstParenthesesInfo();
                l= blockData->getEndParenthesesInfo();
            }
            else
            {
                i= blockData->getEndParenthesesInfo()-1;
                l= blockData->getFirstParenthesesInfo()-1;
            }
        }
    }
    return -1;
}

void KCTextEditor::updateHighlights()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    highlightCurrentLine(extraSelections);
    highlightSearchResult(extraSelections);
    highlightParentheses(extraSelections);

    setExtraSelections(extraSelections);
}

void KCTextEditor::updateAllPanels()
{
    panelManager->updateAllPanels();
    updatePanelAreaWidth();
}

void KCTextEditor::highlightCurrentLine(QList<QTextEdit::ExtraSelection> &selections)
{
    if(!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;
        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor=textCursor();
        selection.cursor.clearSelection();
        selections.append(selection);
    }
}

void KCTextEditor::highlightSearchResult(QList<QTextEdit::ExtraSelection> &selections)
{
    QTextCursor _cursor(document());

    QTextBlock block=firstVisibleBlock();
    int bottom=height()/fontMetrics().lineSpacing()+block.lineCount();

    for(; block.isValid() && bottom>0; block=block.next())
    {
        bottom-=block.lineCount();
        KCTextBlockData *currBlockData=getBlockData(block);
        if(currBlockData==NULL)
        {
            continue;
        }
        checkWhetherBlockSearchedAndDealWith(block, currBlockData);
        currBlockData->beginUsingSearchDatas();
        if(currBlockData->hasMatched())
        {
            int i;
            KCTextBlockData::matchedInfo currentMatched;
            for(i=0; i<currBlockData->matchedCount(); i++)
            {
                currentMatched=currBlockData->getMatchedInfo(i);
                QTextEdit::ExtraSelection selection;

                _cursor.clearSelection();
                _cursor.setPosition(block.position()+currentMatched.pos);
                _cursor.movePosition(QTextCursor::NextCharacter,
                                     QTextCursor::KeepAnchor,
                                     currentMatched.matchedLength);
                selection.cursor=_cursor;

                selection.format.setBackground(searchResultColor);
                selections.append(selection);
            }
        }
        currBlockData->endUsingSearchDatas();
    }
}

/*void KCTextEditor::pasteFromeHistory()
{
    QMenu* menu=new QMenu(this);

    QStringList texts=clipboard->getClipboardTexts();
    for(int i=0,l=texts.size();
        i<l;
        i++)
    {
        QString elidedText=fontMetrics().elidedText(texts.at(i),
                                                    Qt::ElideRight,
                                                    elideWidth);
        QAction *action=menu->addAction(elidedText,
                                        clipboardHistoryMenuSignalMapper,
                                        SLOT(map()));

        clipboardHistoryMenuSignalMapper->setMapping(action,texts.at(i));
    }

    menu->exec(contextMenuPos);

    delete menu;
}*/

void KCTextEditor::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *menu=createStandardContextMenu();
    menu->setStyle(QStyleFactory::create("fusion"));
    menu->exec(event->globalPos());
    delete menu;
}

QString KCTextEditor::parenthesesPair(const QString &parenthesesChar)
{
    if(!parenthesesChar.isEmpty())
    {
        int parenthesesIndex=leftParenthesesLists.indexOf(parenthesesChar);
        if(parenthesesIndex>-1)
        {
            return QString(rightParenthesesLists.at(parenthesesIndex));
        }
    }
    return QString("");
}

void KCTextEditor::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Escape:
        QPlainTextEdit::keyPressEvent(event);
        emit requireHideOthers();
        return;
    }
    QTextCursor _textCursor=textCursor();
    switch(event->key())
    {
    case Qt::Key_Tab:
    {
        tabPressEvent(_textCursor);
        break;
    }
    case Qt::Key_Return:
    case Qt::Key_Enter:
    {
        KCTextBlockData *currData=getBlockData(_textCursor.block());
        if(currData->getHasFolded())
        {
            unfoldCode(_textCursor.block().blockNumber());
            updateAllPanels();
        }
        if(_textCursor.position()>0)
        {
            QChar previousChar=_textCursor.document()->characterAt(_textCursor.position()-1),
                  currentChar=_textCursor.document()->characterAt(_textCursor.position());
            if(previousChar == '{')
            {
                if(currentChar == '}')
                {
                    //It's a pair of char '{}'
                    // Indent it!
                    QPlainTextEdit::keyPressEvent(event);
                    _textCursor.insertBlock();
                    QTextBlock currentBlock=_textCursor.block().previous();
                    autoIndent(currentBlock);
                    autoIndent(currentBlock.next());
                    _textCursor.movePosition(QTextCursor::PreviousBlock);
                    _textCursor.movePosition(QTextCursor::EndOfBlock);
                    setTextCursor(_textCursor);
                    break;
                }
            }
            if(currentChar == ')' || currentChar == ']')
            {
                QPlainTextEdit::keyPressEvent(event);
                QTextBlock currBlock=_textCursor.block();
                QTextBlock prevBlock=currBlock.previous();
                KCTextBlockData *prevData=getBlockData(prevBlock);
                KCTextBlockData *currData=getBlockData(currBlock);
                currData->setCodeLevel(prevData->getCodeLevel());
                int matchedStatus;
                for(auto i=currData->getFirstParenthesesInfo(),
                    l=currData->getEndParenthesesInfo();
                    i<l;
                    i++)
                {
                    if(i->pos == _textCursor.positionInBlock())
                    {
                        matchedStatus=matchParentheses(currentChar.toLatin1(),
                                                       currentChar==')'?'(':'[',
                                                       i,
                                                       currBlock,
                                                       false);
                        if(matchedStatus > -1)
                        {
                            QTextCursor meterCursor(document());
                            meterCursor.setPosition(matchedStatus);
                            QString blankFill=QString(" ").repeated(meterCursor.positionInBlock()+1);
                            _textCursor.insertText(blankFill);
                            return;
                        }
                    }
                }
                break;
            }
        }
        QPlainTextEdit::keyPressEvent(event);
        autoIndent(_textCursor.block());
        break;
    }
    case Qt::Key_Insert:
    {
        setOverwriteMode(!overwriteMode());
        emit overwriteModeChanged(overwriteMode());
        break;
    }
    default:
    {
        QPlainTextEdit::keyPressEvent(event);
        break;
    }
    }
}

void KCTextEditor::mouseReleaseEvent(QMouseEvent *e)
{
    QPlainTextEdit::mouseReleaseEvent(e);
    /*if(!textCursor().selectedText().isEmpty())
    {
        int newXPosition, newYPosition;
        newXPosition=e->globalX()+textFloatToolBar->width() > QApplication::desktop()->width()?
             e->globalX()-textFloatToolBar->width():
             e->globalX();
        newYPosition=e->globalY()+textFloatToolBar->width() > QApplication::desktop()->height()?
             e->globalY()-textFloatToolBar->height():
             e->globalY();
        textFloatToolBar->setGeometry(newXPosition,
                                      newYPosition,
                                      textFloatToolBar->width(),
                                      textFloatToolBar->height());
        textFloatToolBar->show();
    }
    else
    {
        textFloatToolBar->hide();
    }*/
}

void KCTextEditor::wheelEvent(QWheelEvent *event)
{
    if(event->modifiers() & Qt::ControlModifier)
    {
        zoomIn(event->angleDelta().y()/56);
        event->ignore();
        return;
    }
    QPlainTextEdit::wheelEvent(event);
}

void KCTextEditor::setLineErrorState(QList<int> errorList)
{
    for(QTextBlock i=document()->begin(); i.isValid(); i=i.next())
    {
        KCTextBlockData *blockData=getBlockData(i);
        blockData->setHasError((errorList.indexOf(i.blockNumber())!=-1));
    }
}

void KCTextEditor::setCursorAtLine(int blockNumber)
{
    setCursorPosition(blockNumber,0);
}

void KCTextEditor::emulatePressKey(QKeyEvent *keyEvent)
{
    event(keyEvent);
}

QList<int> KCTextEditor::getBreakPoints()
{
    QList<int> breakPointList;
    for(QTextBlock i=document()->begin();
        i.isValid();
        i=i.next())
    {
        KCTextBlockData *blockData=getBlockData(i);
        if(blockData->getMarkInfo().marked)
        {
            breakPointList.append(i.blockNumber());
        }
    }
    return breakPointList;
}

void KCTextEditor::zoomIn(int range)
{
    QFont zoomFont = font();
    const int newSize = zoomFont.pixelSize()+range;
    if(newSize<=0)
    {
        return;
    }
    zoomFont.setPixelSize(newSize);
    setFont(zoomFont);
    setTabWidth(tabSpace);
}

void KCTextEditor::zoomOut(int range)
{
    zoomIn(-range);
}

void KCTextEditor::resetDebugCursor()
{
    debugMarkPanel->resetDebugCursor();
}

void KCTextEditor::setDebugCursor(int lineNumber)
{
    setCursorPosition(lineNumber, 0);
    debugMarkPanel->setDebugCursor(lineNumber);
}

void KCTextEditor::foldCode(int startFoldBlockIndex)
{
    QTextBlock foldBlock=document()->findBlockByNumber(startFoldBlockIndex);
    KCTextBlockData *data=getBlockData(foldBlock);
    int startCodeLevel=data->getCodeLevel();
    foldBlock=foldBlock.next();
    while(foldBlock.isValid())
    {
        data=getBlockData(foldBlock);
        if(data->getCodeLevel()==startCodeLevel)
        {
            break;
        }
        foldBlock.setVisible(false);
        foldBlock=foldBlock.next();
    }
    viewport()->update();
}

void KCTextEditor::unfoldCode(int startUnfoldBlockIndex)
{
    QTextBlock foldBlock=document()->findBlockByNumber(startUnfoldBlockIndex);
    KCTextBlockData *data=getBlockData(foldBlock);
    data->setHasFolded(false);
    foldBlock=foldBlock.next();
    while(foldBlock.isValid() && (!foldBlock.isVisible()))
    {
        data=getBlockData(foldBlock);
        data->setHasFolded(false);
        foldBlock.setVisible(true);
        foldBlock=foldBlock.next();
    }
    viewport()->update();
}

void KCTextEditor::selectBlock(int blockNumber)
{
    QTextCursor cursor=textCursor();
    QTextBlock selectedBlock=document()->findBlockByNumber(blockNumber);
    cursor.setPosition(selectedBlock.position());
    cursor.movePosition(QTextCursor::NextCharacter,
                        QTextCursor::KeepAnchor,
                        selectedBlock.length()-1);
    setTextCursor(cursor);
    setHorizontalScrollValue(0);
}

void KCTextEditor::showEvent(QShowEvent *event)
{
    QPlainTextEdit::showEvent(event);
    updateAllPanels();
}

void KCTextEditor::paintEvent(QPaintEvent *e)
{
    QPlainTextEdit::paintEvent(e);
    updateAllPanels();
}

int KCTextEditor::lineNumberWidth()
{
    return (2+fontMetrics().width(QLatin1Char('9')))*QString::number(qMax(1, blockCount())).count();
}

void KCTextEditor::setLinePanelVisible(bool value)
{
    lineNumberPanel->setVisible(value);
    updatePanelAreaWidth();
}

void KCTextEditor::setCodeLevelVisible(bool value)
{
    unibodyPanel->setShowCodeLevel(value);
}

void KCTextEditor::updatePanelAreaWidth()
{
    setViewportMargins(panelManager->resizeManagerWidth(lineNumberWidth()), 0, 0, 0);
}

void KCTextEditor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);
    updateHighlights();
    updateSearchResults();
    QRect cr = contentsRect();
    panelManager->setGeometry(QRect(cr.left(),
                                    cr.top(),
                                    panelManager->resizeManagerWidth(lineNumberWidth()),
                                    cr.height()));
    //updatePanelAreaWidth();
}

void KCTextEditor::fileInfoChanged(const QFileInfo &_fileInfo)
{
    setDocumentTitle(_fileInfo.fileName());
    emit requireChangeLanguage(_fileInfo.suffix());

    setFilePath(_fileInfo.absoluteFilePath());
    setFileError(QFileDevice::NoError);
    document()->setModified(false);

    emit requireComputeExecFileName();
    emit requireSetHistoryDir(_fileInfo.absolutePath());
}

KCTextBlockData *KCTextEditor::getBlockData(const QTextBlock &b)
{
    return static_cast<KCTextBlockData *>(b.userData());
}

void KCTextEditor::panelPaintEvent(KCTextPanel *panel,
                                   QPaintEvent *event)
{
    QTextBlock block=firstVisibleBlock();
    int blockNumber=block.blockNumber();
    int top=(int)blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom=top+(int)blockBoundingRect(block).height();
    panel->setFirstBlock(block);
    KCTextBlockData *data;
    while(block.isValid() && top <= event->rect().bottom())
    {
        panel->setLastBlock(block);
        data=getBlockData(block);
        data->setRect(blockBoundingGeometry(block).toAlignedRect());
        if(block.isVisible() && bottom >= event->rect().top())
        {
            panel->drawContent(0, top, panel->width(),
                               fontMetrics().height()*block.lineCount(),
                               &block, data, textCursor());
        }
        block=block.next();
        top=bottom;
        bottom=top+(int)blockBoundingRect(block).height();
        ++blockNumber;
    }
}
