/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukoikkuna.h"

#include "editoriikkuna.h"
#include "rataikkuna.h"

#include <QSqlQuery>
#include <QIcon>
#include <QDebug>
#include <QStatusBar>

KaukoIkkuna::KaukoIkkuna(QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Kaukokäyttö");

    skene_ = new KaukoScene;
    view_ = new KaukoView(skene_);
    view_->scale(2.5, 2.5);

    view_->valitseTila(KaukoView::Vierita);

    luoAktiot();
    luoTyoPalkki();

    setCentralWidget(view_);

    connect( view_, SIGNAL(vastausKomentoon(QString)), statusBar(), SLOT( showMessage(QString))  );
    connect( view_, SIGNAL(tilaVaihtunut(int)), this, SLOT(paivitaNapit(int)));

    nakymanVaihto(0);
}


void KaukoIkkuna::rataIkkuna()
{
    RataIkkuna* rataikkuna = new RataIkkuna;
    rataikkuna->show();
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


void KaukoIkkuna::kasky()
{
    qDebug() << RataIkkuna::rataSkene()->ASLKasky( kaskyLine_->text());
    kaskyLine_->clear();
}

void KaukoIkkuna::vaihdaTila()
{
    // Vaihtaa valitun toimintatilan
    QObject* lahettaja = sender();
    QAction* aktio = qobject_cast<QAction*>(lahettaja);

    if( aktio)
        view_->valitseTila(aktio->data().toInt());
}

void KaukoIkkuna::paivitaNapit(int tila)
{
    kulkutieAktio_->setChecked( tila == KaukoView::JunaKulkutieAlkaa || tila == KaukoView::JunaKulkutiePaattyy);
    vaihtoKulkutieAktio_->setChecked( tila == KaukoView::VaihtoKulkutieAlkaa || tila==KaukoView::VaihtoKulkutiePaattyy);
    puraKulkutieAktio_->setChecked( tila == KaukoView::PeruKulkutie);
    kaannaVaihdeAktio_->setChecked(tila==KaukoView::VaihteenKaanto);
    seisAktio_->setChecked( tila == KaukoView::SeisKasky);
    ajaAktio_->setChecked( tila == KaukoView::AjaKasky);


}

void KaukoIkkuna::luoAktiot()
{
    rataIkkunaAktio_ = new QAction( QIcon(":/r/pic/varojunaa.png"),tr("Rata"), this);
    rataIkkunaAktio_->setToolTip("Avaa rataikkunan");
    connect( rataIkkunaAktio_, SIGNAL(triggered()), this, SLOT(rataIkkuna()));

    uusiIkkunaAktio_ = new QAction( QIcon(":/r/pic/ikkuna-uusi.png"), tr("Uusi ikkuna"), this );
    uusiIkkunaAktio_->setToolTip(tr("Avaa uuden ikkunan"));
    connect( uusiIkkunaAktio_, SIGNAL(triggered()), this, SLOT(uusiIkkuna()));

    editoriAktio_ = new QAction( QIcon(":/r/pic/muokkaa.png"), tr("Muokkaa"), this );
    editoriAktio_->setToolTip(tr("Muokkaa rataa tai näkymiä"));
    connect( editoriAktio_, SIGNAL(triggered()), this, SLOT(editori()));


    kulkutieAktio_ = new QAction(tr("Junakulkutie"),this);
    kulkutieAktio_->setData(KaukoView::JunaKulkutieAlkaa);
    kulkutieAktio_->setIcon(QIcon(":/r/pic/vihrea.png"));
    kulkutieAktio_->setCheckable(true);
    kulkutieAktio_->setToolTip(tr("Muodosta junakulkutie"));
    connect( kulkutieAktio_, SIGNAL( triggered()), this, SLOT(vaihdaTila()));

    vaihtoKulkutieAktio_ = new QAction( tr("Vaihtokulkutie"), this);
    vaihtoKulkutieAktio_->setData(KaukoView::VaihtoKulkutieAlkaa);
    vaihtoKulkutieAktio_->setIcon(QIcon(":/r/pic/raideopastin.png"));
    vaihtoKulkutieAktio_->setCheckable(true);
    vaihtoKulkutieAktio_->setToolTip(tr("Muodosta vaihtokulkutie"));
    connect( vaihtoKulkutieAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTila()));

    puraKulkutieAktio_ = new QAction( tr("Peru kulkutie"),this);
    puraKulkutieAktio_->setIcon( QIcon(":/r/pic/kulkutienpurku.png"));
    puraKulkutieAktio_->setCheckable(true);
    puraKulkutieAktio_->setData(KaukoView::PeruKulkutie);
    puraKulkutieAktio_->setCheckable(true);
    connect( puraKulkutieAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTila()));

    kaannaVaihdeAktio_ = new QAction( tr("Käännä vaihde"),this);
    kaannaVaihdeAktio_->setIcon( QIcon(":/r/pic/vaihteenkaanto.png"));
    kaannaVaihdeAktio_->setCheckable(true);
    kaannaVaihdeAktio_->setData(KaukoView::VaihteenKaanto);
    connect( kaannaVaihdeAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTila()));

    seisAktio_ = new QAction( tr("Opastimet SEIS-opasteelle"),this);
    seisAktio_->setIcon( QIcon(":/r/pic/seiskasky.png"));
    seisAktio_->setData(KaukoView::SeisKasky);
    seisAktio_->setCheckable(true);
    connect( seisAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTila()));

    ajaAktio_ = new QAction( tr("Opastimen vapautus SEIS-asennosta"),this);
    ajaAktio_->setIcon( QIcon(":/r/pic/ajakasky.png"));
    ajaAktio_->setData(KaukoView::AjaKasky);
    ajaAktio_->setCheckable(true);
    connect( ajaAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTila()));


}

void KaukoIkkuna::luoTyoPalkki()
{
    hallintaToolBar_ = addToolBar( tr("Hallinta"));

    hallintaToolBar_->addAction( rataIkkunaAktio_);
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

    asetinlaiteToolBar_ = addToolBar( tr("Asetinlaite"));
    asetinlaiteToolBar_->addAction( kulkutieAktio_ );
    asetinlaiteToolBar_->addAction( vaihtoKulkutieAktio_);
    asetinlaiteToolBar_->addAction( puraKulkutieAktio_);
    asetinlaiteToolBar_->addSeparator();
    asetinlaiteToolBar_->addAction( kaannaVaihdeAktio_);
    asetinlaiteToolBar_->addSeparator();
    asetinlaiteToolBar_->addAction( seisAktio_);
    asetinlaiteToolBar_->addAction( ajaAktio_);


    kaskyLine_ = new QLineEdit;
    connect( kaskyLine_, SIGNAL(returnPressed()), this, SLOT(kasky()));
    hallintaToolBar_->addWidget(kaskyLine_);


}
