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


#ifndef ASETINLAITEPANEELI_H
#define ASETINLAITEPANEELI_H

#include <QWidget>

namespace Ui {
class AsetinlaitePaneeli;
}

class AsetinlaitePaneeli : public QWidget
{
    Q_OBJECT

public:
    explicit AsetinlaitePaneeli(QWidget *parent = 0);
    ~AsetinlaitePaneeli();

public slots:
    void ajanPaivitys(int simulaatioAika);
    void yhdistettyRataan(bool onko);
    void kulkutiemaaraPaivitys(int kulkuteita);
    void asiakasmaaraPaivitys(int asiakkaita);

private slots:
    void aslKomento();

private:
    Ui::AsetinlaitePaneeli *ui;
};

#endif // ASETINLAITEPANEELI_H
