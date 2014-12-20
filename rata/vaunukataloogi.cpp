/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  See <http://www.gnu.org/licenses/>
**
**
**************************************************************************/


#include "vaunukataloogi.h"

#include <QMouseEvent>
#include <QApplication>
#include <QDrag>
#include <QMimeData>

VaunuKataloogi::VaunuKataloogi(QWidget *parent) :
    QListWidget(parent)
{
    setViewMode( IconMode );
    setIconSize( QSize(100,60));
    setMovement( Static );
    setViewMode( IconMode );
    setIconSize( QSize(100,60));
    setMovement( Static );

    QStringList tyypit;
    tyypit << "Sr1" <<  "Sr2" << "Sm2" << "Sm3v" << "Sm3o" << "Sm4v" << "Sm4o" << "Dv12" << "Dr16" << "Dm12" << "Eiu" << "Ei3" << "Gbln" << "Cabb" << "Eio";

    foreach( QString tyyppi, tyypit)
        new QListWidgetItem( QIcon( QString(":/r/junakuvat/%1.png").arg(tyyppi)), tyyppi, this );


}

void VaunuKataloogi::mousePressEvent(QMouseEvent *event)
{
    if( event->buttons() & Qt::LeftButton)
        aloitusPaikka_ = event->pos();
    QListWidget::mousePressEvent(event);
}

void VaunuKataloogi::mouseMoveEvent(QMouseEvent *e)
{
    if( e->buttons() & Qt::LeftButton )
    {
        int matka = (e->pos() - aloitusPaikka_).manhattanLength();
        if( matka >= QApplication::startDragDistance())
            teeDrag();
    }
    QListWidget::mouseMoveEvent(e);
}

void VaunuKataloogi::teeDrag()
{
    QListWidgetItem* item = currentItem();
    if( item )
    {
        QMimeData *mimedata = new QMimeData;
        mimedata->setText(item->text());

        QDrag *drag = new QDrag(this);
        drag->setMimeData(mimedata);
        drag->setPixmap( item->icon().pixmap(80,60) );
        drag->exec( Qt::CopyAction);
    }
}
