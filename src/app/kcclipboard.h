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

#ifndef KCCLIPBOARD_H
#define KCCLIPBOARD_H

#include <QObject>
#include <QString>

class QStandardItemModel;
class QStandardItem;
class QClipboard;

class KCClipboard : public QObject
{
    Q_OBJECT
public:
    static KCClipboard *getInstance();

    QStandardItemModel *getClipboardTextsModel() const;
    void setClipboardTextsModel(QStandardItemModel *value);
    QString getHistoryClipboardText(int itemIndex);

    int getMaxDataCount();
    void setMaxDataCount(int value);
    void addToClipboardStack(QString _text);

signals:

public slots:
    void onSystemClipboardChanged();

private:
    KCClipboard();
    static KCClipboard *instance;

    int maxDataCount=10;
    QStandardItemModel *clipboardTextsModel;
    QStandardItem *clipboardTextsModelRoot;

    bool ignoreSignal=false;
    QClipboard *systemClipboard;
};

#endif // KCCLIPBOARD_H
