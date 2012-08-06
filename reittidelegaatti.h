/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
5.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#ifndef REITTIDELEGAATTI_H
#define REITTIDELEGAATTI_H

#include "reititmodel.h"

#include <QItemDelegate>


class ReittiDelegaatti : public QItemDelegate
{
    Q_OBJECT
public:
    ReittiDelegaatti(ReititModel* model);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

signals:

private slots:
    void commitAndCloseEditor();


protected:
    ReititModel* model_;

};

#endif // REITTIDELEGAATTI_H
