/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editoriikkuna.h"

#include <QSqlQuery>

EditoriIkkuna::EditoriIkkuna(int nakyma, QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Editori");

    skene_ = new EditoriScene;
    view_ = new EditoriView( skene_);

    view_->scale(2.0, 2.0);

    luoAktiot();
    luoTyoPalkki();

    setCentralWidget(view_);

    nakymaValinta_->setCurrentIndex( nakymaValinta_->findData(nakyma) );
    skene_->haeNakyma(nakyma);
}


void EditoriIkkuna::nakymanVaihto(int valintaind)
{
    skene_->haeNakyma( nakymaValinta_->itemData( valintaind).toInt());
}


void EditoriIkkuna::luoAktiot()
{


}

void EditoriIkkuna::luoTyoPalkki()
{
    hallintaToolBar_ = addToolBar( tr("Hallinta"));

    // Luodaan näkymän valinta
    nakymaValinta_ = new QComboBox;
    nakymaValinta_->addItem("RATA",0);

    QSqlQuery nakymaKysely("select nakyma,nakymanimi from kaukonakyma");
    while( nakymaKysely.next())
    {
        // Viedään näkymätiedot valintalaatikkoon
        nakymaValinta_->addItem( nakymaKysely.value(1).toString(), nakymaKysely.value(0).toInt());
    }
    connect( nakymaValinta_, SIGNAL(currentIndexChanged(int)), this, SLOT(nakymanVaihto(int)));
    hallintaToolBar_->addWidget(nakymaValinta_);

}
