/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#include "kaukoikkuna.h"
#include "kaukoyhteys.h"
#include <QApplication>
#include <QTextCodec>

int main( int argc, char* argv[])
{
    QApplication app(argc, argv);

    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );

    app.setApplicationName("Kaukokäyttö");
    app.setApplicationVersion("0.001");
    app.setOrganizationName("Ratapiha");

    KaukoYhteys yhteys;

    // Pääikkunaan asti päästään vain, mikäli yhteys tietokantaan muodostuu...
    if( yhteys.onkoLukuyhteys())
    {

        // Varaudutaan monen KaukoIkkunan käyttöliittymään
        KaukoIkkuna* kaukoikkuna = new KaukoIkkuna;

        kaukoikkuna->show();
        return app.exec();
    }
}
