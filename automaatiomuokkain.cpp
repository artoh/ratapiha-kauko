#include "automaatiomuokkain.h"
#include "ui_automaatiomuokkain.h"
#include "ratapihaikkuna.h"

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

    connect( ui->opastinNappi, SIGNAL(clicked(bool)), this, SLOT(kytkeAutomaatio(bool)));
    connect( ui->poistaNappi, SIGNAL(clicked()), this, SLOT(poista()));
    connect( ui->valmisNappi, SIGNAL(clicked()), this, SLOT(tallenna()));
}

AutomaatioMuokkain::~AutomaatioMuokkain()
{
    delete ui;
}

void AutomaatioMuokkain::valitseOpastin(const QString& opastin)
{
    QString paatila = raiteenpaatila(opastin);

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

    ui->opastinNappi->setEnabled( opastimenModel_->rowCount() > 0 );

    ui->automaatioLista->resizeColumnsToContents();

    ui->automaatioLista->setColumnHidden(0, true);
    opastimenModel_->setHeaderData(1, Qt::Horizontal, tr("Heräte"));
    opastimenModel_->setHeaderData(2, Qt::Horizontal, tr("Maali"));
    opastimenModel_->setHeaderData(3, Qt::Horizontal, tr("Ehto"));
    opastimenModel_->setHeaderData(4, Qt::Horizontal, tr("Prior"));
    opastimenModel_->setHeaderData(5, Qt::Horizontal, tr("Viive"));
    opastimenModel_->setHeaderData(6, Qt::Horizontal, tr("Tyyppi"));

    ui->automaatioLista->selectRow( opastimenModel_->rowCount() - 1 );
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
                              "where herateraide =\"%1\" order by prioriteetti").arg( tietue.value(1).toString() );

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

void AutomaatioMuokkain::kytkeAutomaatio(bool paalle)
{
    if( paalle )
        RatapihaIkkuna::getInstance()->aslKasky(QString("AUON %1").arg(opastin_));
    else
        RatapihaIkkuna::getInstance()->aslKasky(QString("AUEI %1").arg(opastin_));

}

void AutomaatioMuokkain::poista()
{
    qDebug() << QString("delete from kulkutieautomaatio where kulkutieautomaatio_id=%1").arg(muokattavaSaanto_);
    if( muokattavaSaanto_)
    {
        QString poistokysely = QString("delete from kulkutieautomaatio where kulkutieautomaatio_id=%1").arg(muokattavaSaanto_);
        kytkeAutomaatio( ui->opastinNappi->isChecked());    // Kytkee pois, jos ainoa sääntö poistettiin
        valitseOpastin(opastin_);
    }

}

void AutomaatioMuokkain::tallenna()
{
    // Ensin kelpoisuus
    QRegExp ehtotesti(ui->ehtoEdit->text());

    if( ui->herateNappi->text().isEmpty() ||
            ui->maaliNappi->text().isEmpty() ||
            !ehtotesti.isValid())
    {
        return; // Virheellinen !!!
    }

    QString jnehto;
    QString viive;
    QString tyyppikirjain;

    if( ui->ehtoEdit->text().isEmpty())
        jnehto = "NULL";
    else
        jnehto = QString("\"%1\"").arg(ui->ehtoEdit->text());

    if( ui->aikatauluViiveCheck->isChecked())
        viive = "NULL";
    else
        viive = QString::number( ui->viiveSpin->value());

    if( ui->tyyppiCombo->currentIndex() == 0 && !ui->kerranCheck->isChecked() )
        tyyppikirjain = "J";
    else if( ui->tyyppiCombo->currentIndex() == 0 && ui->kerranCheck->isChecked() )
        tyyppikirjain = "j";
    else if( ui->tyyppiCombo->currentIndex() == 1 && !ui->kerranCheck->isChecked() )
        tyyppikirjain = "U";
    else if( ui->tyyppiCombo->currentIndex() == 1 && ui->kerranCheck->isChecked() )
        tyyppikirjain = "u";
    else if( ui->tyyppiCombo->currentIndex() == 2 && !ui->kerranCheck->isChecked() )
        tyyppikirjain = "V";
    else if( ui->tyyppiCombo->currentIndex() == 2 && ui->kerranCheck->isChecked() )
        tyyppikirjain = "v";


    QString kysymys;
    if( muokattavaSaanto_)
    {
        // Update
        kysymys = QString("update kulkutieautomaatio set herateraide=\"%1\", maaliraide=\"%2\", jnehto=%3, "
                                  "prioriteetti=%4, viive=%5, kulkutientyyppi=\"%6\" where kulkutieautomaatio_id=%7 ")
                .arg(ui->herateNappi->text()).arg(ui->maaliNappi->text())
                .arg(jnehto).arg(ui->prioriteettiSpin->value()).arg(viive).arg(tyyppikirjain).arg(muokattavaSaanto_);

    }
    else
    {
        // Insert
        kysymys = QString("insert into kulkutieautomaatio(herateraide, opastin, maaliraide, jnehto, prioriteetti, viive, kulkutientyyppi) "
                          "values(\"%1\", \"%7\", \"%2\", %3, %4, %5, \"%6\")")
                .arg(ui->herateNappi->text()).arg(ui->maaliNappi->text())
                .arg(jnehto).arg(ui->prioriteettiSpin->value()).arg(viive).arg(tyyppikirjain).arg(opastin_);
    }
    QSqlQuery kysely(kysymys);

    valitseOpastin(opastin_);
        qDebug() << kysymys;
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
