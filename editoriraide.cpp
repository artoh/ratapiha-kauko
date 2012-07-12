#include "editoriraide.h"

#include <QSqlQuery>

EditoriRaide::EditoriRaide(const QString &liikennepaikka, int raide, int raideid, const QString &raidetila, const QString &etelatila, const QString& pohjoistila)
    : liikennepaikka_(liikennepaikka), raide_(raide), raideid_(raideid)
{

    etelainen_ = new RaiteenPaa(etelatila);
    pohjoinen_ = new RaiteenPaa(pohjoistila);

    sahkoistetty_ = !raidetila.contains("SäEi");
    valvottu_ = !raidetila.contains("ValvEi");
}


EditoriRaide::~EditoriRaide()
{
    delete etelainen_;
    delete pohjoinen_;
}

void EditoriRaide::asetaMaareet(bool sahkoistetty, bool valvottu)
{
    sahkoistetty_ = sahkoistetty;
    valvottu_ = valvottu;
    talleta();
}

void EditoriRaide::talleta()
{
    QSqlQuery kysely;

    QString raidetila;
    if( !sahkoistetty_)
        raidetila.append("SäEi ");
    if( !valvottu_)
        raidetila.append("ValvEi ");

    kysely.exec( QString("update raide set tila_raide=\"%4\",tila_etela=\"%1\",tila_pohjoinen=\"%2\" where raideid=%3").arg(etelainen()->tilaTieto())
                 .arg(pohjoinen()->tilaTieto()).arg(raideid_).arg(raidetila));

}
