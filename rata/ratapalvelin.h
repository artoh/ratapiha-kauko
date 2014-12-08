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


#ifndef RATAPALVELIN_H
#define RATAPALVELIN_H

#include <QtNetwork/QTcpServer>

class RataScene;

class RataPalvelin : public QTcpServer
{
    Q_OBJECT
public:
    RataPalvelin(RataScene *skene);

signals:

private slots:
    void yhteys();

protected:
//    void incomingConnection(int handle);

private:
    RataScene *skene_;
};

#endif // RATAPALVELIN_H
