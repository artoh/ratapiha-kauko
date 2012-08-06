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

#ifndef REITTIDIALOGI_H
#define REITTIDIALOGI_H

#include "ui_reittidialogi.h"

class ReititModel;

class ReittiDialogi : public QDialog, private Ui::ReittiDialogi
{
    Q_OBJECT

public:
    explicit ReittiDialogi(QWidget *parent = 0);

    void lataaReitit();

public slots:
    void lisaaPysays();
    void poistaPysays();
    void uusiReitti();
    void kopioiReitti();
    void haeReitti(const QString& reitti);
    void reittiaMuokattu(bool onko);
    void tallenna();
    void poistaReitti();

protected:
    void valintaVaihtuu();

    ReititModel* reittiModel_;
    QListWidgetItem* uudenReitinItem_;
};

#endif // REITTIDIALOGI_H
