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
    luoAktiot();
    luoTyoPalkki();

    skene_ = new EditoriScene;
    view_ = new EditoriView( skene_, this);

    view_->scale(2.0, 2.0);


    setCentralWidget(view_);
    connect( view_, SIGNAL(editorinTilaVaihtunut(int)), this, SLOT(laitaTilaNappiAlas(int)));
    connect( view_, SIGNAL(kiskoValittu(EditoriKisko*)), this, SLOT(kiskoValittu(EditoriKisko*)));

    nakymaValinta_->setCurrentIndex( nakymaValinta_->findData(nakyma) );
    skene_->haeNakyma(nakyma);
    view_->valitseTila(EditoriView::Osoitin);
    kiskoValittu(0);
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
    {
        int nakymanId = nakymaValinta_->itemData( valintaind).toInt();
        skene_->haeNakyma( nakymanId );

        rataKiskonToolBar_->setVisible( nakymanId == 0);
        nakymaKiskonToolBar_->setVisible( nakymanId != 0);
    }
    kiskoValittu(0);


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

    viivainAktio_ = new QAction( tr("Mittaa viivaimella"), this);
    viivainAktio_ ->setIcon(QIcon(":/r/pic/viivain.png"));
    viivainAktio_ ->setCheckable(true);
    viivainAktio_ ->setData( EditoriView::Viivain);
    connect( viivainAktio_ , SIGNAL(triggered()), this, SLOT(vaihdaTilaActionilta()));


    // Laiturivalinnat
    valittuihinLiittyvat_ = new QActionGroup(this);
    valittuihinLiittyvat_->setExclusive(false);

    laituriVasenAktio_ = new QAction( tr("Laituri vasemmalla"), valittuihinLiittyvat_);
    laituriVasenAktio_ ->setIcon(QIcon(":/r/pic/laituri-vasen.png"));
    laituriVasenAktio_ ->setCheckable(true);
    connect( laituriVasenAktio_ , SIGNAL(triggered()), this, SLOT(paivitaKiskonMaareet()));

    laituriOikeaAktio_ = new QAction( tr("Laituri oikealla"), valittuihinLiittyvat_);
    laituriOikeaAktio_ ->setIcon(QIcon(":/r/pic/laituri-oikea.png"));
    laituriOikeaAktio_ ->setCheckable(true);
    connect( laituriOikeaAktio_ , SIGNAL(triggered()), this, SLOT(paivitaKiskonMaareet()));

    naytaJunanumerotAktio_ = new QAction( tr("Näytä junanumero"), valittuihinLiittyvat_);
    naytaJunanumerotAktio_->setIcon( QIcon(":/r/pic/nayta-junanumerot.png"));
    naytaJunanumerotAktio_ ->setCheckable(true);
    connect( naytaJunanumerotAktio_ , SIGNAL(triggered()), this, SLOT(paivitaKiskonMaareet()));

    naytaaRaidenumerotAktio_ = new QAction( tr("Näytä raiteen numero"), valittuihinLiittyvat_);
    naytaaRaidenumerotAktio_->setIcon( QIcon(":/r/pic/nayta-raidenumerot.png"));
    naytaaRaidenumerotAktio_ ->setCheckable(true);
    connect( naytaaRaidenumerotAktio_ , SIGNAL(triggered()), this, SLOT(paivitaKiskonMaareet()));


}

void EditoriIkkuna::laitaTilaNappiAlas(int valittuTila)
{
    osoitinAktio_->setChecked( valittuTila == EditoriView::Osoitin);
    piirraAktio_->setChecked( valittuTila == EditoriView::Piirto);
    pyyhiAktio_->setChecked( valittuTila == EditoriView::Pyyhi);
    tekstiAktio_->setChecked( valittuTila == EditoriView::Teksti);
    viivainAktio_->setChecked( valittuTila == EditoriView::Viivain);

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

        valittuihinLiittyvat_->setEnabled(true);
        raideLineEdit_->setEnabled(true);
        raideLineEdit_->setFocus();     // Helpottaa raidenumeron syöttämisen

        if( skene_->nakyma() )
        {
            // On kaukonäkymä
            nakymaKiskonToolBar_->setVisible(true);

            // Jn-valintoja...
            naytaJunanumerotAktio_->setChecked( kisko->naytaJunaNumero());
            naytaaRaidenumerotAktio_->setChecked(kisko->naytaRaideNumero());
        }
        else
        {
            rataKiskonToolBar_->setVisible(true);

            // Sn
            snCombo_->setCurrentIndex( snCombo_->findData( kisko->sn()) );
        }

        // Laiturin asetukset
        laituriVasenAktio_->setChecked( kisko->laituri() == Kisko::LaituriVasemmalla || kisko->laituri() == Kisko::LaituriMolemmat);
        laituriOikeaAktio_->setChecked( kisko->laituri() == Kisko::LaituriOikealla || kisko->laituri() == Kisko::LaituriMolemmat);



    }
    else
    {
        // Kiskoa ei ole valittu, liikennepaikka säilyy edellisen mukaan,
        // mutta raidenumeroa ei toistella...
        valittuihinLiittyvat_->setEnabled(false);

        raideLineEdit_->setText(QString());
        raideLineEdit_->setEnabled(false);
    }
}

void EditoriIkkuna::paivitaKiskonMaareet()
{
    EditoriKisko* kisko = view_->valittuKisko();
    if( !kisko )
        return;     // Ellei valittua niin oi voi

    Kisko::Laituri laiturityyppi = Kisko::LaituriEi;
    // Laiturin asettaminen
    if( laituriVasenAktio_->isChecked() && laituriOikeaAktio_->isChecked())
        laiturityyppi = Kisko::LaituriMolemmat;
    else if( laituriVasenAktio_->isChecked() )
        laiturityyppi = Kisko::LaituriVasemmalla;
    else if( laituriOikeaAktio_->isChecked())
        laiturityyppi = Kisko::LaituriOikealla;

    kisko->asetaRaiteenValintoja(laiturityyppi, naytaaRaidenumerotAktio_->isChecked(), naytaJunanumerotAktio_->isChecked(), nykyNopeusRajoitus());

    kisko->talletaKisko();
}


QString EditoriIkkuna::nykyLiikennePaikka() const
{
    return liikennepaikkaCombo_->itemData(liikennepaikkaCombo_->currentIndex() ).toString();
}

int EditoriIkkuna::nykyNopeusRajoitus() const
{
    if( skene_->nakyma())
        return 0;   // sn ei tallenneta näkymälle
    return snCombo_->itemData( snCombo_->currentIndex()  ).toInt();
}

void EditoriIkkuna::raideNumeroaMuutettu()
{
    if( view_->valittuKisko())
    {
        view_->valittuKisko()->asetaRaide( raideLineEdit_->text().toInt());
        view_->valittuKisko()->levitaRaiteenAsetus();
        kiskoValittu( view_->valittuKisko());   // Tällä päivitetään valinnat
            // - raiteen numero lineEditiin ja mahd. raide/jn-näyttämiset
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
    muokkausToolBar_->addAction(viivainAktio_);

    tunnisteToolBar_ = addToolBar( tr("Raide"));
    liikennepaikkaCombo_ = new QComboBox;
    haeLiikennepaikkaLista();
    connect( liikennepaikkaCombo_, SIGNAL(activated(int)), this, SLOT(liikennePaikkaValittu(int)));
    tunnisteToolBar_->addWidget(liikennepaikkaCombo_);

    raideLineEdit_ = new QLineEdit();
    raideLineEdit_->setValidator(new QIntValidator(0,9999,raideLineEdit_));
    connect( raideLineEdit_, SIGNAL(editingFinished()), this, SLOT(raideNumeroaMuutettu()));
    tunnisteToolBar_->addWidget(raideLineEdit_);

    rataKiskonToolBar_ = addToolBar( tr("Muokkaa rataa"));
    rataKiskonToolBar_->setVisible(false);
    rataKiskonToolBar_->addAction(laituriVasenAktio_);
    rataKiskonToolBar_->addAction(laituriOikeaAktio_);
    teeSnCombo();
    rataKiskonToolBar_->addWidget(snCombo_);

    nakymaKiskonToolBar_ = addToolBar( tr("Muokkaa näkymän kiskoa"));
    nakymaKiskonToolBar_->addAction(laituriVasenAktio_);
    nakymaKiskonToolBar_->addAction(laituriOikeaAktio_);
    nakymaKiskonToolBar_->addAction(naytaJunanumerotAktio_);
    nakymaKiskonToolBar_->addAction(naytaaRaidenumerotAktio_);
}

void EditoriIkkuna::teeSnCombo()
{
    // Nopeusrajoituksen valinta
    snCombo_ = new QComboBox;
    snCombo_->addItem("220 km/h", 220);
    snCombo_->addItem("200 km/h", 200);
    snCombo_->addItem("160 km/h", 160);
    snCombo_->addItem("120 km/h", 120);
    snCombo_->addItem("100 km/h", 100);
    snCombo_->addItem("80 km/h", 80);
    snCombo_->addItem("50 km/h", 50);
    snCombo_->addItem("35 km/h", 35);
    snCombo_->addItem("20 km/h", 20);
    snCombo_->addItem("10 km/h", 10);

    snCombo_->setCurrentIndex(5);
}
