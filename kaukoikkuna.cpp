/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukoikkuna.h"

#include <QSqlQuery>
#include <QIcon>

KaukoIkkuna::KaukoIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Kaukokäyttö");

    skene_ = new KaukoScene;
    view_ = new QGraphicsView(skene_);
    view_->scale(2.5, 2.5);

    luoAktiot();
    luoTyoPalkki();

    setCentralWidget(view_);

    nakymanVaihto(0);
}


void KaukoIkkuna::uusiIkkuna()
{
    KaukoIkkuna* uusiikkuna = new KaukoIkkuna;
    uusiikkuna->show();
}

void KaukoIkkuna::nakymanVaihto(int valintaind)
{
    skene_->haeNakyma( nakymaValinta_->itemData(valintaind).toInt() );
}


void KaukoIkkuna::luoAktiot()
{
    uusiIkkunaAktio_ = new QAction( QIcon(":/r/pic/ikkuna-uusi.png"), tr("Uusi ikkuna"), this );
    uusiIkkunaAktio_->setToolTip(tr("Avaa uuden ikkunan"));
    connect( uusiIkkunaAktio_, SIGNAL(triggered()), this, SLOT(uusiIkkuna()));

}

void KaukoIkkuna::luoTyoPalkki()
{
    hallintaToolBar_ = addToolBar( tr("Hallinta"));
    hallintaToolBar_->addAction(uusiIkkunaAktio_);

    // Luodaan näkymän valinta
    nakymaValinta_ = new QComboBox;


    QSqlQuery nakymaKysely("select nakyma,nakymanimi from kaukonakyma");
    while( nakymaKysely.next())
    {
        // Viedään näkymätiedot valintalaatikkoon
        nakymaValinta_->addItem( nakymaKysely.value(1).toString(), nakymaKysely.value(0).toInt());
    }
    connect( nakymaValinta_, SIGNAL(currentIndexChanged(int)), this, SLOT(nakymanVaihto(int)));
    hallintaToolBar_->addWidget(nakymaValinta_);


}