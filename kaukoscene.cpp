#include "kaukoscene.h"
#include "kaukokisko.h"

#include "raidetieto.h"

#include <QSqlQuery>
#include <QVariant>

#include <QDebug>
#include <QSqlError>

KaukoScene::KaukoScene(QObject *parent) :
    KiskoScene(parent), valkytys_(false)
{
    setBackgroundBrush( Qt::gray);
    paivitysTimer_ = new QTimer(this);
    connect( paivitysTimer_, SIGNAL(timeout()), this, SLOT(paivitys()));
}


bool KaukoScene::haeNakyma(int nakyma)
{
    paivitysTimer_->stop();
    clear();
    kiskot_.clear();

    // Hakee näkymän kiskot

    QSqlQuery nkys( QString("select kisko, liikennepaikka, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto from kisko where nakyma=%1").arg(nakyma) );

    while(nkys.next())
    {
        int kkid = nkys.value(0).toInt();
        QString liikennepaikka = nkys.value(1).toString();
        int raide = nkys.value(2).toInt();
        QLineF viiva( QLine( nkys.value(3).toInt(), 0-nkys.value(4).toInt(),
                             nkys.value(5).toInt(), 0-nkys.value(6).toInt())  );

        QString kiskodata = nkys.value(7).toString();

        KaukoKisko* kisko = new KaukoKisko(this, viiva, kkid, liikennepaikka, raide, kiskodata );

        kiskot_.insert(kkid, kisko);
    }


    // Luo raidetaulun
    raiteet_.clear();

    nkys.exec( QString("select kisko,raideid from kisko natural join raide where nakyma=%1").arg(nakyma) );
    while( nkys.next())
    {
        // Hakee tiedon kiskoihin liittyvistä raiteista
        int kiskoid = nkys.value(0).toInt();
        int raideid = nkys.value(1).toInt();

        // Luo puuttuvat raiteet - ne päivittyvät kyllä aikanaan...
        RaideTieto* rtieto = raiteet_.value(raideid, 0);
        if( !rtieto)
        {
            rtieto = new RaideTieto;
            raiteet_.insert(raideid, rtieto);
        }
        // Laittaa talteen raiteen pointterin, jotta saadaan raiteen tiedot
        KaukoKisko* kisko = kiskot_.value(kiskoid,0);
        if( kisko )
            kisko->asetaRaide(rtieto);
    }



    // Nyt on näkymä haettu...
    nakyma_ = nakyma;

    haeTekstit();
    paivitys(); // Haetaan heti päivitettävät tiedot
    paivitysTimer_->start(500);    // Päivitetään puolen sekunnin välein

    return true;
}

void KaukoScene::paivitys()
{
    QSqlQuery kysely( QString("select raideid,akseleita,junanro,tila_raide,tila_etela,tila_pohjoinen,kulkutie from kisko natural join raide where nakyma=%1 group by raideid").arg(nakyma_));
    while( kysely.next())
    {
        int raideid = kysely.value(0).toInt();
        int akseleita = kysely.value(1).toInt();
        QString junanumero = kysely.value(2).toString();
        QString raidetila = kysely.value(3).toString();
        QString etelatila = kysely.value(4).toString();
        QString pohjoistila = kysely.value(5).toString();
        QString kulkutietila = kysely.value(6).toString();

        // Päivitetään tiedot raiteen kohdalle ...
        RaideTieto* tieto = raiteet_.value(raideid);
        if( tieto  )
            tieto->paivita(akseleita, junanumero, raidetila, etelatila, pohjoistila,kulkutietila);

    }
    // Välkky eri päin kuin edellisellä kerralla...
    valkytys_ = !valkytys_;
    // Sitten invalitoidaan
    invalidate( sceneRect());
}

