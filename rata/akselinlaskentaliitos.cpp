#include "akselinlaskentaliitos.h"
#include "kiskonpaa.h"


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
        ekaLaite_->lahetaViesti( ekaPaa_->toinenPaa()->kiskonpaikka()  );
        return tokaPaa_;
    }
    else
    {
        tokaLaite_->lahetaViesti(tokaPaa_->toinenPaa()->kiskonpaikka() );
        return ekaPaa_;
    }
}

void AkselinlaskentaLiitos::lisaaPaa(Kiskonpaa *kiskonpaa, int raidetunnus)
{
    // Akselinlaskentaa varten liitoksella on kaksi ratalaitteen tunnusta:
    // raidetunnus ilmaisee aina raiteen, jolta akseli lasketaan ulos
    if( ekaPaa_ )
    {
        ekaPaa_ = kiskonpaa;
        ekaLaite_ = new Ratalaite( Ratalaite::muodostaLaitetunnus(raidetunnus, 0xF) );
    }
    else
    {
        tokaPaa_ = kiskonpaa;
        tokaLaite_ = new Ratalaite( Ratalaite::muodostaLaitetunnus(raidetunnus, 0xF));
    }
}
