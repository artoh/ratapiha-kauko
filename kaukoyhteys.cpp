/**************************************************************************
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukoyhteys.h"

#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>

KaukoYhteys::KaukoYhteys(QObject *parent) :
    QObject(parent)
{
    // Yhteydet muodostetaan rakentajassa!

    tietokanta_ = QSqlDatabase::addDatabase("QMYSQL");
    tietokanta_.setHostName("localhost");
    tietokanta_.setDatabaseName("ratapiha");
    tietokanta_.setUserName("ratapiha");
    tietokanta_.setPassword("ratapiha");

    if(!tietokanta_.open())
    {
        // Jatkossa tässä näytetään dialogi, jossa kysytään asetuksia ja yritetään uudelleen...

        QMessageBox::critical(0, tr("Tietokantavirhe"),tietokanta_.lastError().text());
    }

}

KaukoYhteys::~KaukoYhteys()
{
    tietokanta_.close();
}



bool KaukoYhteys::onkoLukuyhteys()
{
    return tietokanta_.isOpen();
}

bool KaukoYhteys::onkoKaskyyhteys()
{
    return false; // TCP-yhteyttä ei vielä toteutettu
}
