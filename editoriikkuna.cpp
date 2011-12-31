/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editoriikkuna.h"
#include "editorikisko.h"
#include <QSqlQuery>
#include <QInputDialog>
#include <QIntValidator>


EditoriIkkuna::EditoriIkkuna(int nakyma, QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Editori");

    skene_ = new EditoriScene;
    view_ = new EditoriView( skene_, this);

    view_->scale(2.0, 2.0);

    luoAktiot();
    luoTyoPalkki();

    setCentralWidget(view_);
    connect( view_, SIGNAL(editorinTilaVaihtunut(int)), this, SLOT(laitaTilaNappiAlas(int)));
    connect( view_, SIGNAL(kiskoValittu(EditoriKisko*)), this, SLOT(kiskoValittu(EditoriKisko*)));

    nakymaValinta_->setCurrentIndex( nakymaValinta_->findData(nakyma) );
    skene_->haeNakyma(nakyma);
    view_->valitseTila(EditoriView::Osoitin);
}


void EditoriIkkuna::nakymanVaihto(int valintaind)
{
    view_->poistaValinta();
    if( nakymaValinta_->itemData(valintaind) == -1)
    {
        QString uudenNakymanNimi = QInputDialog::getText(this, tr("Editori"),tr("Uuden näkymän nimi"));
        if( !uudenNakymanNimi.isEmpty())
        {
            skene_->uusiNakyma(uudenNakymanNimi);
            haeNakymaLista();
        }
        nakymaValinta_->setCurrentIndex( nakymaValinta_->findData( skene_->nakyma()) );
    }

    else
        skene_->haeNakyma( nakymaValinta_->itemData( valintaind).toInt());


}

void EditoriIkkuna::vaihdaTilaActionilta()
{
    QObject* aktioObj = sender();
    QAction* aktio = qobject_cast<QAction*>(aktioObj);
    if( aktio )
        view_->valitseTila(aktio->data().toInt());
}


void EditoriIkkuna::luoAktiot()
{

    osoitinAktio_ = new QAction( tr("Piirra"), this);
    osoitinAktio_->setIcon(QIcon(":/r/pic/valintanuoli.png"));
    osoitinAktio_->setCheckable(true);
    osoitinAktio_->setData( EditoriView::Osoitin );
    connect( osoitinAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTilaActionilta()));

    piirraAktio_ = new QAction( tr("Piirra"), this);
    piirraAktio_->setIcon(QIcon(":/r/pic/kyna.png"));
    piirraAktio_->setCheckable(true);
    piirraAktio_->setData( EditoriView::Piirto );
    connect( piirraAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTilaActionilta()));

    pyyhiAktio_ = new QAction( tr("Pyyhi"), this);
    pyyhiAktio_->setIcon(QIcon(":/r/pic/pyyhekumi.png"));
    pyyhiAktio_->setCheckable(true);
    pyyhiAktio_->setData( EditoriView::Pyyhi );
    connect( pyyhiAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTilaActionilta()));

    tekstiAktio_ = new QAction( tr("Lisää tekstiä"), this);
    tekstiAktio_->setIcon(QIcon(":/r/pic/teksti.png"));
    tekstiAktio_->setCheckable(true);
    tekstiAktio_->setData( EditoriView::Teksti);
    connect( tekstiAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTilaActionilta()));

}

void EditoriIkkuna::laitaTilaNappiAlas(int valittuTila)
{
    osoitinAktio_->setChecked( valittuTila == EditoriView::Osoitin);
    piirraAktio_->setChecked( valittuTila == EditoriView::Piirto);
    pyyhiAktio_->setChecked( valittuTila == EditoriView::Pyyhi);
    tekstiAktio_->setChecked( valittuTila == EditoriView::Teksti);

}

void EditoriIkkuna::haeNakymaLista()
{
    // Luodaan näkymän valinta
    nakymaValinta_->clear();
    nakymaValinta_->addItem("RATA",0);

    QSqlQuery nakymaKysely("select nakyma,nakymanimi from nakyma");
    while( nakymaKysely.next())
    {
        // Viedään näkymätiedot valintalaatikkoon
        nakymaValinta_->addItem( nakymaKysely.value(1).toString(), nakymaKysely.value(0).toInt());
    }
    nakymaValinta_->addItem("UUSI NÄKYMÄ", -1);
}

void EditoriIkkuna::haeLiikennepaikkaLista()
{
    QSqlQuery liikennePaikkaKysely("select liikennepaikka,nimi from liikennepaikka order by nimi");
    while( liikennePaikkaKysely.next())
    {
        // Viedään näkymätiedot valintalaatikkoon
        liikennepaikkaCombo_->addItem( liikennePaikkaKysely.value(1).toString(), liikennePaikkaKysely.value(0).toString());
    }
}

void EditoriIkkuna::liikennePaikkaValittu(int index)
{
    if( view_->valittuKisko())
    {
        view_->valittuKisko()->asetaLiikennepaikka(liikennepaikkaCombo_->itemData(index).toString()  );
        view_->valittuKisko()->levitaRaiteenAsetus();
    }
}


void EditoriIkkuna::kiskoValittu(EditoriKisko *kisko)
{
    if( kisko )
    {

        if( liikennepaikkaCombo_->findData( kisko->liikennePaikka()) > -1 )
            liikennepaikkaCombo_->setCurrentIndex( liikennepaikkaCombo_->findData( kisko->liikennePaikka() )  );

        if( kisko->raide())
            raideLineEdit_->setText(QString("%1").arg(kisko->raide(),3,10,QChar('0')) );
        else
            raideLineEdit_->setText(QString());

        raideLineEdit_->setFocus();     // Helpottaa raidenumeron syöttämisen
    }
    else
    {
        // Kiskoa ei ole valittu, liikennepaikka säilyy edellisen mukaan,
        // mutta raidenumeroa ei toistella...
        raideLineEdit_->setText(QString());
    }
}


QString EditoriIkkuna::nykyLiikennePaikka() const
{
    return liikennepaikkaCombo_->itemData(liikennepaikkaCombo_->currentIndex() ).toString();
}

void EditoriIkkuna::raideNumeroaMuutettu()
{
    if( view_->valittuKisko())
    {
        view_->valittuKisko()->asetaRaide( raideLineEdit_->text().toInt());
        view_->valittuKisko()->levitaRaiteenAsetus();
        raideLineEdit_->setText(QString("%1").arg(view_->valittuKisko()->raide(),3,10,QChar('0')) );
    }
}



void EditoriIkkuna::luoTyoPalkki()
{
    hallintaToolBar_ = addToolBar( tr("Hallinta"));
    nakymaValinta_ = new QComboBox;
    haeNakymaLista();
    connect( nakymaValinta_, SIGNAL(activated(int)), this, SLOT(nakymanVaihto(int)));
    hallintaToolBar_->addWidget(nakymaValinta_);

    muokkausToolBar_ = addToolBar( tr("Muokkaus"));
    muokkausToolBar_->addAction(osoitinAktio_);
    muokkausToolBar_->addAction(piirraAktio_);
    muokkausToolBar_->addAction(pyyhiAktio_);
    muokkausToolBar_->addAction(tekstiAktio_);

    tunnisteToolBar_ = addToolBar( tr("Raide"));
    liikennepaikkaCombo_ = new QComboBox;
    haeLiikennepaikkaLista();
    connect( liikennepaikkaCombo_, SIGNAL(activated(int)), this, SLOT(liikennePaikkaValittu(int)));
    tunnisteToolBar_->addWidget(liikennepaikkaCombo_);

    raideLineEdit_ = new QLineEdit();
  //  raideLineEdit_->setInputMask("0000");
    raideLineEdit_->setValidator(new QIntValidator(0,9999,raideLineEdit_));
    connect( raideLineEdit_, SIGNAL(editingFinished()), this, SLOT(raideNumeroaMuutettu()));
    tunnisteToolBar_->addWidget(raideLineEdit_);


}
