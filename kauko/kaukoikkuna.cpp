
#include <QAction>

#include "kaukoikkuna.h"

KaukoIkkuna::KaukoIkkuna(QWidget *parent) : QMainWindow(parent)
{

    scene_ = new KaukoScene;
    view_ = new KaukoView(scene_);

    luoAktiot();
    luoTyokalupalkit();

    setCentralWidget(view_);

    connect(scene_, SIGNAL(nakymaListaan(int,QString)), this, SLOT(lisaaNakymaListaan(int,QString)));

    scene_->yhdistaAsetinlaitteeseen();

    setGeometry(10,10,800,400);
}

KaukoIkkuna::~KaukoIkkuna()
{

}

void KaukoIkkuna::lisaaNakymaListaan(int id, const QString &nimi)
{
    nakymaCombo_->addItem( nimi, id);
}

void KaukoIkkuna::nakymanVaihto(int valintaIndeksi)
{
    int nakymaId = nakymaCombo_->itemData(valintaIndeksi).toInt();
    scene_->vaihdaNakyma(nakymaId);
    setWindowTitle(tr("Ratapiha5 - %1").arg( nakymaCombo_->currentText() ));
}

void KaukoIkkuna::luoAktiot()
{
    aslAktiot_ = new QActionGroup(this);
    aslAktiot_->setExclusive(false);

    QAction *vieritaAktio = new QAction( this );
    vieritaAktio->setData(KaukoView::VIERITA);
    vieritaAktio->setIcon(QIcon(":/kauko/pic/scroll.png"));
    aslAktiot_->addAction(vieritaAktio);


    QAction *junakulkutieAktio = new QAction( this );
    junakulkutieAktio->setData(KaukoView::JUNAKULKUTIE_ALKAA);
    junakulkutieAktio->setIcon(QIcon(":/kauko/pic/vihrea.png"));
    aslAktiot_->addAction(junakulkutieAktio);

    QAction *kperAktio = new QAction( this );
    kperAktio->setData(KaukoView::KULKUTIENPERUMINEN);
    kperAktio->setIcon(QIcon(":/kauko/pic/kulkutienpurku.png"));
    aslAktiot_->addAction(kperAktio);

    QAction *hpAktio = new QAction( this );
    hpAktio->setData(KaukoView::HATAKULKUTIENPURKU);
    hpAktio->setIcon(QIcon(":/kauko/pic/hatakulkutienpurku.png"));
    aslAktiot_->addAction(hpAktio);

    connect( aslAktiot_, SIGNAL(triggered(QAction*)), this, SLOT(vaihdaTila(QAction*)));

}

void KaukoIkkuna::luoTyokalupalkit()
{
    hallintaToolBar_ = addToolBar( tr("Näkymä"));
    nakymaCombo_ = new QComboBox;
    hallintaToolBar_->addWidget(nakymaCombo_);

    connect(nakymaCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(nakymanVaihto(int)));

    aslToolBar_ = addToolBar( tr("Asetinlaitekomennot"));
    aslToolBar_->addActions( aslAktiot_->actions() );
}

void KaukoIkkuna::vaihdaTila(QAction *action)
{
    view_->valitseTila( action->data().toInt() );
}

