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
    selattavanTyyppi_(Tyhja)
{
    setOpenLinks(false);
    connect( this, SIGNAL(anchorClicked(QUrl)), this, SLOT(klikattu(QUrl)));
}


void AikatauluSelaaja::haeAsemaAikataulu(const QString &liikennepaikka)
{
    QString teksti = "<html><body>";


    // Seuraavaksi yksinkertainen sqlkysymys, joka kertoo asemalta kulkevat junat

    QString kysymys = QString("select juna.junanro as Juna, addtime(lahtee,taulu.lahtoaika) as Kello, "
    "minnelkp.nimi as Minne,  addtime(lahtee, minne.lahtoaika) as Perilla, taulu.raide as Laituri, "
    "taulu.pysahtyy as pysahtyy, taulu.tapahtuma as tapahtuma, minnelkp.liikennepaikka as lyhenne "
    "from juna, aikataulu as taulu, aikataulu as minne, liikennepaikka as minnelkp "
    " where taulu.reitti = juna.reitti and taulu.reitti = minne.reitti and "
    " minnelkp.liikennepaikka = minne.liikennepaikka "
    " and minne.tapahtuma=\"S\" and taulu.tapahtuma != \"O\" "
    " and taulu.liikennepaikka=\"%1\" order by kello ").arg( liikennepaikka );


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
    QSqlQuery junaKysely( kysymys );
    while( junaKysely.next())
    {
        rivi++;
        QString juna = junaKysely.value(0).toString();
        QTime kello = junaKysely.value(1).toTime();
        QString minne = junaKysely.value(2).toString();
        QTime perilla = junaKysely.value(3).toTime();
        int raide = junaKysely.value(4).toInt();
        int pysahtyySekuntia = junaKysely.value(5).toInt();
        QString tapahtuma = junaKysely.value(6).toString();
        QString minnelyhenne = junaKysely.value(7).toString();

        QTime saapuu;
        QTime lahtee;

        if( tapahtuma == "S")
        {
            saapuu = kello;
        }
        else if( tapahtuma == "P")
        {
            saapuu = kello.addSecs( 0 - pysahtyySekuntia );
            lahtee = kello;
        }
        else if( tapahtuma == "L" )
        {
            lahtee = kello;
        }

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

        // Näytetään vain raiteen viimeinen numero (paitsi Helsingissä kaksi)
        if( liikennepaikka == "Hki")
            raide = raide % 100;
        else
            raide = raide % 10;

        QString korostettuJunaTunnus;
        if( juna.startsWith('H') && juna.length() > 3 && juna[1].isLetter() )
        {
            korostettuJunaTunnus = QString("%1 <b>%2</b> %3")
                    .arg(juna.mid(0,1)).arg(juna.mid(1,1)).arg(juna.mid(2));
        }
        else
            korostettuJunaTunnus = juna;


        teksti.append( QString("</td><td class=juna><a href=\"file:J%1\">%6</a> </td><td class=minne><a href=\"file:A%5\">%2</a></td>"
                               "<td class=perilla>%3</td><td class=raide>%4</td></tr>\n")
                       .arg(juna).arg(minne).arg( perilla.toString("hh:mm")).arg(raide).arg(minnelyhenne).arg(korostettuJunaTunnus)   );

    }
    teksti.append("</table></body></html>");

    QString tyyli = " * { font-family: \"Helvetica\"; } "
            "h1  { font-size: 16px; background-color: yellow; }  "
            "th { font-weight: normal; background-color: lightgray }  "
            "tr.varjo { background-color: lightgray; } "
            "td.lahtee { font-size: 14px; font-weight: bold; } "
            "td.minne { font-size: 14px; font-weight: bold; } "
            "td.raide { font-size: 14px; } "
            "td.tunti { background-color: darkgray; font-size:14px; } "
            "a  { color: black; text-decoration:none;  }";
    clear();
    document()->setDefaultStyleSheet(tyyli);
    setHtml(teksti);

    selattavanTyyppi_ = Asema;
    selattavanTunnus_ = liikennepaikka;
    emit naytetaanAsema(liikennepaikka);
}

void AikatauluSelaaja::haeJunaAikataulu(const QString &juna)
{
    if( juna.startsWith("(LOKI)"))
    {
        naytaLoki( juna.mid(6));
        return;
    }


    QString kysymys = QString("select nimi, addtime(lahtee,lahtoaika) as aika, pysahtyy, raide, tapahtuma, aikataulu.liikennepaikka "
                              "from juna,aikataulu,liikennepaikka "
                              "where juna.junanro=\"%1\" and "
                              "juna.reitti = aikataulu.reitti and aikataulu.liikennepaikka=liikennepaikka.liikennepaikka "
                              "order by aika").arg(juna);


    QString teksti = QString("<html><body><h1>%1</h1>").arg(juna);

    teksti.append("<table width=100%><tr><th>Liikennepaikka</th><th>Saapuu</th><th>Lähtee</th><th>Raide</th></tr>\n");

    QSqlQuery kysely(kysymys);
    int rivi = 0;
    while( kysely.next())
    {
        rivi++;
        QString liikennepaikka = kysely.value(0).toString();
        QTime kello = kysely.value(1).toTime();
        int pysahtyySekuntia = kysely.value(2).toInt();
        int raide = kysely.value(3).toInt();
        QString tapahtuma = kysely.value(4).toString();
        QString liikennepaikkaLyhenne = kysely.value(5).toString();

        // Näytetään vain raiteen viimeinen numero (paitsi Helsingissä kaksi)
        if( liikennepaikka == "Hki")
            raide = raide % 100;
        else
            raide = raide % 10;

        QTime saapuu;
        QTime lahtee;
        if( tapahtuma == "S")
        {
            saapuu = kello;
        }
        else if( tapahtuma == "P")
        {
            saapuu = kello.addSecs( 0 - pysahtyySekuntia );
            lahtee = kello;
        }
        else if( tapahtuma == "L"  || tapahtuma == "O")
        {
            lahtee = kello;
        }
        if( rivi % 2 )
            teksti.append("<tr>");
        else
            teksti.append("<tr class=varjo>");

        teksti.append( QString("<td><a href=\"file:A%1\">%2</td><td>").arg(liikennepaikkaLyhenne).arg(liikennepaikka));
        if( saapuu.isValid())
            teksti.append( saapuu.toString("hh.mm"));
        teksti.append("</td><td>");
        if( tapahtuma == "O") // Ohitusaika sulkeisiin!
            teksti.append("(");
        if( lahtee.isValid())
            teksti.append( lahtee.toString("hh.mm"));
        if( tapahtuma == "O")
            teksti.append(")");
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
            tapaus = "Pysähtyy";
        else if( ilmoitus == "L")
            tapaus = "Lähtee";
        else if( ilmoitus == "S")
            tapaus = "Saapuu";

        teksti.append( QString("<td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5 km/h</td><td>%6</td></tr>\n ")
                       .arg(liikennepaikka).arg(raide,3,10,QChar('0')).arg( aika.time().toString() )
                       .arg(kulunut.toString()  ).arg(nopeus).arg(tapaus) );
    }

    teksti.append("</table></body></html>");

    QString tyyli = " * { font-family: \"Helvetica\"; } "
            "h1  { font-size: 16px; background-color: yellow; }  "
            "th { font-weight: normal; background-color: lightgray }  "
            "tr.varjo { background-color: lightgray; } "
            "a  { font-size: 14px; color: black; text-decoration:none;  }"
            "td { font-size: 10px; } ";
    clear();
    document()->setDefaultStyleSheet(tyyli);
    setHtml(teksti);
}
