/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukoikkuna.h"

#include "editoriikkuna.h"
#include "rataikkuna.h"
#include "ratapihaikkuna.h"

#include <QSqlQuery>
#include <QIcon>
#include <QDebug>
#include <QStatusBar>

KaukoIkkuna::KaukoIkkuna(RatapihaIkkuna *parent) :
    QMainWindow(parent),
    rpIkkuna_(parent)
{

    skene_ = new KaukoScene;
    view_ = new KaukoView(skene_);
    view_->scale(2.5, 2.5);

    view_->valitseTila(KaukoView::Vierita);

    luoAktiot();
    luoTyoPalkki();

    aikaLabel_ = new QLabel("Tervetuloa!");
    statusBar()->addPermanentWidget(aikaLabel_);

    setCentralWidget(view_);

    connect( rpIkkuna_, SIGNAL(aslVastaus(QString)), statusBar(), SLOT(showMessage(QString)));
    connect( view_, SIGNAL(tilaVaihtunut(int)), this, SLOT(paivitaNapit(int)));
    connect( rpIkkuna_, SIGNAL(yhdistetty(bool)), this, SLOT(yhteysAsettimeen(bool)));

    yhteysAsettimeen( rpIkkuna_->onkoYhteydessa());
    nakymanVaihto(0);

}

void KaukoIkkuna::uusiIkkuna()
{
    KaukoIkkuna* uusiikkuna = new KaukoIkkuna(rpIkkuna_);
    uusiikkuna->show();
}

void KaukoIkkuna::nakymanVaihto(int valintaind)
{
    skene_->haeNakyma( nakymaValinta_->itemData(valintaind).toInt() );
    setWindowTitle( tr("Kaukokäyttö - %1").arg( nakymaValinta_->currentText() ));
}

void KaukoIkkuna::editori()
{
    EditoriIkkuna* editoriIkkuna = new EditoriIkkuna(skene_->nakyma());
    editoriIkkuna->show();
}


void KaukoIkkuna::kasky()
{
    rpIkkuna_->aslKasky( kaskyLine_->text() );
    kaskyLine_->clear();
}

void KaukoIkkuna::vaihdaTila(QAction* aktio)
{
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

void KaukoIkkuna::yhteysAsettimeen(bool onkoYhteytta)
{
    aslAktiot_->setEnabled( onkoYhteytta );
    aikaLabel_->setEnabled( onkoYhteytta );
}

void KaukoIkkuna::paivitaKello(const QDateTime &aika)
{
    aikaLabel_->setText( aika.toString("dd.MM.yyyy HH:mm"));
}

void KaukoIkkuna::luoAktiot()
{

    uusiIkkunaAktio_ = new QAction( QIcon(":/r/pic/ikkuna-uusi.png"), tr("Uusi ikkuna"), this );
    uusiIkkunaAktio_->setToolTip(tr("Avaa uuden ikkunan"));
    connect( uusiIkkunaAktio_, SIGNAL(triggered()), this, SLOT(uusiIkkuna()));

    editoriAktio_ = new QAction( QIcon(":/r/pic/muokkaa.png"), tr("Muokkaa"), this );
    editoriAktio_->setToolTip(tr("Muokkaa rataa tai näkymiä"));
    connect( editoriAktio_, SIGNAL(triggered()), this, SLOT(editori()));

    aslAktiot_ = new QActionGroup(this);
    aslAktiot_->setExclusive(false);

    kulkutieAktio_ = new QAction(tr("Junakulkutie"),this);
    kulkutieAktio_->setData(KaukoView::JunaKulkutieAlkaa);
    kulkutieAktio_->setIcon(QIcon(":/r/pic/vihrea.png"));
    kulkutieAktio_->setCheckable(true);
    kulkutieAktio_->setToolTip(tr("Muodosta junakulkutie"));
    aslAktiot_->addAction(kulkutieAktio_);
    connect( aslAktiot_, SIGNAL(triggered(QAction*)), this, SLOT(vaihdaTila(QAction*)));

    vaihtoKulkutieAktio_ = new QAction( tr("Vaihtokulkutie"), this);
    vaihtoKulkutieAktio_->setData(KaukoView::VaihtoKulkutieAlkaa);
    vaihtoKulkutieAktio_->setIcon(QIcon(":/r/pic/raideopastin.png"));
    vaihtoKulkutieAktio_->setCheckable(true);
    vaihtoKulkutieAktio_->setToolTip(tr("Muodosta vaihtokulkutie"));
    aslAktiot_->addAction(vaihtoKulkutieAktio_);

    puraKulkutieAktio_ = new QAction( tr("Peru kulkutie"),this);
    puraKulkutieAktio_->setIcon( QIcon(":/r/pic/kulkutienpurku.png"));
    puraKulkutieAktio_->setCheckable(true);
    puraKulkutieAktio_->setData(KaukoView::PeruKulkutie);
    puraKulkutieAktio_->setCheckable(true);
    aslAktiot_->addAction(puraKulkutieAktio_);

    junanumeroAktio_ = new QAction( tr("Syötä junanumero"), this);
    junanumeroAktio_->setIcon( QIcon(":/r/pic/junanumero.png"));
    junanumeroAktio_->setCheckable(true);
    junanumeroAktio_->setData( KaukoView::JunaNumeronSyotto);

    kaannaVaihdeAktio_ = new QAction( tr("Käännä vaihde"),this);
    kaannaVaihdeAktio_->setIcon( QIcon(":/r/pic/vaihteenkaanto.png"));
    kaannaVaihdeAktio_->setCheckable(true);
    kaannaVaihdeAktio_->setData(KaukoView::VaihteenKaanto);
    aslAktiot_->addAction(kaannaVaihdeAktio_);

    seisAktio_ = new QAction( tr("Opastimet SEIS-opasteelle"),this);
    seisAktio_->setIcon( QIcon(":/r/pic/seiskasky.png"));
    seisAktio_->setData(KaukoView::SeisKasky);
    seisAktio_->setCheckable(true);
    aslAktiot_->addAction(seisAktio_);

    ajaAktio_ = new QAction( tr("Opastimen vapautus SEIS-asennosta"),this);
    ajaAktio_->setIcon( QIcon(":/r/pic/ajakasky.png"));
    ajaAktio_->setData(KaukoView::AjaKasky);
    ajaAktio_->setCheckable(true);
    aslAktiot_->addAction(ajaAktio_);


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

    asetinlaiteToolBar_ = addToolBar( tr("Asetinlaite"));
    asetinlaiteToolBar_->addAction( kulkutieAktio_ );
    asetinlaiteToolBar_->addAction( vaihtoKulkutieAktio_);
    asetinlaiteToolBar_->addAction( puraKulkutieAktio_);
    asetinlaiteToolBar_->addSeparator();
    asetinlaiteToolBar_->addAction( junanumeroAktio_);
    asetinlaiteToolBar_->addSeparator();
    asetinlaiteToolBar_->addAction( kaannaVaihdeAktio_);
    asetinlaiteToolBar_->addAction( aukiajetunKaantoAktio_);
    asetinlaiteToolBar_->addSeparator();
    asetinlaiteToolBar_->addAction( seisAktio_);
    asetinlaiteToolBar_->addAction( ajaAktio_);


    kaskyLine_ = new QLineEdit;
    connect( kaskyLine_, SIGNAL(returnPressed()), this, SLOT(kasky()));
    hallintaToolBar_->addWidget(kaskyLine_);


}
