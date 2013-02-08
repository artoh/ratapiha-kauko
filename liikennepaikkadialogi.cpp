#include "liikennepaikkadialogi.h"
#include "ui_liikennepaikkadialogi.h"

#include <QSqlQuery>
#include <QListWidgetItem>
#include <QRegExp>
#include <QMessageBox>

LiikennepaikkaDialogi::LiikennepaikkaDialogi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LiikennepaikkaDialogi),
    uusiLiikennepaikka_(false)
{
    ui->setupUi(this);

    // Haetaan luettelo
    QSqlQuery kysely("select liikennepaikka, nimi from liikennepaikka order by nimi");
    while( kysely.next())
    {
        QListWidgetItem* item = new QListWidgetItem(kysely.value(1).toString(), ui->liikennepaikkaLista);
        item->setData(Qt::UserRole, kysely.value(0).toString());
    }

    connect(ui->liikennepaikkaLista, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(valittuListasta(QListWidgetItem*)) );
    connect(ui->lyhenneEdit, SIGNAL(textChanged(QString)), this, SLOT(lyhenneMuuttunut(QString)) );
    connect( ui->tallennaButton, SIGNAL(clicked()), this, SLOT(tallenna()) );


}

LiikennepaikkaDialogi::~LiikennepaikkaDialogi()
{
    delete ui;
}

void LiikennepaikkaDialogi::valittuListasta(QListWidgetItem *uusi)
{
    QString lyhenne = uusi->data(Qt::UserRole).toString();
    if( ui->lyhenneEdit->text() != lyhenne )
        ui->lyhenneEdit->setText( lyhenne );
}

void LiikennepaikkaDialogi::lyhenneMuuttunut(const QString &lyhenne)
{
    if( lyhenne.length() > 1 )
    {
        // Haetaan tiedot
        QString kysymys = QString("select nimi, kmluku, TIME_FORMAT(sijainti_p,\"%H%i%S\"),"
                                  "TIME_FORMAT(sijainti_i,\"%H%i%S\"), henkiloliikenne, liikenteenohjaus "
                                  "from liikennepaikka where liikennepaikka=\"%1\" limit 1").arg(lyhenne);
        QSqlQuery kysely(kysymys);
        if( kysely.next())
        {
            // Liikennepaikka löytyi
            QString nimi = kysely.value(0).toString();
            double kmluku = kysely.value(1).toDouble();


            ui->nimiEdit->setText(nimi);
            ui->kmSpin->setValue(kmluku);

            if( kysely.isNull(2) || kysely.isNull(3))
            {
                ui->sijaintiEdit->clear();
            }
            else
            {
                QString sijaintiP = kysely.value(2).toString();
                QString sijaintiI = kysely.value(3).toString();

                sijaintiP.append(sijaintiI);

                ui->sijaintiEdit->setText(  sijaintiP  );

            }
            QString henkiloliikenne = kysely.value(4).toString();   // Kirjaimet junatyypeistä

            ui->taajamajunaCheck->setChecked( henkiloliikenne.contains(QChar('H')));
            ui->pikajunaCheck->setChecked( henkiloliikenne.contains(QChar('P')));
            ui->pendolinoCheck->setChecked( henkiloliikenne.contains(QChar('S')) );

            ui->liikenteenohjausCheck->setChecked( kysely.value(5).toBool() );  // Onko liikenteenohjausta

            // Merkitään sivussa olevaan listaan valituksi
            QList<QListWidgetItem*> hakulista = ui->liikennepaikkaLista->findItems(nimi, Qt::MatchExactly);
            if( hakulista.count())
            {
                QListWidgetItem* itm = hakulista.first();
                ui->liikennepaikkaLista->setCurrentItem(itm);
            }

            uusiLiikennepaikka_ = false;
            return;
        }
    }
    // Uusi liikennepaikka

    ui->nimiEdit->clear();
    ui->kmSpin->clear();
    ui->sijaintiEdit->clear();

    ui->taajamajunaCheck->setChecked(false);
    ui->pikajunaCheck->setChecked(false);
    ui->pendolinoCheck->setChecked(false);
    ui->liikenteenohjausCheck->setChecked(false);
    uusiLiikennepaikka_ = true;
}

void LiikennepaikkaDialogi::tallenna()
{
    // Tallentaa muutetun taikka uuden liikennepaikan. Ensin tarkistetaan ehdot.
    QString virheet;

    if( ui->lyhenneEdit->text().length() < 2 )
        virheet.append( tr("Liikennepaikan tunnus (lyhenne) puuttuu tai liian lyhyt\n"));
    if( ui->nimiEdit->text().length() < 2)
        virheet.append( tr("Liikennepaikan nimi puuttuu tai liian lyhyt\n"));
    if( ui->sijaintiEdit->text().contains(QRegExp("[0-9]")) )
    {
        QRegExp sijainnintarkastaja("[5-7][0-9].[0-5][0-9].[0-5][0-9].*[2-3][0-9].[0-5][0-9].[0-5][0-9].*");
        if( !sijainnintarkastaja.exactMatch(ui->sijaintiEdit->text()))
        {
            virheet.append("Sijaintikoordinaatit eivät kelpaa.\n");
            virheet.append(ui->sijaintiEdit->text());
        }

    }

    if( !virheet.isEmpty())
    {
        QMessageBox::critical(this, tr("Liikennepaikan tiedot virheelliset"), virheet );
        return;
    }

}
