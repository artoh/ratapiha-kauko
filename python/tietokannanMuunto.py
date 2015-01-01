# coding=utf-8


import math

class Kiskoliitos(object) :
    seuraavaId = 1

    TYHJA = 0
    RAIDEPUSKURI = 1
    LIITOS = 2
    AKSELINLASKENTA = 6
    VAIHDE = 0x8
    RISTEYSVAIHDE = 0x18
    RAIDERISTEYS = 0x30


    def __init__(self,x,y):

        self.x = x
        self.y = y

        self.kiskonpaat = []
        self.tila = Kiskoliitos.TYHJA

        self.nro = Kiskoliitos.seuraavaId
        Kiskoliitos.seuraavaId += 1


    def lisaapaa(self, kiskonpaa):
        for paa in self.kiskonpaat :
            if kiskonpaa.kisko.sn < paa.kisko.sn :
                kiskonpaa.kptieto |= Kiskonpaa.HIDAS  # Tämä kisko lähtee hitaammalta raiteelta
        self.kiskonpaat.append(kiskonpaa)
        kiskoja = len( self.kiskonpaat)
        if kiskoja == 1 :
            self.tila = Kiskoliitos.RAIDEPUSKURI
        elif kiskoja == 2 :
            if self.kiskonpaat[0].kisko.raide == self.kiskonpaat[1].kisko.raide :
                self.tila = Kiskoliitos.LIITOS
            else :
                self.tila = Kiskoliitos.AKSELINLASKENTA
        elif kiskoja == 3 :
            self.tila = Kiskoliitos.VAIHDE
        elif kiskoja == 4 :
            if self.kiskonpaat[0].kisko.raide.etelapaa.tila == Raiteenpaa.RAIDERISTEYS :
                self.tila = Kiskoliitos.RAIDERISTEYS
            else :
                self.tila = Kiskoliitos.RISTEYSVAIHDE



class Kiskonpaa(object) :
    HIDAS = 0x1

    E = 0x1
    A = 0xd
    B = 0x9
    P = 0x3
    C = 0xB
    D = 0xF


    def __init__(self, kisko, liitos, liitosasento):
        self.kisko = kisko
        self.liitos = liitos
        self.liitosasento = liitosasento
        self.raiteensulku = False
        if kisko.sn < 40 :      # Hidas kisko !
            self.kptieto = kisko.tieto & 0x6 | Kiskonpaa.HIDAS
        else :
            self.kptieto = kisko.tieto & 0x6

        # Kptieto: hidas 0x1 , kulkutie_toissijainen 0x2, kulkutie_vaihto 0x4

        self.liitos.lisaapaa(self) # Lisää itsensä listaan
        self.toinen = None # Toistaiseksi tyhjä
        self.opastin = None

    def seuraavaPaa(self):
        for paa in self.liitos.kiskonpaat :
            if paa != self and paa.liitosasento & 0x8 == 0 :
                # ei ole tämä pää eikä vastavaihteessa
                return paa.toinen
        # ei löytynyt
        return None



class Kisko(object) :
    KULKUTIE_TOISSIJAINEN = 0x2
    KULKUTIE_VAIHTO = 0x4
    LAITURI_VASEN = 0x10
    LAITURI_OIKEA = 0x8
    SILTA = 0x20

    def __init__(self, nro, eliitos, pliitos, tieto, sn, raide):
        self.nro = nro

        self.pituus = math.sqrt( (pliitos.y - eliitos.y)**2 + (pliitos.x-pliitos.x)**2)
        raide.pituus += self.pituus  # Lasketaan raiteen kokonaispituus

        # Vaihteen asentotieto eri kiskoilla sijoitetaan kiskonpäähän
        elaji = Kiskonpaa.P
        if "E+" in tieto :
            elaji = Kiskonpaa.D
        elif "E-" in tieto:
            elaji = Kiskonpaa.C

        plaji = Kiskonpaa.E
        if "P+" in tieto :
            plaji = Kiskonpaa.A
        elif "P-" in tieto :
            plaji = Kiskonpaa.B

        self.raide = raide
        self.sn = sn
        self.tieto = 0x00

        if "Lv" in tieto :
            self.tieto |= Kisko.LAITURI_VASEN
        elif "Lo" in tieto :
            self.tieto |= Kisko.LAITURI_OIKEA
        elif "Lm" in tieto :
            self.tieto |= Kisko.LAITURI_VASEN | Kisko.LAITURI_OIKEA

        if "Silta" in tieto :
            self.tieto |= Kisko.SILTA

        if "Kt" in tieto :
            self.tieto |= Kisko.KULKUTIE_TOISSIJAINEN
        elif "Kv" in tieto:
            self.tieto |= Kisko.KULKUTIE_VAIHTO

        self.etelapaa = Kiskonpaa(self,eliitos, elaji)
        self.pohjoispaa = Kiskonpaa(self, pliitos, plaji)

        self.etelapaa.kisko = self
        self.pohjoispaa.kisko = self

        self.etelapaa.toinen = self.pohjoispaa
        self.pohjoispaa.toinen = self.etelapaa

        # Opastimien kiskotietojen sijoittaminen, raiteensulkujen sijoittaminen
        if "E*" in tieto :
            if self.raide.etelapaa.opastin is not None :
                self.raide.etelapaa.opastin.kiskonpaahan(self.etelapaa)
            if self.raide.etelapaa.tila & Raiteenpaa.RAITEENSULKU :
                self.etelapaa.raiteensulku = True
        elif "EoE" in tieto:
            Opastin(self.raide.etelapaa, Opastin.EO).kiskonpaahan(self.etelapaa)
        if "P*" in tieto :
            if self.raide.pohjoispaa.opastin is not None:
                self.raide.pohjoispaa.opastin.kiskonpaahan(self.pohjoispaa)
            if self.raide.pohjoispaa.tila & Raiteenpaa.RAITEENSULKU :
                self.pohjoispaa.raiteensulku = True
        elif "EoP" in tieto:
            Opastin(self.raide.pohjoispaa, Opastin.EO).kiskonpaahan(self.pohjoispaa)




class Opastin(object) :
    P2 = 0x3    # Kaksivaloinen pääopastin
    P3 = 0x7    # Kolmivaloinen pääopastin
    EO = 0x48   # Esiopastin
    RO = 0x32   # Raideopastin
    LO = 0x4A   # Laituriopastin
    SO = 0x83   # Itsetoiminen suojausopastin
    KKEO = 0x100 # Kiinteästi kytketyn esiopastimen merkkibitti

    def __init__(self, raiteenpaa, tyyppi):
        self.raiteenpaa = raiteenpaa
        if tyyppi != Opastin.EO :  # Esiopastin toimii vähän eri tavalla...
            raiteenpaa.opastin = self
        self.kiskonpaa = None  # Toistaiseksi
        self.tyyppi = tyyppi

        # Jos tämä on esiopastin, niin laitetaan saman raiteenpään pääopastimeen merkkibitti
        if tyyppi == Opastin.EO :
            if self.raiteenpaa.opastin is not None :
                self.raiteenpaa.opastin.tyyppi |= Opastin.KKEO


    def kiskonpaahan(self, kiskonpaa):
        self.kiskonpaa = kiskonpaa
        kiskonpaa.opastin = self
        opastimet.append(self)        


    def optimoiOpastin(self):
        """ Lisää opastimeen toimintoja: P2 -> P3 seuraavalle pääopastimelle ei ole selvää reittiä
            P2 / SO - lisätään EO, ellei selvällä reitillä ole esiopastinta
        """


        if self.tyyppi & 0x1 == 0 or self.kiskonpaa is None:
            return  # Ei ole optimoitava opastin

        # Tarkistetaan, ettei tämä ole virheellisen sijoituksen takia laitettu opastin
        # siten, että tarkistetaan, että seuraava kisko ei ole samaa raidetta. Tämä ei poista
        # mahdollisia virheellisiä raiteensulkuja tms.

        seuraava = self.kiskonpaa.seuraavaPaa()
        if seuraava is not None :
            if seuraava.kisko.raide == self.kiskonpaa.kisko.raide :
                # Ohhoh, kävi huonosti. Tämä opastin nollataan
                self.tyyppi = 0
                return


        if self.tyyppi & 0x80 == 0 :
            self.tyyppi |= Opastin.RO   # Lisätään liikennepaikalla oleviin opastimiin vaihtotyövalot

        kpaa = self.kiskonpaa.seuraavaPaa()

        
        hidas = False
        while( kpaa is not None) :
                
            if kpaa.kptieto & Kiskonpaa.HIDAS or kpaa.toinen.kptieto & Kiskonpaa.HIDAS or kpaa.kisko.sn < 40 :
                self.tyyppi |= Opastin.P3  # hitaita kiskoilla, tarttee kolme valoa
            if kpaa.opastin is not None :
                if kpaa.opastin.tyyppi == Opastin.EO :
                    return # Ei tartte esiopastimen valoja    
                elif kpaa.opastin.tyyppi & Opastin.P2 :
                    self.tyyppi |= Opastin.EO # Ei löytynty EO:ta
                    return
            kpaa = kpaa.seuraavaPaa()

            
        # Ei löydy suoralta reitiltä, eli laitetaan kolme valoa ja EO
        self.tyyppi |= Opastin.EO
        self.tyyppi |= Opastin.P3


    def opastinTunnus(self):
        """ Opastintunnus koostuu raideidstä << 4, P/e-bitistä 0 ja opastintunnuksista
        :return: Opastintunnus
        """
        tunnus = self.kiskonpaa.kisko.raide.raideid << 4
        if self.raiteenpaa.pohjoiseen == True :  # Pohjoiseen
            tunnus |= 0x1
        if self.tyyppi & 0x1 :  # Vihreä - voi aloittaa junankulkutien
            tunnus |= 0b0010    # Pääopastin
        elif self.tyyppi & 0x10 :  # Voi aloittaa vaihtokulkutien
            tunnus |= 0b0010    # Raideopastin
        elif self.tyyppi & 0x8 : # Voi antaa esiopasteen
            tunnus |= 0b0110    # Esiopastin
        return tunnus

class Raiteenpaa(object) :
    VAIHDE = 0x1
    RAIDERISTEYS = 0x2
    RAIDEPUSKURI = 0x4
    RAITEENSULKU = 0x8

    def __init__(self, tila, pohjoiseen = True) :
        self.tila = 0
        self.opastin = None
        self.pohjoiseen = pohjoiseen

        if "V" in tila and "Va" not in tila :
            self.tila = Raiteenpaa.VAIHDE
        if "RR" in tila :
            self.tila = Raiteenpaa.RAIDERISTEYS
        if "RP" in tila :
            self.tila |= Raiteenpaa.RAIDEPUSKURI

        if "Sp" in tila :
            self.tila |= Raiteenpaa.RAITEENSULKU


        if "Po" in tila :
            Opastin(self, Opastin.P2)
        elif "Ro" in tila :
            Opastin(self, Opastin.RO)
        elif "So" in tila :
            Opastin(self, Opastin.SO)


class Raide(object) :
    SAHKOISTAMATON = 0x4
    VALVOMATON = 0x8
    def __init__(self, raideid, liikennepaikka, nro, tila, pohjoistila, etelatila):
        self.raideid = raideid
        self.pituus = 0

        self.liikennepaikka = liikennepaikka
        self.nro = nro

        self.etelapaa = Raiteenpaa(etelatila, False)
        self.pohjoispaa = Raiteenpaa(pohjoistila, True)

        self.tila = self.etelapaa.tila << 4 | self.pohjoispaa.tila << 8
        if "SäEi" in tila :
            self.tila |= Raide.SAHKOISTAMATON
        if "ValvEi" in tila :
            self.tila |= Raide.VALVOMATON



def haeRaideliitokset() :
    rliit = dict()  # Raideliitokset (raiteelta, raiteelle, tieto)

    for liitos in liitokset.values() :
        if liitos.tila == Kiskoliitos.AKSELINLASKENTA :
            raiteelta = liitos.kiskonpaat[0].kisko.raide.raideid
            raiteelle = liitos.kiskonpaat[1].kisko.raide.raideid

            pari = (raiteelta, raiteelle)

            if pari not in rliit :  # Ei ole vielä tallessa
                asennosta = liitos.kiskonpaat[0].toinen.liitosasento
                asentoon = liitos.kiskonpaat[1].toinen.liitosasento

                # asentopari = (asennosta, asentoon, liitos.kiskonpaat[0].hidas | liitos.kiskonpaat[1].hidas)

                tila = asennosta << 12 | asentoon << 8 | liitos.kiskonpaat[0].kptieto | liitos.kiskonpaat[1].kptieto

                rliit[ pari ] = tila

    return rliit

class NakymaKisko(object) :
    EPAA = 0x1  # Eteläpää
    EMIINUS = 0x2 # Etelässä vasen vaihde
    EPLUS = 0x4
    PPAA = 0x10
    PMIINUS = 0x20
    PPLUS = 0x40
    LAITURI_VASEN = 0x100
    LAITURI_OIKEA = 0x200
    NAYTA_RAIDETUNNUS = 0x400
    NAYTA_JUNATUNNUS = 0x800

    def __init__(self, nakyma, raide, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto):

        self.nakyma = nakyma
        self.raide = raide
        self.etela_x = etela_x
        self.etela_y = etela_y
        self.pohjoinen_x = pohjoinen_x
        self.pohjoinen_y = pohjoinen_y

        self.kiskotieto = 0
        if "E*" in kiskotieto :
            self.kiskotieto |= NakymaKisko.EPAA
        elif "E-" in kiskotieto :
            self.kiskotieto |= NakymaKisko.EMIINUS
        elif "E+" in kiskotieto :
            self.kiskotieto |= NakymaKisko.EPLUS

        if "P*" in kiskotieto :
            self.kiskotieto |= NakymaKisko.PPAA
        elif "P-" in kiskotieto :
            self.kiskotieto |= NakymaKisko.PMIINUS
        elif "P+" in kiskotieto :
            self.kiskotieto |= NakymaKisko.PPAA

        if "Lv" in kiskotieto or "Lm" in kiskotieto :
            self.kiskotieto |= NakymaKisko.LAITURI_VASEN
        if "Lo" in kiskotieto or "Lo" in kiskotieto :
            self.kiskotieto |= NakymaKisko.LAITURI_OIKEA
        if "Nr" in kiskotieto :
            self.kiskotieto |= NakymaKisko.NAYTA_RAIDETUNNUS
        if "Nj" in kiskotieto :
            self.kiskotieto |= NakymaKisko.NAYTA_JUNATUNNUS

class NakymaTeksti(object) :

    def __init__(self, nakyma, sijainti_x, sijainti_y, teksti) :
        self.nakyma = nakyma
        self.sijainti_x = sijainti_x
        self.sijainti_y = sijainti_y
        self.teksti = teksti


class Nakyma(object) :
    def __init__(self, nakymaid, nimi) :
        self.nakymaid = nakymaid
        self.nimi = nimi
        self.nakymateksti = ""
        self.raiteet = []

    def lisaaRaide(self, raide,  etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto):

        # Lisätään raideen kirjoittamisteksti
        tekstina = "K %s%03d %d %d %d %d %s\n" % ( raide.liikennepaikka, raide.nro, etela_x,
        etela_y, pohjoinen_x, pohjoinen_y, kiskotieto)

        self.nakymateksti = self.nakymateksti + tekstina;

        #Lisätään raidetunnus
        self.raiteet.append(raide.raideid);

    def lisaaTeksti(self, sijainti_x, sijainti_y, teksti):
        tekstina = "T %d %d %s\n" % (sijainti_x, sijainti_y, teksti)
        self.nakymateksti = self.nakymateksti + tekstina;


liitokset = dict()
raiteet = dict()
kiskot = []
opastimet = []

import mysql.connector

cnx = mysql.connector.connect(host="192.168.1.100", user='ratapiha', password='ratapiha', database='ratapiha')
cur = cnx.cursor()

query = "SELECT kisko, etela_x, etela_y, pohjoinen_x, pohjoinen_y, raideid, kiskotieto, sn, liikennepaikka, \
raide, tila_raide, tila_pohjoinen, tila_etela from kisko natural join raide where nakyma=0"

cur.execute(query)

for (kisko, etela_x, etela_y, pohjoinen_x, pohjoinen_y, raideid, kiskotieto, sn, liikennepaikka, raide,
tila_raide, tila_pohjoinen, tila_etela) in cur :

    eliitos = None
    exy = ( etela_x, etela_y)
    if exy in liitokset :
        eliitos = liitokset[exy]
    else:
        eliitos = Kiskoliitos(etela_x, etela_y)
        liitokset[exy] = eliitos


    pliitos = None
    pxy = (pohjoinen_x, pohjoinen_y)
    if pxy in liitokset :
        pliitos = liitokset[pxy]
    else :
        pliitos = Kiskoliitos(pohjoinen_x, pohjoinen_y)
        liitokset[pxy] = pliitos


    raiteeni = None
    if raideid in raiteet :
        raiteeni = raiteet[raideid]
    else:
        raiteeni = Raide(raideid, liikennepaikka, raide, tila_raide, tila_pohjoinen, tila_etela)
        raiteet[raideid] = raiteeni

    kiskot.append( Kisko(kisko, eliitos, pliitos, kiskotieto, sn , raiteeni) )

print( len(kiskot) , " kiskoa " , len(raiteet)  , " raidetta", len(liitokset), "kiskoliitosta")

raideliitokset = haeRaideliitokset()

print(len(raideliitokset)," raideliitosta ")

print (len(opastimet), "opastinta")

for opastin in opastimet :
    opastin.optimoiOpastin()

# Haetaan näkymät
nakymat = dict()
query = "SELECT nakyma,nakymanimi from nakyma order by nakymanimi"
cur.execute(query)

for( nakyma, nakymanimi ) in cur :
    nakymat[nakyma] = Nakyma(nakyma, nakymanimi)
print( len(nakymat), " näkymää")

# Näkymien kiskot
nakymakiskot = []

query = "SELECT nakyma, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto, raideid FROM `kisko` \
NATURAL JOIN raide WHERE nakyma >0"
cur.execute(query)

for( nakyma, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto, raideid) in cur :
    if nakyma in nakymat and raideid in raiteet:
        raide = raiteet[raideid]
        nakymat[nakyma].lisaaRaide( raide,  etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto)

        # nakymakiskot.append( NakymaKisko(nakyma, raideid, etela_x, etela_y, pohjoinen_x, pohjoinen_y, kiskotieto) )
print( len(nakymakiskot), " näkymäkiskoa ")

# Näkymien tekstit
nakymatekstit = []
query = "SELECT nakyma,sijainti_x,sijainti_y,teksti from teksti"
cur.execute(query)

for( nakyma, sijainti_x, sijainti_y, teksti) in cur :
    if nakyma in nakymat :
        nakymat[nakyma].lisaaTeksti(sijainti_x, sijainti_y, teksti)
        # nakymatekstit.append(  NakymaTeksti(nakyma, sijainti_x, sijainti_y, teksti) )
print( len(nakymatekstit), " näkymätekstiä")

cnx.close()

# Tallennetaan

kon = mysql.connector.connect(host="192.168.1.100", user='ratapiha', password='ratapiha', database='rata')
kur = kon.cursor()

print("Tallennetaan")

if 0==1 :   # Hypätään yli
    # 1) Raiteet
    kysely = "insert into raide(raide,liikennepaikka,raidetunnus,raidetila,raidepituus) values (%s,%s,%s,%s,%s)"

    for raidenro in raiteet:
        raide = raiteet[raidenro]
        param = ( raide.raideid, raide.liikennepaikka, raide.nro, raide.tila, raide.pituus)
        kur.execute(kysely, param)

if 0==1 :
    kysely = "insert into kiskoliitos(kiskoliitos,x,y,kiskoliitoslaji) values (%s,%s,%s,%s)"
    for liitosind in liitokset :
        liitos = liitokset[liitosind]
        param = ( liitos.nro, liitos.x, liitos.y, liitos.tila)
        kur.execute(kysely,param)

if 0==1 :
    kysely = "insert into kisko(kisko,raide,etela,pohjoinen,kiskotieto,sn,kiskopituus) values (%s,%s,%s,%s,%s,%s,%s)"
    for kisko in kiskot :
        tieto = kisko.tieto | kisko.etelapaa.liitosasento << 8 | kisko.pohjoispaa.liitosasento << 12
        param = ( kisko.nro, kisko.raide.raideid, kisko.etelapaa.liitos.nro,
                  kisko.pohjoispaa.liitos.nro, tieto, kisko.sn, kisko.pituus )
        kur.execute(kysely, param)
if 0 == 1 :
    kysely = "insert into opastin(opastin,kisko,opastintyyppi) values (%s,%s,%s)"
    for opastin in opastimet :
        param = ( opastin.opastinTunnus(), opastin.kiskonpaa.kisko.nro, opastin.tyyppi)
        print( param , opastin.kiskonpaa.kisko.raide.liikennepaikka, opastin.kiskonpaa.kisko.raide.nro)
        if opastin.tyyppi != 0 :
            kur.execute(kysely, param)

if 0 == 1 :
    kysely = "insert into raideliitos(raiteelta,raiteelle,raideliitostila) values (%s,%s,%s)"
    for raideliitos in raideliitokset :
        param = ( raideliitos[0], raideliitos[1], raideliitokset[raideliitos])
        kur.execute(kysely, param)

if 0 == 0 :

    kysely = "insert into nakymakisko(nakyma,raide,etela_x,etela_y,pohjoinen_x,pohjoinen_y,kiskotieto) \
values (%s,%s,%s,%s,%s,%s,%s)"
    for nakymakisko in nakymakiskot :
        param = (nakymakisko.nakyma, nakymakisko.raide, nakymakisko.etela_x, nakymakisko.etela_y,
        nakymakisko.pohjoinen_x, nakymakisko.pohjoinen_y, nakymakisko.kiskotieto)
        kur.execute(kysely,param)

if 0 == 1 :

    kysely = "insert into nakymateksti(nakyma, sijainti_x, sijainti_y, teksti) values (%s, %s, %s, %s)"
    for nakymateksti in nakymatekstit :
        param = (nakymateksti.nakyma, nakymateksti.sijainti_x, nakymateksti.sijainti_y, nakymateksti.teksti)
        kur.execute(kysely, param)


if 1 == 1 :
    kysely = "insert into nakyma(nakyma, nakymanimi, nakymateksti) values (%s, %s, %s)"
    raidelisays = "insert into nakymaraide(nakyma,raide) values(%s, %s)"
    for nakymaid in nakymat :
        nakyma = nakymat[nakymaid]
        param = (nakyma.nakymaid, nakyma.nimi, nakyma.nakymateksti)
        kur.execute(kysely, param)
        for raide in nakyma.raiteet :
            param = ( nakymaid, raide)
            kur.execute(raidelisays, param)


kon.commit()
kur.close()
kon.close()