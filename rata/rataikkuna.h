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


#ifndef RATAIKKUNA_H
#define RATAIKKUNA_H

#include <QMainWindow>

#include <QtWidgets/QLabel>

#include "ratascene.h"

class RataView;

/**
 * @brief Rataa näyttävä ikkuna
 */
class RataIkkuna : public QMainWindow
{
    Q_OBJECT
public:
    RataIkkuna(RataScene *skene);

    RataScene* skene()  { return skene_; }

signals:

public slots:
    /**
     * @brief Päivittää statusrivin kellonaikaa
     * @param simulaatioAika Aika sekunteina nollahetkestä
     */
    void kellonPaivitys(int simulaatioAika);

    void naytaLiikennepaikka(const QString& liikennepaikka);

protected:
    void luoTilarivi();
    void luoTyokalurivi();
    void luoVaunuDock();

private:
    RataScene *skene_;
    RataView* view_;
    QLabel* aikaLabel_;

};

#endif // RATAIKKUNA_H
