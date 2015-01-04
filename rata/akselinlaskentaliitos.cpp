#include "akselinlaskentaliitos.h"
#include "kiskonpaa.h"
#include "ratascene.h"
#include "akselinlaskentalaite.h"

AkselinlaskentaLiitos::AkselinlaskentaLiitos(int liitosId, int x, int y)
    : Suoraliitos(liitosId, x, y), ekaLaite_(0), tokaLaite_(0)
{

}

Kiskonpaa *AkselinlaskentaLiitos::siirrySeuraavalle(Kiskonpaa *mista)
{
    // Lähetettävä akselinlaskentaviesti on raiteen liitosasento
    // eli tämän kisko toisen pään asentotunnus
    if( mista == ekaPaa_)
    {
        ekaLaite_->komento(0x42);
        tokaLaite_->komento(0x41);
        return tokaPaa_;
    }
    else
    {
        tokaLaite_->komento(0x42);
        ekaLaite_->komento(0x41);
        return ekaPaa_;
    }
}

void AkselinlaskentaLiitos::lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus)
{
    // Akselinlaskentaa varten liitoksella on kaksi ratalaitteen tunnusta:
    // raidetunnus ilmaisee aina raiteen, jolta akseli lasketaan ulos
    if( ekaPaa_ )
    {
        tokaPaa_ = kiskonpaa;
        tokaLaite_ = Ratalaite::skene()->ratalaite( Ratalaite::muodostaLaitetunnus(raidetunnus, 0xf) );
        if( !tokaLaite_)
            tokaLaite_ = new AkselinLaskentaLaite(raidetunnus);
    }
    else
    {
        ekaPaa_ = kiskonpaa;
        ekaLaite_ = Ratalaite::skene()->ratalaite( Ratalaite::muodostaLaitetunnus(raidetunnus, 0xf) );
        if( !ekaLaite_)
            ekaLaite_ = new AkselinLaskentaLaite(raidetunnus);
    }
}
