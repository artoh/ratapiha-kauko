#include "junakulkutie.h"
#include "opastin.h"

JunaKulkutie::JunaKulkutie(RaideTieto *mista, RaideTieto *minne)
    : Kulkutie(mista, minne)
{

}

JunaKulkutie::~JunaKulkutie()
{

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

