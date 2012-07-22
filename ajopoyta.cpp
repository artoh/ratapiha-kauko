/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 25.6.2011
**************************************************************************/

#include "ajopoyta.h"
#include "ui_ajopoyta.h"

#include "rataikkuna.h"
#include "ratascene.h"

#include <QMouseEvent>
#include <QIntValidator>

Ajopoyta::Ajopoyta(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Ajopoyta),
    veturi_(0)
{
    ui->setupUi(this);

    ui->veturiValinta->setValidator(new QIntValidator(1,9999,this));

    connect( ui->veturiValinta, SIGNAL( editingFinished() ), this, SLOT( valitseVeturi()));

    connect( ui->ajopoyta1, SIGNAL(toggled(bool)), this, SLOT(ajopoytaYksi(bool)));
    connect( ui->ajopoyta2, SIGNAL(toggled(bool)), this, SLOT(ajopoytaKaksi(bool)));
    connect(  ui->nopeusSaadin, SIGNAL(valueChanged(int)), this, SLOT(asetaTavoiteNopeus(int)));
    connect( ui->stopNappi, SIGNAL(clicked()), this, SLOT(hataJarru()));

    connect( ui->etsi, SIGNAL(clicked()), this, SLOT(keskita()));

    // Asettaa event filterin kosketusnäyttöominaisuutta varten
    ui->jkvTieto->installEventFilter(this);

    timer_ = new QTimer(this);
    connect( timer_, SIGNAL(timeout()), this, SLOT(paivitaJKVnaytto()));
    // Aloittaa jkv-näytön päivittämisen 0.5 sekunnin välein
    timer_->start(500);


}

Ajopoyta::~Ajopoyta()
{
    delete ui;
}



void Ajopoyta::valitseVeturi()
{

    valitseVeturi( ui->veturiValinta->text().toInt() );

    /*
    Vaunu* uusivaunu= RaideElementti::rata()->vaunu(veturi);
    Veturi* uusiveturi = dynamic_cast<Veturi*>(uusivaunu);
    if( veturi_ && uusiveturi )
    {
        // Puretaan edelliset säädöt
        disconnect( ui->nopeusSaadin, SIGNAL(valueChanged(int)), veturi_, SLOT(asetaTavoiteNopeus(int)));
        disconnect( veturi_, SIGNAL(nopeusIlmoitus(int)), this, SLOT(naytaNopeus(int)));
        disconnect( veturi_, SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));
        disconnect( ui->stopNappi, SIGNAL(clicked()), veturi_, SLOT(hataJarru()));
        disconnect( veturi_, SIGNAL(hataJarrutus()), this, SLOT(hataJarrutettu()));
        disconnect( ui->automaatioNappi, SIGNAL(clicked(bool)), veturi_, SLOT(asetaAikatauluAutomaatio(bool)));
        disconnect( veturi_, SIGNAL(itseAsetus(int,int,QPixmap)), this, SLOT(tilaMuutos(int,int,QPixmap)));

        // Irroitetaan jkv-laite näytöstä
        if( veturi_->jkvlaite())
            disconnect( veturi_->jkvlaite(), SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));

    }
    if( uusiveturi)
    {
        ui->jkvTieto->clear();

        veturi_ = uusiveturi;
        // Alustetaan veturin tiedot
        ui->ajopoyta1->setChecked( veturi_->ajopoyta() == 1);
        ui->ajopoyta2->setChecked(veturi_->ajopoyta() == 2);
        ui->automaatioNappi->setChecked( veturi_->aikatauluautomaatio());

        ui->maxNopeusLabel->setText( QString::number( veturi_->enimmaisNopeus()) );
        ui->nopeusSaadin->setRange(0, veturi_->enimmaisNopeus());
        ui->nopeusSaadin->setValue( veturi_->tavoitenopeus());
        // Näyttää paitsi nopeuden, myös ohjaa ajopöytänappien 1 ja 2 toimintaa
        naytaNopeus( veturi_->nopeus());


        connect( ui->nopeusSaadin, SIGNAL(valueChanged(int)), veturi_, SLOT(asetaTavoiteNopeus(int)));
        connect( veturi_, SIGNAL(nopeusIlmoitus(int)), this, SLOT(naytaNopeus(int)));
        connect( veturi_, SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));
        connect( ui->stopNappi, SIGNAL(clicked()), this, SLOT(hataJarru()));
        connect( veturi_, SIGNAL(hataJarrutus()), this, SLOT(hataJarrutettu()));
        connect( ui->automaatioNappi, SIGNAL(clicked(bool)), veturi_, SLOT(asetaAikatauluAutomaatio(bool)));
        connect( veturi_, SIGNAL(itseAsetus(int,int,QPixmap)), this, SLOT(tilaMuutos(int,int,QPixmap)));

        // Kytketään jkv-laite näyttöön
        if( veturi_->jkvlaite())
        {
            connect( veturi_->jkvlaite(), SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));
            ui->jkvTieto->setPixmap( veturi_->jkvlaite()->jkvKuva()  );
        }

    }
    else
    {

    } */
}

void Ajopoyta::valitseVeturi(int veturinNumero)
{
    Vaunu* uusivaunu = RataIkkuna::rataSkene()->haeVaunu(veturinNumero);
    Veturi* veturi = 0;
    if( uusivaunu)
        veturi = dynamic_cast<Veturi*>(uusivaunu);

    // Jos on kelpo veturi, niin vaihdetaan, muuten ei!
    if( veturi)
    {
        veturi_ = veturi;
        ui->veturinTyyppi->setText( veturi->vaununTyyppi());
        ui->nopeusSaadin->setValue( veturi->tavoiteNopeus());

        ui->ajopoyta1->setChecked( veturi->ajopoyta() == 1);
        ui->ajopoyta2->setChecked(veturi->ajopoyta() == 2);

        ui->ajopoyta1->setVisible( veturi->ajopoydat() == Vaunu::AjopoytaYksi || veturi->ajopoydat() == Vaunu::MolemmatAjopoydat);
        ui->ajopoyta2->setVisible( veturi->ajopoydat() == Vaunu::AjopoytaKaksi || veturi->ajopoydat() == Vaunu::MolemmatAjopoydat);

        ui->nopeusSaadin->setEnabled( veturi->ajopoyta() );

        naytaNopeus( veturi_->nopeus());
    }
    else
    {
        // Varmistetaan veturinnumeron näyttö
        if( veturi_)
            ui->veturiValinta->setText( QString::number(veturi_->vaunuNumero()) );
        else
            ui->veturiValinta->clear();
    }



}

void Ajopoyta::keskita()
{
    // Pyytää keskittämään tähän veturiin
    if( veturi_)
        emit keskitysPyynto(veturi_);
}

void Ajopoyta::naytaJkvIlmoitus( QPixmap jkvkuva)
{
    ui->jkvTieto->setPixmap( jkvkuva);
}

void Ajopoyta::hataJarru()
{
    ui->nopeusSaadin->setValue(0);

}


void Ajopoyta::ajopoytaYksi(bool onko)
{
    // Irroitetaan vanha jkv-laitteen näyttökytkentä
    // if( veturi_ && veturi_->jkvlaite())
    //    disconnect( veturi_->jkvlaite(), SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));
    if( onko )
    {
        ui->ajopoyta2->setChecked(false);
        if( veturi_)
        {
            veturi_->asetaAjoPoyta(1);
            ui->nopeusSaadin->setEnabled(true);
        }
    }
    else
    {
        if( veturi_)
            veturi_->asetaAjoPoyta(0);
        ui->jkvTieto->clear();
        ui->nopeusSaadin->setEnabled(false);
    }
    // Yhdistetään jkv-laite näyttöön
    // if( veturi_ && veturi_->jkvlaite())
    //    connect( veturi_->jkvlaite(), SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));

}

void Ajopoyta::ajopoytaKaksi(bool onko)
{
    // if( veturi_ && veturi_->jkvlaite())
    //    disconnect( veturi_->jkvlaite(), SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));

    if( onko )
    {
        ui->ajopoyta1->setChecked(false);
        if( veturi_)
        {
            veturi_->asetaAjoPoyta(2);
            ui->nopeusSaadin->setEnabled(true);
        }
    }
    else
    {
        if( veturi_)
        {
            veturi_->asetaAjoPoyta(0);
            ui->nopeusSaadin->setEnabled(false);
        }
            ui->jkvTieto->clear();
        ui->nopeusSaadin->setEnabled(false);
    }
    // if( veturi_ && veturi_->jkvlaite())
    //    connect( veturi_->jkvlaite(), SIGNAL(jkvIlmoitus(QPixmap)), this, SLOT(naytaJkvIlmoitus(QPixmap)));

}

void Ajopoyta::tilaMuutos(int ajopoyta, int tavoitenopeus, const QPixmap& kuva)
{
    ui->ajopoyta1->setChecked( ajopoyta == 1);
    ui->ajopoyta2->setChecked( ajopoyta == 2);
    ajopoytaYksi(ajopoyta == 1);
    ajopoytaKaksi(ajopoyta == 2);
    ui->nopeusSaadin->setValue(tavoitenopeus);
    if( !kuva.isNull() )
        ui->jkvTieto->setPixmap(kuva);
}

void Ajopoyta::asetaTavoiteNopeus(int nopeus)
{
    if( veturi_)
        veturi_->asetaTavoiteNopeus(nopeus);
}

void Ajopoyta::naytaNopeus(int nopeus)
{
    if(veturi_)
    {
        ui->nopeusLabel->setText( QString::number(nopeus ));
        ui->ajopoyta1->setEnabled( !nopeus && !ui->nopeusSaadin->value());
        ui->ajopoyta2->setEnabled( !nopeus && !ui->nopeusSaadin->value());
    }
}

bool Ajopoyta::eventFilter(QObject *target, QEvent *event)
{
    if( target == ui->jkvTieto && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *hiiritapaus = static_cast<QMouseEvent*>(event);
//        if( veturi_ && veturi_->jkvlaite())
//            veturi_->jkvlaite()->nayttoonKoskettu( hiiritapaus->pos());
//        return true;
    }
    return QWidget::eventFilter(target,event);
}


void Ajopoyta::paivitaJKVnaytto()
{
    // if( veturi_ && veturi_->jkvlaite())
    //    veturi_->jkvlaite()->paivitaNaytto();

    if( veturi_)
        naytaNopeus( veturi_->nopeus());
}
