/**************************************************************************
Ratapiha - railway construction and simulation toy

(c) Arto Hyvättinen 2012
4.8.2012

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


**************************************************************************/

#ifndef AIKATAULUMUOKKAAJA_H
#define AIKATAULUMUOKKAAJA_H


#include "aikatauluselaaja.h"

#include <QWidget>
#include <QTimeEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>

class AikatauluMuokkaaja : public QWidget
{
    Q_OBJECT
public:
    explicit AikatauluMuokkaaja(QWidget *parent = 0);

    AikatauluSelaaja* selaaja() { return selaaja_; }

signals:
    void junaPaivitetty(const QString& tunnus);

public slots:
    void valittuJuna(const QString& tunnus = QString());
    void katsoKelpaako();

    void eiValittuaJunaa();
    void lataaReitit();

    void muokkausValmis();
    void peruMuokkaus();
    void poistaJuna();
    void uusiJuna();

protected:
    QString junaTunnus_;

    QLineEdit* tunnusEdit_;
    QTimeEdit* aikaEdit_;
    QComboBox* reittiCombo_;
    AikatauluSelaaja* selaaja_;

    QPushButton* lisaaNappi_;
    QPushButton* poistaNappi_;
    QPushButton* valmisNappi_;
    QPushButton* peruNappi_;

};

#endif // AIKATAULUMUOKKAAJA_H
