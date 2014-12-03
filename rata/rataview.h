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


#ifndef RATAVIEW_H
#define RATAVIEW_H

#include <QGraphicsView>

#include "ratascene.h"

class RataView : public QGraphicsView
{
    Q_OBJECT
public:
    RataView(RataScene* skene);

protected:

    void wheelEvent(QWheelEvent *event);
    bool rullaSkaalaa_;

signals:

public slots:

};

#endif // RATAVIEW_H
