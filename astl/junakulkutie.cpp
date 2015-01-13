#include "junakulkutie.h"
#include "opastin.h"

#include "suoranraiteenpaa.h"
#include "linjasuojastus.h"

JunaKulkutie::JunaKulkutie(RaideTieto *mista, RaideTieto *minne)
    : Kulkutie(mista, minne),
      purkamisenAjastin_(0)
{

}

JunaKulkutie::~JunaKulkutie()
{

}

bool JunaKulkutie::PeruKulkutie()
{
    // Alkuehdot
    if( tila()!= Ratapiha::KULKUTIE_LUKITAAN && tila()!=Ratapiha::KULKUTIE_VALMIS
            && tila()!=Ratapiha::KULKUTIE_PERUSEHDOT)
        return false;

    if( tila() == Ratapiha::KULKUTIE_VALMIS)
    {
        // Valmiissa kulkutiessä ei saa olla junaa lähestymismatkalla (1200 m)
        int matka = 0;
        RaiteenPaa *paa = valmisKulkutie_.first()->liitettyPaa();

        while( paa && (matka < 1200))
        {
            if( paa->raide()->vapaanaOlo() != Ratapiha::RAIDE_VAPAA)
                return false;      // Raide on varattu
            matka += paa->raide()->pituus();
            paa = paa->seuraavaRaiteenpaa();
        }
    }

    // Kulkutie saadaan purkaa
    foreach (RaiteenPaa* paa, valmisKulkutie_)
    {
        paa->raide()->vapautaKulkutielta(this);

        // Kulkutiehen kuuluvat opasteet SEIS-asentoon
        if( tila() == Ratapiha::KULKUTIE_VALMIS && paa->liitettyPaa() && paa->liitettyPaa()->opastin() &&
                paa->liitettyPaa()->opastin()->opaste() != Ratapiha::OPASTE_SEIS)
            paa->liitettyPaa()->opastin()->asetaOpaste(Ratapiha::OPASTE_SEIS);
    }
    valmisKulkutie_.clear();    // Tyhjennetään kulkutie-elementtien lista
    return true;

}

void JunaKulkutie::valvoKulkutie(int kellonaika)
{
    // Jos purkuviive valmistuu
    if(purkamisenAjastin_ && purkamisenAjastin_ < kellonaika)
    {
        // Puretaan kulkutie
        // Kulkutie saadaan purkaa
        foreach (RaiteenPaa* paa, valmisKulkutie_)
        {
            paa->raide()->vapautaKulkutielta(this);
        }
        valmisKulkutie_.clear();    // Tyhjennetään kulkutie-elementtien lista
        return;
    }

    bool kaikkiLukittu = true;
    foreach (RaiteenPaa* paa, valmisKulkutie_)
    {
        if( paa->raide()->onkoLukittuKulkutielle() != Ratapiha::ELEMENTTI_LUKITTU)
            kaikkiLukittu = false;
    }

    if( tila() == Ratapiha::KULKUTIE_LUKITAAN && kaikkiLukittu)
    {
        // Nyt kulkutie on viimein valmis
        tila_ = Ratapiha::KULKUTIE_VALMIS;
        laitaVarit();
    }
    else if(( tila()==Ratapiha::KULKUTIE_KAYTOSSA || tila()==Ratapiha::KULKUTIE_VALMIS ) && !kaikkiLukittu )
    {
        // Jos valvotussa kulkutiessä elementit eivät pysy oikein lukittuina (syynä vaikka aukiajo), on
        // välittömänä seurauksena kulkutien siirtyminen vikatilaan
        kulkutieVikatilaan();
    }

}

bool JunaKulkutie::PuraKulkutie(int kellonaika)
{
    // Purkaa kulkutien hätävaraisesti, vaikka se olisi vikatilassa tai osittain tai kokonaan käytössä

    foreach (RaiteenPaa *paa, valmisKulkutie_)
    {
        if( paa->liitettyPaa() && paa->liitettyPaa()->opastin())
            paa->liitettyPaa()->opastin()->asetaOpaste(Ratapiha::OPASTE_SEIS);
    }
    purkamisenAjastin_ = kellonaika + 60;
    return true;
}

void JunaKulkutie::raideVarautuu(RaideTieto *raide)
{
    // Jos purku käynnissä..
    if( purkamisenAjastin_)
    {
        if( tila() == Ratapiha::KULKUTIE_VALMIS && raide == valmisKulkutie_.first()->raide())
        {
            // Jos juna ajoi purettavalle kulkutielle, annetaan sen edetä kulkutiellä ja
            // purkaa kulkutie automaattisesti
            tila_ = Ratapiha::KULKUTIE_VALMIS;
            purkamisenAjastin_ = 0;
        }
    }

    // Jos kulkutie oli VALMIS-tilassa, siirtyy se KÄYTÖSSÄ-tilaan
    if( tila() == Ratapiha::KULKUTIE_VALMIS)
        tila_ = Ratapiha::KULKUTIE_KAYTOSSA;

    // Etsitään kyseinen raiteenpää
    // Kulkutien tulee olla varautunut tähän raiteeseen saakka
    foreach (RaiteenPaa *paa, valmisKulkutie_)
    {
        if( paa->raide() == raide )
        {
            // Löydettiin varautunut raide
            // Mahdollinen opaste näyttämään SEIS
            if( paa->liitettyPaa() && paa->liitettyPaa()->opastin())
                paa->liitettyPaa()->opastin()->asetaOpaste(Ratapiha::OPASTE_SEIS);
            break;
        }
        // Valvotaan varautumisen järjestystä sillä, että kaikkien tätä edeltävien elementtien
        // pitää olla myös varattuja
        if( paa->raide()->vapaanaOlo() != Ratapiha::RAIDE_VARATTU)
            kulkutieVikatilaan();

    }

}

void JunaKulkutie::raideVapautuu(RaideTieto *raide)
{
    if( tila() == Ratapiha::KULKUTIE_KAYTOSSA)
    {

        // Jos kulkutiessä ensimmäisenä oleva raide vapautuu, niin silloin se puretaan kulkutieltä
        if( raide == valmisKulkutie_.first()->raide())
        {
            raide->vapautaKulkutielta(this);
            valmisKulkutie_.removeFirst();
        }
        else
        {
            // Muuten vapautumisen suunta on väärä, ja kulkutie merkitään vikatilaan
            kulkutieVikatilaan();
        }

        // Jos juna on kokonaan maaliraiteella, puretaan kulkutie
        if( valmisKulkutie_.count() == 1)
        {
            valmisKulkutie_.first()->raide()->vapautaKulkutielta(this);
            valmisKulkutie_.removeFirst();
        }
    }
}

Ratapiha::KulkutieTila JunaKulkutie::tila() const
{
    if( purkamisenAjastin_)
        return Ratapiha::KULKUTIE_PURETAAN;
    else
        return tila_;
}

bool JunaKulkutie::alkuEhdot(RaiteenPaa *paa)
{
    // Opastimen pitää pystyä näyttämään AJA-opaste
    Opastin *opastin = paa->opastin();

    if( !opastin)
        return false;

    return (opastin->tyyppitieto() & Opastin::VIHREA );
}

bool JunaKulkutie::kulkutieEhdot(RaiteenPaa *paa)
{
    if( paa->kulkutieLajit() == RaiteenPaa::VAINVAIHTO ||
            paa->liitettyPaa()->kulkutieLajit() == RaiteenPaa::VAINVAIHTO)
        return false; // Ei kelpaa junakulkutielle

    return true;
}

bool JunaKulkutie::loppuEhdot(RaiteenPaa *maaliPaa)
{
    // Jos päättyy suojastukseen, pitää sen voida suojastaa
    SuoranRaiteenPaa* spaa = dynamic_cast<SuoranRaiteenPaa*>(maaliPaa);
    if( spaa )
    {
        if( spaa->suojastus() && !spaa->suojastus()->voikoSuojastaa(spaa) )
            return false;
    }

    // Päätyttävä raidepuskuriin tai opastimeen, joka voi
    // näyttää punaista
    if( !maaliPaa->liitettyPaa() )
        return true;

    Opastin *opastin = maaliPaa->opastin();
    if( opastin && ( opastin->tyyppitieto() & Opastin::PUNAINEN ) )
            return true;

    return false;
}

void JunaKulkutie::laitaVarit()
{
    bool hidas = false;
    Opastin *paaOpastin = 0;
    foreach (RaiteenPaa *paa, valmisKulkutie_)
    {
        if( paa->onkoHidas() || paa->liitettyPaa()->onkoHidas())
            hidas = true;

        Opastin *opastin = paa->liitettyPaa()->opastin();
        if( opastin )
        {
            if( !(opastin->tyyppitieto() & Opastin::VIHREA ))
            {
                // Tämä on raideopastin
                if( paa->raide() != minne())
                    opastin->asetaOpaste( Ratapiha::OPASTE_AJAVAROVASTI);
            }
            else
            {
                if(paaOpastin)
                {
                    // Välissä oleva opastin
                    if( hidas)
                        paaOpastin->asetaOpaste(Ratapiha::OPASTE_AJASN);
                    else
                        paaOpastin->asetaOpaste(Ratapiha::OPASTE_AJA);
                }
                hidas = false;
                paaOpastin = opastin;
            }

        }
    }
    // Nyt ollaan aivan lopussa, laitetaan vielä viimeinen opastin
    if(paaOpastin)
    {
        // Välissä oleva opastin
        if( hidas)
            paaOpastin->asetaOpaste(Ratapiha::OPASTE_AJASN);
        else
            paaOpastin->asetaOpaste(Ratapiha::OPASTE_AJA);
    }
    // Nyt on värejä
}

void JunaKulkutie::kulkutieLukitaan()
{
    SuoranRaiteenPaa *spaa = dynamic_cast<SuoranRaiteenPaa*>(valmisKulkutie_.last()->aktiivinenVastapaa() );
    if( spaa && spaa->suojastus())
        spaa->suojastus()->suojasta(spaa);
}

