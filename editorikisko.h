/**************************************************************************
** Ratapiha - railway yard simulation toy
** (c) Arto Hyvättinen 2011
** Gnu Public Licence 2 - No Warranty !!!
** 14.6.2011
**************************************************************************/

#ifndef EDITORIKISKO_H
#define EDITORIKISKO_H

#include "kisko.h"
#include <QLineF>
#include <QColor>

class EditoriScene;
class EditoriRaide;

class EditoriKisko : public Kisko
{
public:
    EditoriKisko(EditoriScene* skene, const QLineF& viiva, int kiskoid=0, const QString& liikennepaikka=QString(), int raide = 0,
                 const QString& kiskodata = QString(), int sn=0);


    enum { Type = UserType + 2 } ;

    enum Kulkutietyypit { Ensisijainen, Toissijainen, VainVaihto } ;

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int type() const { return Type; }

    void valitse(bool onko=true);
    int sn() const { return sn_; }
    Kulkutietyypit kulkutietyypit() const { return kulkutietyypit_; }

    void asetaLiikennepaikka(const QString& lyhenne);
    void asetaRaide(int raide);
    void asetaSn(int sn);
    /** Asettaa raiteen ja liikenenpaikan myös vaihdekavereille*/
    void levitaRaiteenAsetus();

    void asetaRaiteenValintoja(Kisko::Laituri laituri, bool naytaRaideNumero = false,
                               bool naytaJunaNumero = false, int sn = 0,
                               Kulkutietyypit kulkutietyypit = Ensisijainen,
                               bool esiopastinEtela = false,
                               bool esiopastinPohjoinen = false);

    void talletaKisko();

    void tarkistaPaanTyypit();

    static QColor nopeusVari(int nopeus);

    EditoriRaide* raidePointteri() { return raidePtr_; }

    bool esiopastinEtela() const { return esiopastinEtela_; }
    bool esiopastinPohjoinen() const { return esiopastinPohjoinen_; }


protected:
    // Tarkastetaan päiden tyypit ja kaverit tekee myös saman
    void paidenTarkistusToimet(bool tallenna=false);
    QString kiskoTietoTalletettavaksi() const;

    int sn_;
    Kulkutietyypit kulkutietyypit_;
    EditoriScene* skene_;
    bool esiopastinEtela_;
    bool esiopastinPohjoinen_;
    bool valittu_;

    EditoriRaide* raidePtr_;
};

#endif // EDITORIKISKO_H
