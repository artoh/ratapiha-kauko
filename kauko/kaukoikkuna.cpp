
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

    junakulkutieAktio_ = new QAction( this );
    junakulkutieAktio_->setData(KaukoView::JUNAKULKUTIE_ALKAA);
    junakulkutieAktio_->setIcon(QIcon(":/kauko/pic/vihrea.png"));
    junakulkutieAktio_->setCheckable(true);
    aslAktiot_->addAction(junakulkutieAktio_);

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

