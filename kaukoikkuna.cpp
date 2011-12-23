#include "kaukoikkuna.h"

#include <QSqlQuery>
#include <QIcon>

KaukoIkkuna::KaukoIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Kaukokäyttö");

    luoAktiot();
    luoTyoPalkki();
}


void KaukoIkkuna::uusiIkkuna()
{
    KaukoIkkuna* uusiikkuna = new KaukoIkkuna;
    uusiikkuna->show();
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

    hallintaToolBar_->addWidget(nakymaValinta_);


}
