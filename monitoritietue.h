#ifndef MONITORITIETUE_H
#define MONITORITIETUE_H

#include <QTime>

class MonitoriTietue
{
public:
    MonitoriTietue(QTime lahtoaika, QString junatunnus, QString minne, int raide);

    QTime lahtoaika() const { return lahtoaika_; }
    QString junatunnus() const { return junatunnus_; }
    QString junatunnusNaytettava() const;
    QString minne() const { return minne_; }
    int raide() const { return raide_; }

protected:
    QTime lahtoaika_;
    QString junatunnus_;
    QString minne_;
    int raide_;
};

#endif // MONITORITIETUE_H
