/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "editoriikkuna.h"
#include "editorikisko.h"

#include "editoriraide.h"
#include "raiteenpaa.h"

#include "rataikkuna.h"

#include <QSqlQuery>
#include <QInputDialog>
#include <QIntValidator>

#include <QMessageBox>


EditoriIkkuna::EditoriIkkuna(int nakyma, QWidget *parent) :
    QMainWindow(parent)
{
    setWindowTitle("Editori");
    luoAktiot();
    luoTyoPalkki(nakyma == 0);

    skene_ = new EditoriScene;
    view_ = new EditoriView( skene_, this);

    view_->scale(2.0, 2.0);


    setCentralWidget(view_);
    connect( view_, SIGNAL(editorinTilaVaihtunut(int)), this, SLOT(laitaTilaNappiAlas(int)));
    connect( view_, SIGNAL(kiskoValittu(EditoriKisko*)), this, SLOT(kiskoValittu(EditoriKisko*)));
    connect( view_, SIGNAL(metriTeksti(QString)), metriLabel_, SLOT(setText(QString)));

    connect( naytaNopeusRajoitusAktio_, SIGNAL(toggled(bool)), skene_, SLOT(naytaNopeusRajoitus(bool)));

    if( nakyma < 0)
        nakyma = nakymaValinta_->itemData(0).toInt();

    if( nakyma )    // Rataa muokattaessa ei vaihtolaatikkoa ollenkaan...
        nakymaValinta_->setCurrentIndex( nakymaValinta_->findData(nakyma) );
    else
        nakymaValinta_->setVisible(false);

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

    vieritysAktio_ = new QAction( tr("Vieritä hiirellä vetämällä"), this);
    vieritysAktio_->setIcon(QIcon(":/r/pic/scroll.png"));
    vieritysAktio_->setCheckable(true);
    vieritysAktio_->setData( EditoriView::Vierita );
    connect( vieritysAktio_, SIGNAL(triggered()), this, SLOT(vaihdaTilaActionilta()));

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

    laituriOikeaAktio_ = new QAction( tr("Laituri oikealla"), valittuihinLiittyvat_);
    laituriOikeaAktio_ ->setIcon(QIcon(":/r/pic/laituri-oikea.png"));
    laituriOikeaAktio_ ->setCheckable(true);

    siltaAktio_ = new QAction( tr("Silta"), valittuihinLiittyvat_);
    siltaAktio_->setIcon( QIcon(":/r/pic/silta.png"));
    siltaAktio_->setCheckable(true);

    naytaJunanumerotAktio_ = new QAction( tr("Näytä junanumero"), valittuihinLiittyvat_);
    naytaJunanumerotAktio_->setIcon( QIcon(":/r/pic/nayta-junanumerot.png"));
    naytaJunanumerotAktio_ ->setCheckable(true);

    naytaaRaidenumerotAktio_ = new QAction( tr("Näytä raiteen numero"), valittuihinLiittyvat_);
    naytaaRaidenumerotAktio_->setIcon( QIcon(":/r/pic/nayta-raidenumerot.png"));
    naytaaRaidenumerotAktio_ ->setCheckable(true);
    connect( valittuihinLiittyvat_ , SIGNAL(triggered(QAction*)), this, SLOT(paivitaKiskonMaareet()));


    // Radan muokkausta
    naytaNopeusRajoitusAktio_ = new  QAction( tr("Raiteiden värit nopeusrajoituksen mukaan"), this);
    naytaNopeusRajoitusAktio_->setIcon( QIcon(":r/pic/nopeusrajoitus.png"));
    naytaNopeusRajoitusAktio_->setCheckable(true);

    kulkutietyypitAktiot_ = new QActionGroup(this);
    ensisijainenKulkutieAktio_ = new QAction( tr("Ensisijaisella junakulkutiellä"), kulkutietyypitAktiot_);
    ensisijainenKulkutieAktio_->setIcon( QIcon(":r/pic/po-aja.png"));
    ensisijainenKulkutieAktio_->setCheckable(true);

    toissijainenKulkutieAktio_ = new QAction( tr("Toissijaisella junakulkutiellä"), kulkutietyypitAktiot_ );
    toissijainenKulkutieAktio_->setIcon(QIcon(":r/pic/po-ajasn.png"));
    toissijainenKulkutieAktio_->setCheckable(true);

    vainVaihtotieAktio_ = new QAction( tr("Vain vaihtokulkutien käytössä"), kulkutietyypitAktiot_);
    vainVaihtotieAktio_->setIcon(QIcon(":/r/pic/ro-aja.png"));
    vainVaihtotieAktio_->setCheckable(true);

    connect( kulkutietyypitAktiot_, SIGNAL(triggered(QAction*)), this, SLOT(paivitaKiskonMaareet()));

    // Raiteen määreet - sähköistys, valvonta
    raiteenMaareAktiot_ = new QActionGroup(this);
    raiteenMaareAktiot_->setExclusive(false);

    sahkoistettyAktio_ = new QAction( tr("Sähköistetty raide"), raiteenMaareAktiot_);
    sahkoistettyAktio_->setIcon( QIcon(":/r/pic/sahko.png"));
    sahkoistettyAktio_->setCheckable(true);

    valvomatonAktio_ = new QAction( tr("Raiteella ei vapaanaolon valvontaa"), raiteenMaareAktiot_);
    valvomatonAktio_->setIcon(QIcon(":/r/pic/eivalvottu.png"));
    valvomatonAktio_->setCheckable(true);

    connect( raiteenMaareAktiot_, SIGNAL(triggered(QAction*)), this, SLOT(paivitaRaiteenMaareet()));

    // Opastimet

    eoEtelaanAktio_ = new QAction( tr("Esiopastin etelään"), valittuihinLiittyvat_);
    eoEtelaanAktio_->setIcon( QIcon(":/r/pic/eo-seis.png"));
    eoEtelaanAktio_ ->setCheckable(true);

    eoPohjoiseenAktio_ = new QAction( tr("Esiopastin pohjoiseen"), valittuihinLiittyvat_);
    eoPohjoiseenAktio_->setIcon( QIcon(":/r/pic/eo-seis.png"));
    eoPohjoiseenAktio_ ->setCheckable(true);


    opastinAktiot_ = new QActionGroup(this);
    opastinAktiot_->setExclusive(false);

    poEtelaanAktio_ = new QAction( tr("Pääopastin etelään"), opastinAktiot_);
    poEtelaanAktio_->setIcon(QIcon(":/r/pic/po-seis.png"));
    poEtelaanAktio_->setCheckable(true);

    poPohjoiseenAktio_ = new QAction( tr("Pääopastin pohjoiseen"), opastinAktiot_);
    poPohjoiseenAktio_->setIcon(QIcon(":/r/pic/po-seis.png"));
    poPohjoiseenAktio_->setCheckable(true);

    roEtelaanAktio_ = new QAction( tr("Raideopastin etelään"), opastinAktiot_);
    roEtelaanAktio_->setIcon(QIcon(":/r/pic/ro-seis.png"));
    roEtelaanAktio_->setCheckable(true);

    roPohjoiseenAktio_ = new QAction( tr("Raideopastin pohjoiseen"), opastinAktiot_);
    roPohjoiseenAktio_->setIcon(QIcon(":/r/pic/ro-seis.png"));
    roPohjoiseenAktio_->setCheckable(true);

    soEtelaanAktio_ = new QAction( tr("Suojastusopastin etelään"), opastinAktiot_);
    soEtelaanAktio_->setIcon(QIcon(":/r/pic/so-seis.png"));
    soEtelaanAktio_->setCheckable(true);

    soPohjoiseenAktio_ = new QAction( tr("Suojastusopastin pohjoiseen"), opastinAktiot_);
    soPohjoiseenAktio_->setIcon(QIcon(":/r/pic/so-seis.png"));
    soPohjoiseenAktio_->setCheckable(true);

    spEtelaanAktio_ = new QAction( tr("Raiteensulku etelään"), opastinAktiot_);
    spEtelaanAktio_->setIcon(QIcon(":/r/pic/raiteensulku.png"));
    spEtelaanAktio_->setCheckable(true);

    spPohjoiseenAktio_ = new QAction( tr("Raiteensulku pohjoiseen"), opastinAktiot_);
    spPohjoiseenAktio_->setIcon(QIcon(":/r/pic/raiteensulku-pohjoiseen.png"));
    spPohjoiseenAktio_->setCheckable(true);

    connect( opastinAktiot_, SIGNAL(triggered(QAction*)), this, SLOT(paivitaOpastimet()));


    // RaideRisteys slottaa opastinaktioihin, koska vaikutta raiteeseen
    raideRisteysAktio_ = new QAction( tr("Raideristeys"), opastinAktiot_);
    raideRisteysAktio_->setIcon( QIcon(":/r/pic/raideristeys.png"));
    raideRisteysAktio_->setCheckable(true);

    connect( raideRisteysAktio_, SIGNAL(triggered()), this, SLOT(paivitaOpastimet()));

}

void EditoriIkkuna::laitaTilaNappiAlas(int valittuTila)
{
    osoitinAktio_->setChecked( valittuTila == EditoriView::Osoitin);
    vieritysAktio_->setChecked( valittuTila==EditoriView::Vierita);
    piirraAktio_->setChecked( valittuTila == EditoriView::Piirto);
    pyyhiAktio_->setChecked( valittuTila == EditoriView::Pyyhi);
    tekstiAktio_->setChecked( valittuTila == EditoriView::Teksti);
    viivainAktio_->setChecked( valittuTila == EditoriView::Viivain);

}

void EditoriIkkuna::haeNakymaLista()
{
    // Luodaan näkymän valinta
    nakymaValinta_->clear();

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

            ensisijainenKulkutieAktio_->setChecked( kisko->kulkutietyypit() == EditoriKisko::Ensisijainen );
            toissijainenKulkutieAktio_->setChecked(kisko->kulkutietyypit() == EditoriKisko::Toissijainen);
            vainVaihtotieAktio_->setChecked(kisko->kulkutietyypit() == EditoriKisko::VainVaihto);

            eoEtelaanAktio_->setChecked( kisko->esiopastinEtela());
            eoPohjoiseenAktio_->setChecked( kisko->esiopastinPohjoinen());

            EditoriRaide* rp = kisko->raidePointteri();
            if( rp )
            {

                sahkoistettyAktio_->setChecked( rp->sahkoistetty());
                valvomatonAktio_->setChecked( !rp->valvottu());

                opastinAktiot_->setEnabled(true);
                poEtelaanAktio_->setChecked( rp->etelainen()->paaOpastin() );
                poPohjoiseenAktio_->setChecked( rp->pohjoinen()->paaOpastin());
                roEtelaanAktio_->setChecked( rp->etelainen()->raideOpastin());
                roPohjoiseenAktio_->setChecked( rp->pohjoinen()->raideOpastin());
                soEtelaanAktio_->setChecked( rp->etelainen()->suojastusOpastin());
                soPohjoiseenAktio_->setChecked( rp->pohjoinen()->suojastusOpastin());
                raideRisteysAktio_->setChecked( rp->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys);
                spEtelaanAktio_->setChecked( rp->etelainen()->raiteenSulku());
                spPohjoiseenAktio_->setChecked( rp->pohjoinen()->raiteenSulku());

                opastintenHimmennys(rp);

                // Jos molemmilla puolilla vaihde tai RR, niin mahdollista tehdä RR
                if( (rp->etelainen()->paanTyyppi() == RaiteenPaa::Vaihde  ||
                     rp->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys) &&
                        (rp->pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde ||
                         rp->pohjoinen()->paanTyyppi() == RaiteenPaa::RaideRisteys) )
                    raideRisteysAktio_->setEnabled(true);
                else
                    raideRisteysAktio_->setEnabled(false);


            }
            else
            {
                opastinAktiot_->setEnabled(false);
            }
        }

        // Laiturin asetukset
        laituriVasenAktio_->setChecked( kisko->laituri() == Kisko::LaituriVasemmalla || kisko->laituri() == Kisko::LaituriMolemmat);
        laituriOikeaAktio_->setChecked( kisko->laituri() == Kisko::LaituriOikealla || kisko->laituri() == Kisko::LaituriMolemmat);
        siltaAktio_->setChecked( kisko->silta());

        metriLabel_->setText( QString("%1 m").arg( qRound(kisko->pituus())));

    }
    else
    {
        // Kiskoa ei ole valittu, liikennepaikka säilyy edellisen mukaan,
        // mutta raidenumeroa ei toistella...
        valittuihinLiittyvat_->setEnabled(false);

        raideLineEdit_->setText(QString());
        raideLineEdit_->setEnabled(false);
        metriLabel_->setText(QString());
        opastinAktiot_->setEnabled(false);
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

    EditoriKisko::Kulkutietyypit kulkutietyypit = EditoriKisko::Ensisijainen;
    if(  toissijainenKulkutieAktio_->isChecked())
        kulkutietyypit = EditoriKisko::Toissijainen;
    else if( vainVaihtotieAktio_->isChecked())
        kulkutietyypit = EditoriKisko::VainVaihto;

    kisko->asetaRaiteenValintoja(laiturityyppi, naytaaRaidenumerotAktio_->isChecked(), naytaJunanumerotAktio_->isChecked(), nykyNopeusRajoitus(), kulkutietyypit,
                                 eoEtelaanAktio_->isChecked(), eoPohjoiseenAktio_->isChecked(), siltaAktio_->isChecked());

    kisko->talletaKisko();
}

void EditoriIkkuna::paivitaRaiteenMaareet()
{
    EditoriKisko* kisko = view_->valittuKisko();
    if( !kisko || !kisko->raidePointteri())
        return;

    kisko->raidePointteri()->asetaMaareet( sahkoistettyAktio_->isChecked(), !valvomatonAktio_->isChecked() );
    skene_->invalidate( skene_->sceneRect());
}

void EditoriIkkuna::paivitaOpastimet()
{
    EditoriKisko* kisko = view_->valittuKisko();
    if( !kisko || !kisko->raidePointteri())
        return;

    if( poEtelaanAktio_->isChecked())
        kisko->raidePointteri()->etelainen()->asetaOpastin(RaiteenPaa::PaaOpastin);
    else if( soEtelaanAktio_->isChecked())
        kisko->raidePointteri()->etelainen()->asetaOpastin(RaiteenPaa::SuojastusOpastin);
    else if( roEtelaanAktio_->isChecked())
        kisko->raidePointteri()->etelainen()->asetaOpastin(RaiteenPaa::RaideOpastin);
    else
        kisko->raidePointteri()->etelainen()->asetaOpastin(RaiteenPaa::EiOpastinta);

    if( poPohjoiseenAktio_->isChecked())
        kisko->raidePointteri()->pohjoinen()->asetaOpastin(RaiteenPaa::PaaOpastin);
    else if( soPohjoiseenAktio_->isChecked())
        kisko->raidePointteri()->pohjoinen()->asetaOpastin(RaiteenPaa::SuojastusOpastin);
    else if( roPohjoiseenAktio_->isChecked())
        kisko->raidePointteri()->pohjoinen()->asetaOpastin(RaiteenPaa::RaideOpastin);
    else
        kisko->raidePointteri()->pohjoinen()->asetaOpastin(RaiteenPaa::EiOpastinta);

    if( (kisko->raidePointteri()->etelainen()->paanTyyppi() == RaiteenPaa::Vaihde  ||
         kisko->raidePointteri()->etelainen()->paanTyyppi() == RaiteenPaa::RaideRisteys) &&
            (kisko->raidePointteri()->pohjoinen()->paanTyyppi() == RaiteenPaa::Vaihde ||
             kisko->raidePointteri()->pohjoinen()->paanTyyppi() == RaiteenPaa::RaideRisteys) )
    {
            // Voidaan vaihtaa raideristeyksen ja vaihteen väliltä
        if( raideRisteysAktio_->isChecked())
        {
            kisko->raidePointteri()->etelainen()->asetaPaanTyyppi(RaiteenPaa::RaideRisteys);
            kisko->raidePointteri()->pohjoinen()->asetaPaanTyyppi(RaiteenPaa::RaideRisteys);
        }
        else
        {
            kisko->raidePointteri()->etelainen()->asetaPaanTyyppi(RaiteenPaa::Vaihde);
            kisko->raidePointteri()->pohjoinen()->asetaPaanTyyppi(RaiteenPaa::Vaihde);
        }

    }

    kisko->raidePointteri()->etelainen()->asetaRaiteenSulku( spEtelaanAktio_->isChecked());
    kisko->raidePointteri()->pohjoinen()->asetaRaiteenSulku( spPohjoiseenAktio_->isChecked());

    kisko->raidePointteri()->talleta();
    skene_->invalidate( skene_->sceneRect());

    // Sittenpä vielä tarkastetaan nappien vaihtoehtoisuudet..
    opastintenHimmennys( kisko->raidePointteri());
}

void EditoriIkkuna::opastintenHimmennys(EditoriRaide *raidePointteri)
{
    poEtelaanAktio_->setEnabled( !raidePointteri->etelainen()->suojastusOpastin() && !raidePointteri->etelainen()->raideOpastin());
    poPohjoiseenAktio_->setEnabled( !raidePointteri->pohjoinen()->suojastusOpastin() && !raidePointteri->pohjoinen()->raideOpastin());

    roEtelaanAktio_->setEnabled( !raidePointteri->etelainen()->suojastusOpastin() && !raidePointteri->etelainen()->paaOpastin());
    roPohjoiseenAktio_->setEnabled( !raidePointteri->pohjoinen()->suojastusOpastin() && !raidePointteri->pohjoinen()->paaOpastin());

    soEtelaanAktio_->setEnabled( !( raidePointteri->etelainen()->paaOpastin() || raidePointteri->etelainen()->raideOpastin() ));
    soPohjoiseenAktio_->setEnabled( !( raidePointteri->pohjoinen()->paaOpastin() || raidePointteri->pohjoinen()->raideOpastin() ));

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





void EditoriIkkuna::luoTyoPalkki(bool ratanakyma)
{
    nakymaValinta_ = new QComboBox;
    if( !ratanakyma )
    {
        hallintaToolBar_ = addToolBar( tr("Hallinta"));
        haeNakymaLista();
        connect( nakymaValinta_, SIGNAL(activated(int)), this, SLOT(nakymanVaihto(int)));
        hallintaToolBar_->addWidget(nakymaValinta_);
    }

    muokkausToolBar_ = addToolBar( tr("Muokkaus"));
    muokkausToolBar_->addAction(osoitinAktio_);
    muokkausToolBar_->addAction(vieritysAktio_);
    muokkausToolBar_->addAction(piirraAktio_);
    muokkausToolBar_->addAction(pyyhiAktio_);
    muokkausToolBar_->addAction(tekstiAktio_);
    muokkausToolBar_->addAction(viivainAktio_);
    metriLabel_ = new QLabel("        ");
    muokkausToolBar_->addWidget(metriLabel_);

    tunnisteToolBar_ = addToolBar( tr("Raide"));
    liikennepaikkaCombo_ = new QComboBox;
    haeLiikennepaikkaLista();
    connect( liikennepaikkaCombo_, SIGNAL(activated(int)), this, SLOT(liikennePaikkaValittu(int)));
    tunnisteToolBar_->addWidget(liikennepaikkaCombo_);

    raideLineEdit_ = new QLineEdit();
    raideLineEdit_->setValidator(new QIntValidator(0,9999,raideLineEdit_));
    connect( raideLineEdit_, SIGNAL(editingFinished()), this, SLOT(raideNumeroaMuutettu()));
    tunnisteToolBar_->addWidget(raideLineEdit_);

    if( ratanakyma )
    {
        rataKiskonToolBar_ = addToolBar( tr("Muokkaa rataa"));
        rataKiskonToolBar_->addAction(spEtelaanAktio_);
        rataKiskonToolBar_->addAction(poEtelaanAktio_);
        rataKiskonToolBar_->addAction(roEtelaanAktio_);
        rataKiskonToolBar_->addAction(soEtelaanAktio_);
        rataKiskonToolBar_->addSeparator();

        rataKiskonToolBar_->addAction(eoEtelaanAktio_);
        rataKiskonToolBar_->addSeparator();
        rataKiskonToolBar_->addAction(laituriVasenAktio_);
        rataKiskonToolBar_->addAction(laituriOikeaAktio_);
        rataKiskonToolBar_->addAction(siltaAktio_);
        rataKiskonToolBar_->addAction(sahkoistettyAktio_);
        rataKiskonToolBar_->addAction(valvomatonAktio_);
        rataKiskonToolBar_->addAction(naytaNopeusRajoitusAktio_);
        teeSnCombo();
        rataKiskonToolBar_->addWidget(snCombo_);
        rataKiskonToolBar_->addAction(raideRisteysAktio_);
        rataKiskonToolBar_->addAction(ensisijainenKulkutieAktio_);
        rataKiskonToolBar_->addAction(toissijainenKulkutieAktio_);
        rataKiskonToolBar_->addAction(vainVaihtotieAktio_);
        rataKiskonToolBar_->addSeparator();
        rataKiskonToolBar_->addAction(eoPohjoiseenAktio_);
        rataKiskonToolBar_->addSeparator();
        rataKiskonToolBar_->addAction(soPohjoiseenAktio_);
        rataKiskonToolBar_->addAction(roPohjoiseenAktio_);
        rataKiskonToolBar_->addAction(poPohjoiseenAktio_);
        rataKiskonToolBar_->addAction(spPohjoiseenAktio_);
    }
    else
    {
        nakymaKiskonToolBar_ = addToolBar( tr("Muokkaa näkymän kiskoa"));
        nakymaKiskonToolBar_->addAction(laituriVasenAktio_);
        nakymaKiskonToolBar_->addAction(laituriOikeaAktio_);
        nakymaKiskonToolBar_->addAction(siltaAktio_);
        nakymaKiskonToolBar_->addAction(naytaJunanumerotAktio_);
        nakymaKiskonToolBar_->addAction(naytaaRaidenumerotAktio_);
    }


}

void EditoriIkkuna::teeSnCombo()
{

    QList<int> nopeudet;
    nopeudet << 240 << 200 << 160 << 140 << 120 << 100 << 80
                << 50 << 35 << 20 << 10;

    snCombo_ = new QComboBox;
    foreach(int nopeus, nopeudet)
    {
        QPixmap pmap(16,16);
        pmap.fill(EditoriKisko::nopeusVari(nopeus));

        snCombo_->addItem(QIcon(pmap), QString("%1 km/h").arg(nopeus), nopeus);
    }
    snCombo_->setCurrentIndex(5); // Oletus 80 km/h
    connect( snCombo_, SIGNAL(activated(int)), this, SLOT(paivitaKiskonMaareet()));
}
