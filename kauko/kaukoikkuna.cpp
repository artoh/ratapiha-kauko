#include "kaukoikkuna.h"

KaukoIkkuna::KaukoIkkuna(QWidget *parent) : QMainWindow(parent)
{

    scene_ = new KaukoScene;
    view_ = new KaukoView(scene_);

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

void KaukoIkkuna::luoTyokalupalkit()
{
    hallintaToolBar_ = addToolBar( tr("Näkymä"));
    nakymaCombo_ = new QComboBox;
    hallintaToolBar_->addWidget(nakymaCombo_);

    connect(nakymaCombo_, SIGNAL(currentIndexChanged(int)), this, SLOT(nakymanVaihto(int)));
}

