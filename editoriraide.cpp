#include "editoriraide.h"

#include <QSqlQuery>

EditoriRaide::EditoriRaide(const QString &liikennepaikka, int raide, int raideid, const QString &raidetila, const QString &etelatila, const QString& pohjoistila)
    : liikennepaikka_(liikennepaikka), raide_(raide), raideid_(raideid)
{

    etelainen_ = new EditoriRaiteenPaa(etelatila);
    pohjoinen_ = new EditoriRaiteenPaa(pohjoistila);
}


EditoriRaide::~EditoriRaide()
{
    delete etelainen_;
    delete pohjoinen_;
}

void EditoriRaide::talleta()
{
    QSqlQuery kysely;

    kysely.exec( QString("update raide set tila_etela=\"%1\",tila_pohjoinen=\"%2\" where raideid=%3").arg(etelainen()->tilaTieto())
                 .arg(pohjoinen()->tilaTieto()).arg(raideid_));

}
