#include "automaatiomuokkain.h"
#include "ui_automaatiomuokkain.h"

#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QRegExp>

#include <QDebug>

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

    connect( ui->lisaaNappi, SIGNAL(clicked()), this, SLOT(uusiSaanto()));

    connect( ui->maaliNappi, SIGNAL(clicked()), this, SIGNAL(valitseMaaliraide()) );
    connect( ui->herateNappi, SIGNAL(clicked()), this, SIGNAL(valitseHerateraide()) );
    connect( ui->ehtoEdit, SIGNAL( textChanged(QString) ), this, SLOT(tarkistaEhto()) );
}

AutomaatioMuokkain::~AutomaatioMuokkain()
{
    delete ui;
}

void AutomaatioMuokkain::valitseOpastin(const QString& opastin)
{
    QString paatila = raiteenpaatila(opastin);
    qDebug() << paatila;

    if( paatila.contains("Auto") || paatila.contains("Läpikulku"))
    {
        ui->opastinNappi->setChecked(true);
    }
    else if( paatila.contains("Po") || paatila.contains("Ro"))
    {
        ui->opastinNappi->setChecked(false);
    }
    else
    {
        if( opastin_.isEmpty())
            setVisible(false);
        return; // Ei hyväksy, kun ei ole opastinta!
    }

    opastin_ = opastin;

    QString liikennepaikka = opastin.mid(1);
    liikennepaikka.replace(QRegExp("\\d"),"");  // Liikennepaikka: numerot pois..

    QSqlQuery nimikysely( QString("select nimi from liikennepaikka where liikennepaikka=\"%1\"").arg(liikennepaikka));
    if( nimikysely.next())
        liikennepaikka = nimikysely.value(0).toString();

    QString raidenro = opastin;
    raidenro.replace( QRegExp("\\D"),"");   // Numero-osa: kirjaimet pois

    QString lteksti = QString("%1\n%2%3").arg(liikennepaikka)
            .arg(opastin[0]).arg(raidenro);


    ui->opastinLabel->setText(lteksti);

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


void AutomaatioMuokkain::saantoValittu(bool tyhjenna)
{
    QSqlRecord tietue;
    if(ui->automaatioLista->currentIndex().isValid() && !tyhjenna)
        tietue = opastimenModel_->record( ui->automaatioLista->currentIndex().row() );

    // Nyt laitetaan tiedot paikoilleen
    muokattavaSaanto_ = tietue.value(0).toInt();

    if( tietue.isNull(0))
        ui->herateNappi->setText( opastin_);
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

void AutomaatioMuokkain::uusiSaanto()
{
    ui->automaatioLista->clearSelection();
    saantoValittu(true);
}

void AutomaatioMuokkain::tarkistaEhto()
{
    QRegExp ehto(ui->ehtoEdit->text());
    if( ehto.isValid())
        ui->ehtoEdit->setStyleSheet("color: black;");
    else
        ui->ehtoEdit->setStyleSheet("color: red;");
}

void AutomaatioMuokkain::asetaHerateraide(const QString &raiteenpaa)
{
    ui->herateNappi->setText( raiteenpaa );
}

void AutomaatioMuokkain::asetaMaaliraide(const QString &raiteenpaa)
{
    QString tarkastus = raiteenpaatila(raiteenpaa);
    if( tarkastus.contains("Po") || tarkastus.contains("So")
            || tarkastus.contains("Ro") || tarkastus.contains("RP"))
        ui->maaliNappi->setText(raiteenpaa.mid(1));
}


QString AutomaatioMuokkain::raiteenpaatila(const QString &raiteenpaa)
{
    QString liikennepaikka = raiteenpaa.mid(1);
    liikennepaikka.replace(QRegExp("\\d"),"");  // Liikennepaikka: numerot pois..

    QString raidenro = raiteenpaa;
    raidenro.replace( QRegExp("\\D"),"");   // Numero-osa: kirjaimet pois

    int raidenumero = raidenro.toInt();

    QString kysymys = QString("select tila_etela, tila_pohjoinen from raide where liikennepaikka=\"%1\" and raide=%2 ")
            .arg(liikennepaikka).arg(raidenumero);

    QSqlQuery kysely(kysymys);
    if( kysely.next())
    {
        if( raiteenpaa.startsWith('E'))
            return kysely.value(0).toString();
        return kysely.value(1).toString();
    }
    return QString();
}
