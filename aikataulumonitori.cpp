#include "aikataulumonitori.h"

#include <QSqlQuery>
#include <QToolBar>

#include <QDebug>

#include "ratapihaikkuna.h"

AikatauluMonitori::AikatauluMonitori(QWidget *parent) :
    QMainWindow(parent)
{
    monitori_ = new QTextBrowser(this);
    setCentralWidget(monitori_);

    QToolBar* tbar = addToolBar( tr("Monitori"));
    asemaValintaCombo_ = new QComboBox;
    connect( asemaValintaCombo_, SIGNAL(currentIndexChanged(int)),
             this, SLOT(haeAsema(int)));

    QSqlQuery asemakysely("select liikennepaikka, nimi from liikennepaikka where henkiloliikenne !=\"\" order by nimi");
    while( asemakysely.next())
    {
        asemaValintaCombo_->addItem( asemakysely.value(1).toString(),
                                      asemakysely.value(0).toString());
    }


    tbar->addWidget(asemaValintaCombo_);

    connect(RatapihaIkkuna::getInstance(), SIGNAL(kello(QDateTime)), this, SLOT(paivita(QDateTime)) );
}

void AikatauluMonitori::paivita(const QDateTime &aika)
{
    // Kirjoittaa monitoriin junat ko. hetkestä eteenpäin, 10 junaa..

    QString teksti = "<html><body>";

    teksti.append( tr("<table width=100%><tr><td class=asema>%1</td><td class=kello align=right>%2</td></tr></table>")
                   .arg( asemanNimi_).arg( aika.toString("hh:mm") ) ) ;
    teksti.append( tr("<table width=100%><tr><th align=left>Lähtee</th><th align=left>Juna</th><th align=left>Minne</th><th align=left>Raide</th></tr>"));

    int rivi = 0;
    int raidejakaja = 10;
    if( asemanNimi_ == "Helsinki" || asemanNimi_ == "Pasila ")
        raidejakaja = 100;


    if( tietueet_.count())
    {

        QListIterator<MonitoriTietue> i(tietueet_);

        QTime kello = aika.time();
        QString ekajuna;

        while(1)
        {
            if( !rivi )
            {
                QTime lahtoaika = i.next().lahtoaika();
                if( kello.secsTo(lahtoaika) > -90  )
                {
                    rivi = 1;
                    i.previous();
                }
            }
            if( rivi )
            {
                // Nämä laitetaan tauluun
                MonitoriTietue tietue = i.next();

                // Katkaisemaan, jos on menossa ympäri ämpäri vuorokauden
                if( ekajuna.isNull() )
                    ekajuna = tietue.junatunnus();
                else if( ekajuna == tietue.junatunnus())
                    break;

                QString tahti = "lahtee";
                if( kello.secsTo(tietue.lahtoaika()) > -90 && kello.secsTo(tietue.lahtoaika()) < 3 * 60 )
                    tahti = "lahteepian";

                teksti.append( QString("<tr><td class=%5>%1</td><td class=juna>%2</td><td class=minne>%3</td><td class=raide>%4</td></tr>")
                        .arg(tietue.lahtoaika().toString("hh:mm"))
                        .arg(tietue.junatunnusNaytettava())
                        .arg(tietue.minne())
                        .arg(tietue.raide() % raidejakaja )
                               .arg(tahti));

                rivi++;

                if( rivi > 10)
                    break;

            }

            // Jos tullaan puoleenyöhön, aloitetaan alusta !
            if( !i.hasNext())
            {
                if( !rivi )
                    rivi = 1;
                i.toFront();
            }
        }

    }
    teksti.append("</table></body></html>");

    monitori_->clear();
    QString tyyli = " * { font-family: \"Helvetica\"; color: white;} "
            "table { background-color: darkblue; }"
            "td.asema  { font-size: 18px;font-weight: bold;  }  "
            "td.kello  { font-size: 18px; text-align: right;font-weight: bold;  }"
            "th { font-weight: normal; text-align:left; }  "
            "td.lahtee { font-size: 14px; } "
            "td.lahteepian { font-size: 14px; font-weight: bold; color:red; } "
            "td.minne { font-size: 14px; } "
            "td.raide { font-size: 14px; } "
            "td.juna { font-size: 14px; }";
    monitori_->document()->setDefaultStyleSheet(tyyli);
    monitori_->setHtml(teksti);


}



void AikatauluMonitori::haeAsema(int indeksi)
{
    QString liikennepaikka = asemaValintaCombo_->itemData(indeksi).toString();
    asemanNimi_ = asemaValintaCombo_->currentText();
    setWindowTitle( tr("Lähtevät junat: %1").arg( asemanNimi_ )  );

    tietueet_.clear();

    QString kysymys = QString("select juna.junanro as Juna, "
            "addtime(lahtee, taulu.lahtoaika) as Lahtoaika, "
            "minnelkp.nimi as minne, taulu.raide as laituri "
            "from juna, aikataulu as taulu, liikennepaikka as minnelkp, "
            "aikataulu as minne "
            "where taulu.reitti=juna.reitti and taulu.reitti=minne.reitti "
            "and minnelkp.liikennepaikka = minne.liikennepaikka and "
            "minne.tapahtuma=\"S\" and ( taulu.tapahtuma=\"L\" or taulu.tapahtuma=\"P\" ) and "
                              "taulu.liikennepaikka=\"%1\" order by Lahtoaika").arg(liikennepaikka);

    qDebug() << kysymys;

    QSqlQuery junakysely(kysymys);

    while(junakysely.next())
    {
        QString juna = junakysely.value(0).toString();
        QTime aika = junakysely.value(1).toTime();
        QString minne = junakysely.value(2).toString();
        int laituri = junakysely.value(3).toInt();

        // Vain lähtevät matkustajajunat monitoriin!
        if( juna.startsWith('H') || juna.startsWith('P') || juna.startsWith('S'))
            tietueet_.append( MonitoriTietue(aika,juna,minne,laituri) );

        qDebug() << aika << juna << minne << laituri << "\n";
    }

    paivita( RatapihaIkkuna::getInstance()->simulaatioAika() );
}


