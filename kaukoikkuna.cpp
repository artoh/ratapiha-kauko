/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukoikkuna.h"

#include "editoriikkuna.h"

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

void KaukoIkkuna::editori()
{
    EditoriIkkuna* editoriIkkuna = new EditoriIkkuna(skene_->nakyma());
    editoriIkkuna->show();
}


void KaukoIkkuna::luoAktiot()
{
    uusiIkkunaAktio_ = new QAction( QIcon(":/r/pic/ikkuna-uusi.png"), tr("Uusi ikkuna"), this );
    uusiIkkunaAktio_->setToolTip(tr("Avaa uuden ikkunan"));
    connect( uusiIkkunaAktio_, SIGNAL(triggered()), this, SLOT(uusiIkkuna()));

    editoriAktio_ = new QAction( QIcon(":/r/pic/muokkaa.png"), tr("Muokkaa"), this );
    editoriAktio_->setToolTip(tr("Muokkaa rataa tai näkymiä"));
    connect( editoriAktio_, SIGNAL(triggered()), this, SLOT(editori()));

}

void KaukoIkkuna::luoTyoPalkki()
{
    hallintaToolBar_ = addToolBar( tr("Hallinta"));
    hallintaToolBar_->addAction(uusiIkkunaAktio_);

    // Luodaan näkymän valinta
    nakymaValinta_ = new QComboBox;


    QSqlQuery nakymaKysely("select nakyma,nakymanimi from nakyma");
    while( nakymaKysely.next())
    {
        // Viedään näkymätiedot valintalaatikkoon
        nakymaValinta_->addItem( nakymaKysely.value(1).toString(), nakymaKysely.value(0).toInt());
    }
    connect( nakymaValinta_, SIGNAL(currentIndexChanged(int)), this, SLOT(nakymanVaihto(int)));
    hallintaToolBar_->addWidget(nakymaValinta_);

    hallintaToolBar_->addAction(editoriAktio_);


}
