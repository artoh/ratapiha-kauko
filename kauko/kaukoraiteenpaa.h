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


#ifndef KAUKORAITEENPAA_H
#define KAUKORAITEENPAA_H

#include <QString>

#include "ratapiha.h"

class KaukoRaiteenpaa
{
public:
    KaukoRaiteenpaa();

    void paivita(const QString tieto);

    Ratapiha::Opaste opaste() const { return opaste_; }
    Ratapiha::OpastinLaji opastinlaji() const { return opastinlaji_; }

private:
    Ratapiha::Opaste opaste_;
    Ratapiha::OpastinLaji opastinlaji_;

};

#endif // KAUKORAITEENPAA_H
