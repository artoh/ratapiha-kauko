#include "kantaristeys.h"

KantaRisteys::KantaRisteys()
    : RaideTieto() ,
      paaA_(this),
      paaB_(this),
      paaC_(this),
      paaD_(this)
{

}

KantaRisteys::~KantaRisteys()
{

}

RaiteenPaa *KantaRisteys::raiteenPaa(int paaKirjain)
{
    if( paaKirjain == RaiteenPaa::A)
        return &paaA_;
    else if(paaKirjain == RaiteenPaa::B)
        return &paaB_;
    else if(paaKirjain == RaiteenPaa::C)
        return &paaC_;
    else if(paaKirjain == RaiteenPaa::D)
        return &paaD_;
    return 0;
}
