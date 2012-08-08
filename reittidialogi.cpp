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

#include "reittidialogi.h"
#include "reititmodel.h"
#include "reittidelegaatti.h"

#include <QSqlQuery>
#include <QListWidgetItem>
#include <QMapIterator>
#include <QModelIndexList>
#include <QItemSelectionModel>
#include <QMessageBox>


ReittiDialogi::ReittiDialogi(QWidget* /*parent*/) :
    QWidget(0),
    uudenReitinItem_(0)
{
    setupUi(this);

    reittiModel_ = new ReititModel(this);
    reittiView->setModel(reittiModel_);
    reittiView->setItemDelegate( new ReittiDelegaatti(reittiModel_) );
    reittiView->setEditTriggers( QTableView::AllEditTriggers);

    lataaReitit();

    QMapIterator<QString,QString> i( *reittiModel_->liikennepaikkalista());
    while( i.hasNext()  )
    {
        i.next();
        QListWidgetItem* item = new QListWidgetItem( i.value(), liikennepaikkaLista );
        item->setData(Qt::UserRole, i.key() );
    }


    connect( reittiLista, SIGNAL(currentTextChanged(QString)), this, SLOT(haeReitti(QString)));

    connect( lisaaReittiNappi, SIGNAL(clicked()), this, SLOT( uusiReitti()) );
    connect( kopioiNappi, SIGNAL(clicked()), this, SLOT(kopioiReitti()));
    connect( poistaReittiNappi, SIGNAL(clicked()), this, SLOT(poistaReitti()));

    connect( lisaaPysaysNappi, SIGNAL(clicked()), this, SLOT(lisaaPysays()));
    connect( poistaPysaysNappi, SIGNAL(clicked()), this, SLOT(poistaPysays()));

    connect( hyvaksyNappi, SIGNAL(clicked()), this, SLOT(tallenna()));
    connect( hylkaaNappi, SIGNAL(clicked()), reittiModel_, SLOT(peruMuokkaukset()));

    connect( reittiModel_, SIGNAL(muokattu(bool)), this, SLOT(reittiaMuokattu(bool)));
    connect( reittiModel_, SIGNAL(uusiHylatty()), this, SLOT(poistaReitti()));

    tallennusVaroitus->setVisible(false);

}

void ReittiDialogi::lataaReitit()
{
    reittiLista->clear();
    QSqlQuery kysely("select distinct(reitti) from aikataulu order by reitti");
    while(kysely.next())
    {
        QListWidgetItem* item = new QListWidgetItem( kysely.value(0).toString(), reittiLista);
        item->setFlags( item->flags() | Qt::ItemIsEditable);
    }

}

void ReittiDialogi::lisaaPysays()
{
    QListWidgetItem* item= liikennepaikkaLista->currentItem();
    if( item )
        reittiModel_->lisaaPysahdys( item->data(Qt::UserRole).toString() );
}

void ReittiDialogi::poistaPysays()
{
    if( reittiView->selectionModel()->hasSelection())
    {
        QModelIndexList valintalista = reittiView->selectionModel()->selectedIndexes();
        if( !valintalista.isEmpty() )
            reittiModel_->poistaRivi( valintalista.first().row() );
    }
}

void ReittiDialogi::haeReitti(const QString &reitti)
{
    if( !reitti.isEmpty())
    {
        valintaVaihtuu();
        reittiModel_->valitseReitti(reitti);
    }
}

void ReittiDialogi::uusiReitti()
{
    valintaVaihtuu();
    uudenReitinItem_ = new QListWidgetItem("", reittiLista);
    uudenReitinItem_->setFlags( uudenReitinItem_->flags() | Qt::ItemIsEditable);

    reittiModel_->uusiReitti();
    reittiLista->clearSelection();
    uudenReitinItem_->setSelected(true);
    reittiLista->editItem(uudenReitinItem_); // Määrää nimen syötettäväksi
    tallennusVaroitus->setVisible(true);
}

void ReittiDialogi::reittiaMuokattu(bool onko)
{
    if( onko )
    {
        tallennusVaroitus->setVisible(true);
    }
    else
        tallennusVaroitus->setVisible(false);
}


void ReittiDialogi::valintaVaihtuu()
{
    if( uudenReitinItem_ )
    {
        uudenReitinItem_->setHidden(true);
        uudenReitinItem_ = 0;
    }
    tallennusVaroitus->setVisible(false);
}

void ReittiDialogi::kopioiReitti()
{
    valintaVaihtuu();
    uudenReitinItem_ = new QListWidgetItem("", reittiLista);
    uudenReitinItem_->setFlags( uudenReitinItem_->flags() | Qt::ItemIsEditable);

    reittiModel_->reitistaKopio();
    reittiLista->clearSelection();
    uudenReitinItem_->setSelected(true);
    reittiLista->editItem(uudenReitinItem_); // Määrää nimen syötettäväksi
    tallennusVaroitus->setVisible(true);
}

void ReittiDialogi::tallenna()
{
    QString uusitunnus;
    if( uudenReitinItem_ )
        uusitunnus = uudenReitinItem_->text();
    else if( !reittiLista->currentItem() )
        uusitunnus = reittiModel_->reittiTunnus();
    else
        uusitunnus = reittiLista->currentItem()->text();

    if( uusitunnus.length() < 3)
    {
        QMessageBox::critical(this,tr("Reitin tallennus"),tr("Reitin tunnus %1 ei kelpaa (vähimmäispituus kolme merkkiä).").arg(uusitunnus));
        return;

    }

    bool muutettuTunnusta = false;
    // Jos tunnus vaihtuu, pitää varmistaa, että tunnus kelpaa
    if( uusitunnus != reittiModel_->reittiTunnus())
    {
        QSqlQuery varmistus( QString("select reitti from aikataulu where reitti=\"%1\" ")
                                     .arg(uusitunnus) );
        if( varmistus.next())
        {
            // Reitin tunnus on jo käytetössä
            QMessageBox::critical(this, tr("Reitin tallennus"),tr("Reitin tunnus %1 on jo toisen reitin käytössä").arg(uusitunnus));
            return;
        }
        muutettuTunnusta = true;

    }
    // Nyt näyttää kaikki kelpaavan, eli voidaan tallentaa!
    reittiModel_->tallenna(uusitunnus);

    if( uudenReitinItem_ || muutettuTunnusta )
    {
        emit reittiListaaMuutettu();
        uudenReitinItem_ = 0;   // Tallennettu!
    }
    emit muutettu();
}

void ReittiDialogi::poistaReitti()
{
    int indeksi = 1;
    // Poistaa nykyisen reitin, jos sellainen on... ja valitsee listan ensimmäisen
    // Ensimmäisenä pitäisi poistaa listalta
    if( reittiLista->currentItem())
    {
        indeksi = reittiLista->currentRow();
        reittiLista->currentItem()->setHidden(true);
    }
    reittiLista->clearSelection();

    // Sitten poistetaan tietokannasta...
    if( !reittiModel_->reittiTunnus().isEmpty())
        QSqlQuery poisto( QString("delete from aikataulu where reitti=\"%1\"").arg( reittiModel_->reittiTunnus()  ));

    QListWidgetItem* seuraavaValittu = reittiLista->itemAt(0,indeksi - 1);
    if( seuraavaValittu)
    {
        haeReitti( seuraavaValittu->text());
        seuraavaValittu->setSelected(true);
    }
    emit reittiListaaMuutettu();
}

