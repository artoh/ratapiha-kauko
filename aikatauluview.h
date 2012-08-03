/**************************************************************************
**  aikatauluview.h
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
**  AikatauluView  3.8.2012
**************************************************************************/

#ifndef AIKATAULUVIEW_H
#define AIKATAULUVIEW_H

#include <QGraphicsView>

#include "graafinenaikatauluscene.h"

class AikatauluView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AikatauluView(GraafinenAikatauluScene* skene);
    
signals:
    
public slots:

protected:
    void wheelEvent(QWheelEvent *event);

    /** Käytetäänkö hiiren rullaa skaalaamiseen vai vierittäimiseen */
    bool rullaSkaalaa_;
    
};

#endif // AIKATAULUVIEW_H
