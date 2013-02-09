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

#include "reittidelegaatti.h"
#include "reititmodel.h"

#include <QComboBox>
#include <QTimeEdit>
#include <QSqlQuery>
#include <QPainter>
#include <QBrush>

ReittiDelegaatti::ReittiDelegaatti(ReititModel *model) :
    QItemDelegate(),
    model_(model)
{
}

void ReittiDelegaatti::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{

    // Piirtää delegaatilla sarakkeet: Aika, Pysähtyy, Tyyppi, Suunta
    QStyleOptionViewItem optioni = option;
    QString teksti;

    optioni.displayAlignment = Qt::AlignLeft | Qt::AlignVCenter;

    if( index.column() == ReititModel::Liikennepaikka)
    {
        teksti = model_->liikennepaikkalista()->value( index.model()->data(index, Qt::DisplayRole).toString() );
    }
    else if( index.column() == ReititModel::Raide)
    {
        int raide = index.model()->data(index, Qt::DisplayRole).toInt();
        if( raide )
            teksti = QString("%1").arg(raide, 3, 10, QChar('0') );
        else
        {
            // Jos 0, kelvoton, niin punainen tausta eikä aikaa!
            painter->save();
            painter->setBrush( QBrush(Qt::red));
            painter->setPen( Qt::NoPen);
            painter->drawRect( optioni.rect);
            painter->restore();
        }

    }
    else if( index.column() == ReititModel::LahtoAika || index.column() == ReititModel::SaapumisAika )
    {
        QTime aika = index.model()->data(index, Qt::DisplayRole).toTime();
        if( aika.isNull())
        {
            // Jos NULL, niin keltainen tausta eikä aikaa!
            painter->save();
            painter->setBrush( QBrush(Qt::yellow));
            painter->setPen( Qt::NoPen);
            painter->drawRect( optioni.rect);
            painter->restore();
        }
        else
            teksti = aika.toString("hh:mm");
    }
    else if( index.column() == ReititModel::Pysahtyy)
    {
        if( index.model()->data(index, Qt::DisplayRole).toInt() )
        {
            QTime pysaysaika = QTime(0,0).addSecs( index.model()->data(index, Qt::DisplayRole).toInt() );
            teksti = tr("%1 min").arg(pysaysaika.toString("m.ss"));
        }
    }
    else if( index.column() == ReititModel::Tapahtuma)
    {
        int tyyppi = index.model()->data(index, Qt::DisplayRole).toInt();

        if( tyyppi == ReittiTieto::Pysahtyy)
            teksti = tr("Pysähtyy");
        else if(tyyppi == ReittiTieto::Saapuu)
            teksti = tr("Saapuu");
        else if( tyyppi == ReittiTieto::Lahtee)
            teksti = tr("Lähtee");
        else if( tyyppi == ReittiTieto::Ohittaa)
            teksti = tr("Ohittaa");
    }
    else if( index.column() == ReititModel::Suunta)
    {
        int suunta = index.model()->data(index, Qt::DisplayRole).toInt();
        if( suunta == RaiteenPaa::Pohjoiseen)
            teksti = "Pohjoiseen";
        else if( suunta == RaiteenPaa::Etelaan)
            teksti = "Etelään";
    }
    else if( index.column() == ReititModel::LokiLahtoaika || index.column() == ReititModel::LokiSaapuiAika)
    {
        QTime aika = index.model()->data(index, Qt::DisplayRole).toTime();
        if( aika.isValid())
            teksti = aika.toString("hh:mm:ss");

    }

    drawDisplay( painter, optioni, optioni.rect, teksti);
    drawFocus( painter, optioni, optioni.rect);

}

QWidget * ReittiDelegaatti::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    if( index.column() == ReititModel::LahtoAika)
    {
        QTimeEdit* editori = new QTimeEdit(parent);
        editori->setDisplayFormat("hh:mm");
        connect( editori, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
        return editori;
    }
    else if( index.column() == ReititModel::Pysahtyy)
    {
        QTimeEdit* editori = new QTimeEdit(parent);
        editori->setDisplayFormat("m:ss");
        connect( editori, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));
        return editori;
    }
    else if( index.column() == ReititModel::Raide)
    {
        QComboBox* editori = new QComboBox(parent);

        // Haetaan liikennepaikka samalta riviltä
        QString liikennepaikka = index.model()->data( index.sibling( index.row(), 0 ) ).toString();

        QSqlQuery kysely( QString("SELECT distinct(raide) from kisko where nakyma=0 and "
                                   "liikennepaikka=\"%1\" order by raide").arg(liikennepaikka) );
        while( kysely.next())
        {
            int raide = kysely.value(0).toInt();
            editori->addItem( QString("%1").arg(raide,3,10,QChar('0')) , raide );
        }
        connect( editori, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndCloseEditor()));
        return editori;
    }
    else if( index.column() == ReititModel::Suunta)
    {
        QComboBox* editori = new QComboBox(parent);
        editori->addItem(tr("Pohjoiseen"), RaiteenPaa::Pohjoiseen);
        editori->addItem(tr("Etelään"), RaiteenPaa::Etelaan);
        connect( editori, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndCloseEditor()));
        return editori;
    }

    else if( index.column() == ReititModel::Tapahtuma)
    {
        QComboBox* editori = new QComboBox(parent);
        editori->addItem(tr("Pysähtyy"), ReittiTieto::Pysahtyy);
        editori->addItem(tr("Lähtee"), ReittiTieto::Lahtee);
        editori->addItem( tr("Saapuu"), ReittiTieto::Saapuu);
        editori->addItem(tr("Ohittaa"), ReittiTieto::Ohittaa);
        connect( editori, SIGNAL(currentIndexChanged(int)), this, SLOT(commitAndCloseEditor()));
        return editori;
    }


    return QItemDelegate::createEditor(parent, option, index);
}

void ReittiDelegaatti::commitAndCloseEditor()
{
    QWidget* editori = qobject_cast<QWidget*>(sender());
    emit commitData(editori);
    emit closeEditor(editori);
}

void ReittiDelegaatti::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if( index.column() == ReititModel::LahtoAika)
    {
        QTimeEdit* editori = qobject_cast<QTimeEdit*>(editor);
        editori->setTime( index.model()->data(index).toTime() );
    }
    else if( index.column() == ReititModel::Pysahtyy)
    {
        QTimeEdit* editori = qobject_cast<QTimeEdit*>(editor);
        int sekunnit =  index.model()->data(index).toInt();
        editori->setTime( QTime(0, sekunnit/60, sekunnit % 60) );
    }
    else if( index.column() == ReititModel::Raide || index.column() == ReititModel::Suunta ||
            index.column() == ReititModel::Tapahtuma)
    {
        QComboBox* editori = qobject_cast<QComboBox*>(editor);
        editori->setCurrentIndex( editori->findData( index.model()->data(index).toInt() ));
    }
}

void ReittiDelegaatti::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if( index.column() == ReititModel::LahtoAika)
    {
        QTimeEdit* editori = qobject_cast<QTimeEdit*>(editor);
        model->setData( index, editori->time());
    }
    else if( index.column() == ReititModel::Pysahtyy)
    {
        QTimeEdit* editori = qobject_cast<QTimeEdit*>(editor);
        QTime aika = editori->time();
        int sekunnit = aika.minute() * 60 + aika.second();
        model->setData( index, sekunnit );
    }
    else if( index.column() == ReititModel::Raide || index.column() == ReititModel::Suunta ||
            index.column() == ReititModel::Tapahtuma)
    {
        QComboBox* editori = qobject_cast<QComboBox*>(editor);
        model->setData( index, editori->itemData( editori->currentIndex(), Qt::UserRole ));
    }




}
