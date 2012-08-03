/**************************************************************************
**  aikataulunselausikkuna.h
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
**  AikataulunSelausIkkuna  3.8.2012
**************************************************************************/

#ifndef AIKATAULUNSELAUSIKKUNA_H
#define AIKATAULUNSELAUSIKKUNA_H

#include <QMainWindow>
#include <QComboBox>

#include "aikatauluselaaja.h"

class AikataulunSelausIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    explicit AikataulunSelausIkkuna(QWidget *parent = 0);
    
signals:
    
public slots:
    void haeAsemaAikataulu(int indeksi);


protected:
    void luoTyokalurivi();

    AikatauluSelaaja* selaaja_;
    QComboBox* asemaValintaCombo_;
};

#endif // AIKATAULUNSELAUSIKKUNA_H
