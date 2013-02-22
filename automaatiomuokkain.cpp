#include "automaatiomuokkain.h"
#include "ui_automaatiomuokkain.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QRegExp>

AutomaatioMuokkain::AutomaatioMuokkain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AutomaatioMuokkain),
    muokattavaSaanto_(0)
{
    ui->setupUi(this);

    opastimenModel_ = new QSqlQueryModel(this);
    ui->automaatioLista->setModel(opastimenModel_);
    ui->automaatioLista->setSelectionMode( QAbstractItemView::SingleSelection);
    ui->automaatioLista->setSelectionBehavior( QAbstractItemView::SelectRows);

    heratteenModel_ = new QSqlQueryModel(this);
    ui->heratteenLista->setModel(heratteenModel_);
    ui->heratteenLista->setSelectionMode( QAbstractItemView::NoSelection );

    connect( ui->automaatioLista->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
                                                           this, SLOT(saantoValittu()) );

    connect( ui->atraideNappi, SIGNAL(clicked()), this, SLOT(aikatauluRaide()));
}

AutomaatioMuokkain::~AutomaatioMuokkain()
{
    delete ui;
}

void AutomaatioMuokkain::valitseOpastin(const QString& opastin)
{
    ui->opastinLabel->setText(opastin);

    QString kysymys = QString("select kulkutieautomaatio_id, herateraide,"
                              "maaliraide, jnehto, prioriteetti, viive,"
                              "kulkutientyyppi from kulkutieautomaatio "
                              "where opastin =\"%1\"").arg(opastin);

    opastimenModel_->setQuery(kysymys);
    ui->automaatioLista->resizeColumnsToContents();

    ui->automaatioLista->setColumnHidden(0, true);
    opastimenModel_->setHeaderData(1, Qt::Horizontal, tr("Heräte"));
    opastimenModel_->setHeaderData(2, Qt::Horizontal, tr("Maali"));
    opastimenModel_->setHeaderData(3, Qt::Horizontal, tr("Ehto"));
    opastimenModel_->setHeaderData(4, Qt::Horizontal, tr("Prior"));
    opastimenModel_->setHeaderData(5, Qt::Horizontal, tr("Viive"));
    opastimenModel_->setHeaderData(6, Qt::Horizontal, tr("Tyyppi"));

    ui->automaatioLista->selectRow(0);
    saantoValittu();
}


void AutomaatioMuokkain::saantoValittu()
{
    int rivi = ui->automaatioLista->currentIndex().row();
    QSqlRecord tietue = opastimenModel_->record(rivi);

    // Nyt laitetaan tiedot paikoilleen
    muokattavaSaanto_ = tietue.value(0).toInt();

    if( tietue.isNull(0))
        ui->herateNappi->setText( ui->opastinLabel->text() );
    else
        ui->herateNappi->setText( tietue.value(1).toString() );

    ui->maaliNappi->setText( tietue.value(2).toString());
    ui->ehtoEdit->setText( tietue.value(3).toString());
    ui->prioriteettiSpin->setValue( tietue.value(4).toInt());
    ui->viiveSpin->setValue( tietue.value(5).toInt());
    ui->aikatauluViiveCheck->setChecked( tietue.value(5).isNull() &&
                                         !tietue.isNull(0) );

    QChar tyyppikirjain = tietue.value(6).toString()[0];

    if( tyyppikirjain.toUpper() == 'U')
        ui->tyyppiCombo->setCurrentIndex(1);
    else if( tyyppikirjain.toUpper() == 'V')
        ui->tyyppiCombo->setCurrentIndex(2);
    else
        ui->tyyppiCombo->setCurrentIndex(0);


    ui->kerranCheck->setChecked( tyyppikirjain.isLower() );


    // Herätekysely
    QString kysymys = QString("select kulkutieautomaatio_id, opastin,"
                              "maaliraide, jnehto, prioriteetti, viive,"
                              "kulkutientyyppi from kulkutieautomaatio "
                              "where herateraide =\"%1\"").arg( tietue.value(1).toString() );

    heratteenModel_->setQuery(kysymys);
    ui->heratteenLista->resizeColumnsToContents();
    ui->heratteenLista->setColumnHidden(0, true);

    heratteenModel_->setHeaderData(1, Qt::Horizontal, tr("Opastin"));
    heratteenModel_->setHeaderData(2, Qt::Horizontal, tr("Maali"));
    heratteenModel_->setHeaderData(3, Qt::Horizontal, tr("Ehto"));
    heratteenModel_->setHeaderData(4, Qt::Horizontal, tr("Prior"));
    heratteenModel_->setHeaderData(5, Qt::Horizontal, tr("Viive"));
    heratteenModel_->setHeaderData(6, Qt::Horizontal, tr("Tyyppi"));



}

void AutomaatioMuokkain::aikatauluRaide()
{
    QString maali = ui->maaliNappi->text();
    maali.replace( QRegExp("\\d"),"" );
    ui->maaliNappi->setText( maali );
}
