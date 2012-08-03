/**************************************************************************
**  graafinenaikatauluscene.h
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
**  GraafinenAikatauluScene  3.8.2012
**************************************************************************/

#ifndef GRAAFINENAIKATAULUSCENE_H
#define GRAAFINENAIKATAULUSCENE_H

#include <QGraphicsScene>
#include <QDateTime>
#include <QGraphicsLineItem>

class GraafinenAikatauluScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit GraafinenAikatauluScene(QObject *parent = 0);


    int taulu() const { return taulu_; }
    void lataaTaulu(int taulu);
    void asetaAikavali(int mista, int mihin);

    qreal yKmluvusta(qreal kmluku) const;
    qreal xAjasta(QTime aika) const;
    int alkaaTunnista() const { return tuntiAlkaa_; }
    int loppuuTuntiin() const { return tuntiLoppuu_; }
    qreal maxX() const { return maxX_; }
    int ruudukonleveys() const { return ruudukonLeveys_; }
    QString tauluNimi() const { return taulunimi_ ; }

    static const int JUNANRODATAKENTTA = 1;
    static const int ASEMANTUNNUSKENTTA = 2;

signals:
    
public slots:
    void paivitaKelloViiva(const QDateTime& aika);
    

protected:
    void lataaRuudukko();
    void lataaAikataulut();

    qreal pieninKmluku_;
    qreal isoinKmluku_;

    qreal maxX_;

    int taulu_;
    QString taulunimi_;

    int ruudukonLeveys_;
    int tuntiAlkaa_;
    int tuntiLoppuu_;

    QGraphicsLineItem* kelloViiva_;

};

#endif // GRAAFINENAIKATAULUSCENE_H
