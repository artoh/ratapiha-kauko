/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 26.6.2011
**************************************************************************/


#include "vaunukataloogi.h"

#include <QMouseEvent>
#include <QApplication>
#include <QDrag>

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
