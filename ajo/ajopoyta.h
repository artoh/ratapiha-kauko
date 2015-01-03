/**************************************************************************
**
**  Ratapiha
**
**  Copyright (c) 2012-2015 Arto Hyvättinen
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
**
**************************************************************************/


#ifndef AJOPOYTA_H
#define AJOPOYTA_H

#include <QWidget>

#include <QTcpSocket>

namespace Ui {
class AjoPoyta;
}

class AjoPoyta : public QWidget
{
    Q_OBJECT

public:
    explicit AjoPoyta(QWidget *parent = 0);
    ~AjoPoyta();

private slots:
    void paivita();

    void ajoPoytaYksi(bool onko);
    void valitseVeturi();
    void muutaNopeus(int nopeus);
    void pyydaTiedot();

private:
    Ui::AjoPoyta *ui;

    QTcpSocket soketti_;
};

#endif // AJOPOYTA_H
