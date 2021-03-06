/**************************************************************************
**  aikatauluselaaja.cpp
**  Copyright (c) 2012 Arto Hyvättinen 
**
**  This program is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  See <http://www.gnu.org/licenses/>
**
**  AikatauluSelaaja  4.8.2012
**************************************************************************/

#include "aikatauluselaaja.h"
#include "ratapihaikkuna.h"

#include <QSqlQuery>
#include <QTime>

AikatauluSelaaja::AikatauluSelaaja(QWidget *parent) :
    QTextBrowser(parent) ,
    selattavanTyyppi_(Tyhja),
    lahtevat_(true),
    saapuvat_(true),
    etelaan_(true),
    pohjoiseen_(true),
    lahijunat_(true),
    kaukojunat_(true),
    muutjunat_(true)
{
    setOpenLinks(false);
    connect( this, SIGNAL(anchorClicked(QUrl)), this, SLOT(klikattu(QUrl)));
}


void AikatauluSelaaja::haeAsemaAikataulu(const QString &liikennepaikka)
{
    QString teksti = "<html><body>";


    // Seuraavaksi yksinkertainen sqlkysymys, joka kertoo asemalta kulkevat junat

    QString kysymys = QString("select juna.junanro as Juna, addtime(lahtee,taulu.lahtoaika) as Kello, "
    "minnelkp.nimi as Minne,  addtime(lahtee, minne.saapumisaika) as Perilla, taulu.raide as Laituri, "
    "taulu.pysahtyy as pysahtyy, taulu.tapahtuma as tapahtuma, minnelkp.liikennepaikka as lyhenne, taulu.lahtoaika, "
                              "addtime(lahtee,taulu.saapumisaika) as Saapuminen, taulu.suunta "
    "from juna, aikataulu as taulu, aikataulu as minne, liikennepaikka as minnelkp "
    " where taulu.reitti = juna.reitti and taulu.reitti = minne.reitti and "
    " minnelkp.liikennepaikka = minne.liikennepaikka "
    " and minne.tapahtuma=\"S\" and taulu.tapahtuma != \"O\" "
                              " and taulu.liikennepaikka=\"%1\" order by EiNulAikaa(Kello,Saapuminen) ").arg( liikennepaikka );


    QSqlQuery asemaKysely( QString("select nimi from liikennepaikka where liikennepaikka=\"%1\"").arg(liikennepaikka));

    QString asemanimi;
    if( asemaKysely.next() )
    {
        asemanimi = asemaKysely.value(0).toString();
        teksti.append( QString("<h1>%1</h1>").arg( asemanimi.toUpper()) );
        setWindowTitle(tr("Aikataulu %1").arg( asemanimi ));
    }

    teksti.append("\n<table width=100%><tr><th>Saapuu</th><th>Lähtee</th><th>Juna</th><th>Määräasema</th><th>Perillä</th><th>Raide</th></tr>\n");

    int rivi = 0;
    int tunti = -1;
    bool raideVarattu = false;
    QTime edellinenSaapui;

    QSqlQuery junaKysely( kysymys );
    while( junaKysely.next())
    {
        QString juna = junaKysely.value(0).toString();
        QString tapahtuma = junaKysely.value(6).toString();
        QString suuntakirjain = junaKysely.value(10).toString();


        int raide = junaKysely.value(4).toInt();
        // Näytetään vain raiteen viimeinen numero (paitsi Helsingissä kaksi)
        if( liikennepaikka == "Hki" || liikennepaikka == "Psl")
            raide = raide % 100;
        else
            raide = raide % 10;

        if( raide_ > 0 && raide != raide_ )
            continue;   // Raidefiltteri

        QTime lahtee = junaKysely.value(1).toTime();
        QString minne = junaKysely.value(2).toString();
        QTime perilla = junaKysely.value(3).toTime();
        QTime saapuu = junaKysely.value(9).toTime();

        // Jos filtteröidään raiteella, selvittää myös "raidevirheen" eli jos raide ylikuormittuu. Ei ole virhe, jos
        // junia liitetään tai pätkitään...

        bool raidevirhe = ( (tapahtuma != "O" && tapahtuma != "P" && ( (raide_ && raideVarattu && saapuu.isValid() )  || ( raide_ && !raideVarattu && lahtee.isValid() ) ) ) ||
                            (tapahtuma == "P"  && raide_ && raideVarattu)) ;
        if( saapuu.isValid() )
            raideVarattu = true;
        if( lahtee.isValid() || tapahtuma == "O")
            raideVarattu = false;

        QString raidetyyli="raide";
        if( raidevirhe )
            raidetyyli = "virheraide";
        else if( raide_ && edellinenSaapui.isValid() && lahtee.isValid() && edellinenSaapui.secsTo(lahtee) < 5 * 60 )
            raidetyyli = "kiireraide";

        if( saapuu.isValid())
            edellinenSaapui = saapuu;



        QString minnelyhenne = junaKysely.value(7).toString();



        // Filtteröinti
        if( !saapuvat_ && tapahtuma == "S" )
            continue;
        else if( !lahtevat_ && tapahtuma == "L")
            continue;
        else if( !etelaan_ && suuntakirjain == "E" )
            continue;
        else if( !pohjoiseen_ && suuntakirjain == "P")
            continue;
        else if( !lahijunat_ && juna.startsWith('H'))
            continue;
        else if( !kaukojunat_ && ( juna.startsWith('P') || juna.startsWith('S')  ) )
            continue;
        else if( !muutjunat_ && ( juna.startsWith('M') || juna.startsWith('X') || juna.startsWith('T')  ) )
            continue;


        rivi++;


        QTime kello = lahtee;
        if( !kello.isValid())
            kello = saapuu;

        if( tunti != kello.hour())
        {
            tunti = kello.hour();
            teksti.append(QString("<tr><td class=tunti colspan=6 align=center>%1.00</td></tr>").arg(tunti ));
            rivi = 1;
        }

        if( rivi % 2 )
            teksti.append("<tr><td class=saapuu>");
        else
            teksti.append("<tr class=varjo><td class=saapuu>");
        if( saapuu.isValid())
            teksti.append( saapuu.toString("hh.mm"));
        teksti.append("</td><td class=lahtee>");
        if( lahtee.isValid())
            teksti.append( lahtee.toString("hh.mm"));



        QString korostettuJunaTunnus;
        if( juna.startsWith('H') && juna.length() > 3 && juna[1].isLetter() )
        {
            korostettuJunaTunnus = QString("%1 <b>%2</b> %3")
                    .arg(juna.mid(0,1)).arg(juna.mid(1,1)).arg(juna.mid(2));
        }
        else
            korostettuJunaTunnus = juna;



        teksti.append( QString("</td><td class=juna><a href=\"file:J%1\">%6</a> </td><td class=minne><a href=\"file:A%5\">%2</a></td>"
                               "<td class=perilla>%3</td><td class=%7>%4</td></tr>\n")
                       .arg(juna).arg(minne).arg( perilla.toString("hh:mm")).arg(raide).arg(minnelyhenne).arg(korostettuJunaTunnus).arg(raidetyyli)   );

    }
    teksti.append("</table></body></html>");

    QString tyyli = " * { font-family: \"Helvetica\"; } "
            "h1  { font-size: 16px; background-color: yellow; }  "
            "th { font-weight: normal; background-color: lightgray; }  "
            "tr.varjo { background-color: lightgray; } "
            "td.lahtee { font-size: 14px; font-weight: bold; } "
            "td.saapuu { font-size: 14px; } "
            "td.minne { font-size: 14px; font-weight: bold; } "
            "td.raide { font-size: 14px; } "
            "td.virheraide { font-size: 14px; background-color: red; } "
            "td.kiireraide { font-size: 14px; background-color: orange; } "
            "td.tunti { background-color: darkgray; font-size:14px; } "
            "a  { color: black; text-decoration:none;  }";
    clear();
    document()->setDefaultStyleSheet(tyyli);
    setHtml(teksti);

    selattavanTyyppi_ = Asema;
    selattavanTunnus_ = liikennepaikka;
    emit naytetaanAsema(liikennepaikka);
}

void AikatauluSelaaja::asetaFiltteri(bool saapuvat, bool lahtevat, bool etelaan, bool pohjoiseen, bool lahi, bool kauko, bool muut, int raide)
{
    saapuvat_ = saapuvat;
    lahtevat_ = lahtevat;
    etelaan_ = etelaan;
    pohjoiseen_ = pohjoiseen;

    lahijunat_ = lahi;
    kaukojunat_ = kauko;
    muutjunat_ = muut;
    raide_ = raide;

    if( selattavanTyyppi_ == Asema )
        paivita();
}

void AikatauluSelaaja::paivita()
{
    if( selattavanTyyppi_ == Asema )
        haeAsemaAikataulu(selattavanTunnus_);
    else
        haeJunaAikataulu(selattavanTunnus_);
}

void AikatauluSelaaja::haeJunaAikataulu(const QString &juna)
{
    if( juna.startsWith("(LOKI)"))
    {
        naytaLoki( juna.mid(6));
        return;
    }


    QString kysymys = QString("select nimi, addtime(lahtee,lahtoaika) as aika, raide, tapahtuma, aikataulu.liikennepaikka, addtime(lahtee,saapumisaika) as saapunee "
                              "from juna,aikataulu,liikennepaikka "
                              "where juna.junanro=\"%1\" and "
                              "juna.reitti = aikataulu.reitti and aikataulu.liikennepaikka=liikennepaikka.liikennepaikka "
                              "order by saapunee,aika").arg(juna);


    QString teksti = QString("<html><body><h1>%1</h1>").arg(juna);

    teksti.append("<table width=100%><tr><th>Liikennepaikka</th><th>Saapuu</th><th>Lähtee</th><th>Raide</th></tr>\n");

    QSqlQuery kysely(kysymys);
    int rivi = 0;
    while( kysely.next())
    {
        rivi++;
        QString liikennepaikka = kysely.value(0).toString();
        QTime lahtee = kysely.value(1).toTime();

        int raide = kysely.value(2).toInt();
        QString tapahtuma = kysely.value(3).toString();
        QString liikennepaikkaLyhenne = kysely.value(4).toString();
        QTime saapuu = kysely.value(5).toTime();

        // Näytetään vain raiteen viimeinen numero (paitsi Helsingissä kaksi)
        if( liikennepaikkaLyhenne == "Hki" || liikennepaikkaLyhenne =="Psl")
            raide = raide % 100;
        else
            raide = raide % 10;

        if( rivi % 2 )
            teksti.append("<tr>");
        else
            teksti.append("<tr class=varjo>");

        teksti.append( QString("<td><a href=\"file:A%1\">%2</td><td>").arg(liikennepaikkaLyhenne).arg(liikennepaikka));
        if( tapahtuma == "O") // Ohitusaika sulkeisiin!
            teksti.append("(");
        if( saapuu.isValid())
            teksti.append( saapuu.toString("hh.mm"));
        if( tapahtuma == "O")
            teksti.append(")");

        teksti.append("</td><td>");
        if( lahtee.isValid())
            teksti.append( lahtee.toString("hh.mm"));
        teksti.append( "</td><td>" );
        if( tapahtuma != "O")
            teksti.append( QString::number(raide));
        teksti.append("</td></tr>\n");
    }

    teksti.append("</table>");

    teksti.append(QString("<hr><a href=\"file:J(LOKI)%1\">Kulkutiedot</a>").arg(juna));

    teksti.append("</body></html>");

    QString tyyli = " * { font-family: \"Helvetica\"; } "
            "h1  { font-size: 16px; background-color: yellow; }  "
            "th { font-weight: normal; background-color: lightgray }  "
            "tr.varjo { background-color: lightgray; } "
            "a  { font-size: 14px; color: black; text-decoration:none;  }"
            "td { font-size: 14px; } ";
    clear();
    document()->setDefaultStyleSheet(tyyli);
    setHtml(teksti);

    selattavanTyyppi_ = Juna;
    selattavanTunnus_ = juna;
    emit naytetaanJuna(juna);

}

void AikatauluSelaaja::klikattu(const QUrl &linkki)
{
    QString haettava = linkki.toLocalFile();
    if( haettava.startsWith('J'))
        haeJunaAikataulu(haettava.mid(1));
    else if( haettava.startsWith('A'))
        haeAsemaAikataulu(haettava.mid(1));
}

void AikatauluSelaaja::naytaLoki(const QString &junanumero)
{

    QDateTime verrokki = RatapihaIkkuna::getInstance()->simulaatioAika().addDays(-1);
    QString aikajono = verrokki.toString(Qt::ISODate);
    aikajono[10]=' ';
    QString kysymys = QString("select nimi, raide, aika, ilmoitus,nopeus "
                              "from veturiloki natural join liikennepaikka "
                              "where junanro=\"%1\" and aika>\"%2\" ")
            .arg(junanumero).arg(aikajono);

    QString teksti = QString("<html><body><h1>Kulkutiedot junasta %1</h1>").arg(junanumero);
    teksti.append("<table width=100%><tr><th>Liikennepaikka</th><th>Raide</th><th>Kello</th><th>Kulunut</th><th>Nopeus</th><th>Tapahtuma</th></tr>\n");

    QDateTime alkuaika;

    QSqlQuery kysely(kysymys);
    int rivi = 1;
    while( kysely.next())
    {
        QString liikennepaikka = kysely.value(0).toString();
        int raide = kysely.value(1).toInt();
        QDateTime aika = kysely.value(2).toDateTime();
        QString ilmoitus = kysely.value(3).toString();
        int nopeus = kysely.value(4).toInt();

        if( alkuaika.isNull())
            alkuaika = aika;

        QTime kulunut = QTime(0,0).addSecs( alkuaika.secsTo(aika)   );


        if( rivi++ % 2 )
            teksti.append("<tr>");
        else
            teksti.append("<tr class=varjo>");

        QString tapaus;
        if( ilmoitus == "P")
            tapaus = "<td class=saapuu>Pysähtyy</td>";
        else if( ilmoitus == "L")
            tapaus = "<td>Lähtee</td>";
        else if( ilmoitus == "S")
            tapaus = "<td class=saapuu>Saapuu</saapuu>";
        else
            tapaus = "<td> </td>";

        teksti.append( QString("<td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5 km/h</td>%6</tr>\n ")
                       .arg(liikennepaikka).arg(raide,3,10,QChar('0')).arg( aika.time().toString() )
                       .arg(kulunut.toString()  ).arg(nopeus).arg(tapaus) );
    }

    teksti.append("</table></body></html>");

    QString tyyli = " * { font-family: \"Helvetica\"; } "
            "h1  { font-size: 16px; background-color: yellow; }  "
            "th { font-weight: normal; background-color: lightgray }  "
            "tr.varjo { background-color: lightgray; } "
            "a  { font-size: 14px; color: black; text-decoration:none;  }"
            "td { font-size: 10px; } "
            "td.saapuu { background-color: red; } ";
    clear();
    document()->setDefaultStyleSheet(tyyli);
    setHtml(teksti);
}
