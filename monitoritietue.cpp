#include "monitoritietue.h"

MonitoriTietue::MonitoriTietue(QTime lahtoaika, QString junatunnus, QString minne, int raide)
    : lahtoaika_(lahtoaika), junatunnus_(junatunnus), minne_(minne), raide_(raide)
{
}

QString MonitoriTietue::junatunnusNaytettava() const
{
    if( junatunnus_.startsWith('H'))
        return junatunnus_.mid(1,1);
    else
        return junatunnus_;
}
