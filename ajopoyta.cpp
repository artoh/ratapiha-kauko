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

    connect( ui->ajopoyta1, SIGNAL(clicked(bool)), this, SLOT(ajopoytaYksi(bool)));
    connect( ui->ajopoyta2, SIGNAL(clicked(bool)), this, SLOT(ajopoytaKaksi(bool)));
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
        if( veturi_)
        {
            disconnect( veturi_, SIGNAL(nopeusIlmoitus(int)), ui->nopeusLabel, SLOT(setNum(int)));
            disconnect( veturi_, SIGNAL(automaatioIlmoitus(int,int,QPixmap)), this, SLOT(tilaMuutos(int,int, QPixmap)));
        }

        veturi_ = veturi;
        ui->veturinTyyppi->setText( veturi->vaununTyyppi());
        ui->nopeusSaadin->setValue( veturi->tavoiteNopeus());

        ui->ajopoyta1->setChecked( veturi->ajopoyta() == 1);
        ui->ajopoyta2->setChecked(veturi->ajopoyta() == 2);

        ui->ajopoyta1->setVisible( veturi->ajopoydat() == Vaunu::AjopoytaYksi || veturi->ajopoydat() == Vaunu::MolemmatAjopoydat);
        ui->ajopoyta2->setVisible( veturi->ajopoydat() == Vaunu::AjopoytaKaksi || veturi->ajopoydat() == Vaunu::MolemmatAjopoydat);

        ui->nopeusSaadin->setEnabled( veturi->ajopoyta() );

        connect( veturi_, SIGNAL(nopeusIlmoitus(int)), ui->nopeusLabel, SLOT(setNum(int)));
        connect( veturi_, SIGNAL(automaatioIlmoitus(int,int,QPixmap)), this, SLOT(tilaMuutos(int,int, QPixmap)));

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
    if( onko )
    {
        if( veturi_)
        {
            veturi_->asetaAjoPoyta(1);
        }
    }
    else
    {
        if( veturi_)
            veturi_->asetaAjoPoyta(0);
        ui->jkvTieto->clear();
    }

    ui->ajopoyta1->setChecked( veturi_->ajopoyta() == 1 );
    ui->ajopoyta2->setChecked( veturi_->ajopoyta() == 2 );
    ui->nopeusSaadin->setEnabled(veturi_->ajopoyta());

}

void Ajopoyta::ajopoytaKaksi(bool onko)
{

    if( onko )
    {
        if( veturi_)
        {
            veturi_->asetaAjoPoyta(2);
        }
    }
    else
    {
        if( veturi_)
        {
            veturi_->asetaAjoPoyta(0);
        }
            ui->jkvTieto->clear();
    }

    ui->ajopoyta1->setChecked( veturi_->ajopoyta() == 1 );
    ui->ajopoyta2->setChecked( veturi_->ajopoyta() == 2 );
    ui->nopeusSaadin->setEnabled(veturi_->ajopoyta());


}

void Ajopoyta::tilaMuutos(int ajopoyta, int tavoitenopeus, const QPixmap& kuva)
{
    ui->ajopoyta1->setChecked( ajopoyta == 1);
    ui->ajopoyta2->setChecked( ajopoyta == 2);

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
// Kosketusnäytön toiminnallisuus
{
    if( target == ui->jkvTieto && event->type() == QEvent::MouseButtonPress)
    {
        QMouseEvent *hiiritapaus = static_cast<QMouseEvent*>(event);
        if( veturi_ )
            veturi_->nayttoonKoskettu( hiiritapaus->pos());
        return true;
    }
    return QWidget::eventFilter(target,event);
}


void Ajopoyta::paivitaJKVnaytto()
{
    if( veturi_)
    {
        naytaNopeus( veturi_->nopeus());
        ui->jkvTieto->setPixmap( veturi_->jkvKuva());
    }
}

