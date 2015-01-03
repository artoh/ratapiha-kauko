#include "junakulkutie.h"
#include "opastin.h"

JunaKulkutie::JunaKulkutie(RaideTieto *mista, RaideTieto *minne)
    : Kulkutie(mista, minne)
{

}

JunaKulkutie::~JunaKulkutie()
{

}

bool JunaKulkutie::PuraKulkutie()
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

        if( tila() == Ratapiha::KULKUTIE_VALMIS && paa->liitettyPaa() && paa->liitettyPaa()->opastin() &&
                paa->liitettyPaa()->opastin()->opaste() != Ratapiha::OPASTE_SEIS)
            paa->liitettyPaa()->opastin()->asetaOpaste(Ratapiha::OPASTE_SEIS);
    }
    tila_ = Ratapiha::KULKUTIE_EIKULKUTIETA;
    return true;

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

