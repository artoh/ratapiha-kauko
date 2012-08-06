/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
5.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#ifndef REITITMODEL_H
#define REITITMODEL_H

#include "reittisuunnittelutieto.h"

#include <QAbstractTableModel>
#include <QList>
#include <QMap>

class ReititModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit ReititModel(QObject *parent = 0);

    enum { Liikennepaikka = 0, Raide = 1, Aika = 2, Pysahtyy = 3, Tapahtuma = 4, Suunta = 5 } ;

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    bool setData(const QModelIndex &index, const QVariant &value, int role);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QMap<QString,QString>* liikennepaikkalista() { return &liikennepaikat_; }

    QString reittiTunnus() const { return reittiTunnus_; }

signals:
    void muokattu(bool onko);

public slots:
    void valitseReitti( const QString& reittitunnus);
    void uusiReitti();
    void peruMuokkaukset();
    void lisaaPysahdys( const QString& liikennepaikka);
    void poistaRivi(int rivi);
    void reitistaKopio();
    void tallenna( const QString& uusitunnus);


protected:
    QList<ReittiSuunnitteluTieto> tiedot_;
    QString reittiTunnus_;
    QMap<QString, QString> liikennepaikat_; // Tätä käytetään liikennepaikkojen nimien näyttämiseen
};

#endif // REITITMODEL_H
