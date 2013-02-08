#include "liikennepaikkadialogi.h"
#include "ui_liikennepaikkadialogi.h"

#include <QSqlQuery>
#include <QListWidgetItem>
#include <QRegExp>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

LiikennepaikkaDialogi::LiikennepaikkaDialogi(QWidget *parent) :
    QWidget(parent),
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
    QString sijaintiP("NULL");
    QString sijaintiI("NULL");

    if( ui->lyhenneEdit->text().length() < 2 )
        virheet.append( tr("Liikennepaikan tunnus (lyhenne) puuttuu tai liian lyhyt\n"));
    if( ui->nimiEdit->text().length() < 2)
        virheet.append( tr("Liikennepaikan nimi puuttuu tai liian lyhyt\n"));
    if( ui->sijaintiEdit->text().contains(QRegExp("[0-9]")) )
    {
        QRegExp sijainnintarkastaja("[5-7][0-9].[0-5][0-9].[0-5][0-9].*[2-3][0-9].[0-5][0-9].[0-5][0-9].*");
        // Sijainnin paree olla Suomessa tai sitten ihan lähellä !
        if( !sijainnintarkastaja.exactMatch(ui->sijaintiEdit->text()))
            virheet.append("Sijaintikoordinaatit eivät kelpaa.\n");
        else
        {
            // Kelvollinen sijainti
            QString st = ui->sijaintiEdit->text();
            sijaintiP = QString("\"%1:%2:%3\"").arg( st.left(2)).arg(st.mid(3,2)).arg(st.mid(6,2));
            sijaintiI = QString("\"%1:%2:%3\"").arg( st.mid(13,2)).arg(st.mid(16,2)).arg(st.mid(19,2));
        }
    }

    if( !virheet.isEmpty())
    {
        QMessageBox::critical(this, tr("Liikennepaikan tiedot virheelliset"), virheet );
        return;
    }

    // Nyt kyllä kelpaa, siis tallentamaan

    // Tehdään muutama string... Tässä yksi pilkku liikaa lopussa
    QString henkiloliikenne;
    if( ui->taajamajunaCheck->isChecked())
        henkiloliikenne.append("H,");
    if(ui->pikajunaCheck->isChecked())
        henkiloliikenne.append("P,");
    if( ui->pendolinoCheck->isChecked())
        henkiloliikenne.append("S,");

    QString liikenteenohjaus;
    if( ui->liikenteenohjausCheck->isChecked())
        liikenteenohjaus = "1";
    else
        liikenteenohjaus = "2";

    if( uusiLiikennepaikka_)
    {
        // insert ...
        QString kysymys = QString("insert into liikennepaikka(liikennepaikka,nimi,kmluku,sijainti_p,sijainti_i,henkiloliikenne,liikenteenohjaus) "
                                 "values(\"%1\",\"%2\",%3,%4,%5,\"%6\",%7)").arg(ui->lyhenneEdit->text())
                                                                .arg(ui->nimiEdit->text())
                                                                .arg(ui->kmSpin->value())
                .arg(sijaintiP).arg(sijaintiI).arg(henkiloliikenne.left(henkiloliikenne.length()-1)).arg(liikenteenohjaus);

        qDebug() << kysymys;


        QSqlQuery kysely(kysymys);
        if( kysely.lastError().isValid() )
        {
            QMessageBox::critical(this,"Virhe tallettamisessa",kysely.lastError().databaseText());
        }
        else
        {
            // lisätään myös listaan, aktiiviseksi
            QListWidgetItem* item = new QListWidgetItem(ui->nimiEdit->text(), ui->liikennepaikkaLista);
            item->setData(Qt::UserRole,  ui->lyhenneEdit->text() );
            ui->liikennepaikkaLista->setCurrentItem(item);
            uusiLiikennepaikka_ = false;
        }
    }
    else
    {
        // update
        QString kysymys = QString("update liikennepaikka set nimi=\"%2\", kmluku=%3, sijainti_p=%4, sijainti_i=%5, "
                                  "henkiloliikenne=\"%6\", liikenteenohjaus=%7 where liikennepaikka=\"%1\" limit 1")
                                .arg(ui->lyhenneEdit->text()).arg(ui->nimiEdit->text()).arg(ui->kmSpin->value())
                .arg(sijaintiP).arg(sijaintiI).arg(henkiloliikenne.left(henkiloliikenne.length()-1)).arg(liikenteenohjaus);

        qDebug() << kysymys;


        QSqlQuery kysely(kysymys);
        if( kysely.lastError().isValid() )
        {
            QMessageBox::critical(this,"Virhe tallettamisessa",kysely.lastError().databaseText());
        }

    }


}
