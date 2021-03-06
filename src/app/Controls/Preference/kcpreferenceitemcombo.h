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

#ifndef KCPREFERENCEITEMCOMBO_H
#define KCPREFERENCEITEMCOMBO_H

#include <QList>
#include "kcpreferenceitembase.h"

class QLabel;
class QComboBox;

class KCPreferenceItemCombo : public KCPreferenceItemBase
{
    Q_OBJECT
public:
    explicit KCPreferenceItemCombo(QWidget *parent = 0);
    ~KCPreferenceItemCombo();
    void setOriginalValue(const QVariant &value);
    void refreshComboText();
    QList<QString> getComboTextList() const;
    void setComboTextList(const QList<QString> &value);
    void setComboCaptionText(const QString &value);

signals:

public slots:

private slots:

protected:
    QVariant getUserNewValue();
    void refreshValueDisplay();
    void setEditWidgetStatus(bool states);
    void setWidgetValue(QVariant newWidgeValue);

private:
    QList<QString> comboTextList;

    QBoxLayout *comboLayout;
    QLabel *comboCaption;
    QComboBox *comboData;
};

#endif // KCPREFERENCEITEMCOMBO_H
