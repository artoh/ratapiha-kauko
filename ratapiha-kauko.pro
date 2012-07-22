
QT  += core qui sql svg

TARGET = ratapiha-kauko
TEMPLATE = app

HEADERS += \
    kaukoikkuna.h \
    kaukoyhteys.h \
    kaukokisko.h \
    kaukoscene.h \
    editorikisko.h \
    kisko.h \
    kiskoscene.h \
    editoriscene.h \
    editoriikkuna.h \
    editoriview.h \
    ruudukko.h \
    mittanauha.h \
    kaukoview.h \
    editoriraide.h \
    raiteenpaa.h \
    raidetieto.h \
    ratakisko.h \
    rataraide.h \
    ratascene.h \
    laajennettukisko.h \
    rataikkuna.h \
    rataview.h \
    opastin.h \
    kulkutieelementti.h \
    kulkutienmuodostaja.h \
    naapuruus.h \
    kulkutienraide.h \
    kulkutie.h \
    akseli.h \
    vaunu.h \
    vaunukataloogi.h \
    veturi.h \
    ajopoyta.h

SOURCES += \
    kaukoikkuna.cpp \
    main.cpp \
    kaukoyhteys.cpp \
    kaukokisko.cpp \
    kaukoscene.cpp \
    editorikisko.cpp \
    kisko.cpp \
    kiskoscene.cpp \
    editoriscene.cpp \
    editoriikkuna.cpp \
    editoriview.cpp \
    ruudukko.cpp \
    mittanauha.cpp \
    kaukoview.cpp \
    editoriraide.cpp \
    raiteenpaa.cpp \
    raidetieto.cpp \
    ratakisko.cpp \
    rataraide.cpp \
    ratascene.cpp \
    laajennettukisko.cpp \
    rataikkuna.cpp \
    rataview.cpp \
    opastin.cpp \
    kulkutieelementti.cpp \
    kulkutienmuodostaja.cpp \
    naapuruus.cpp \
    kulkutienraide.cpp \
    kulkutie.cpp \
    akseli.cpp \
    vaunu.cpp \
    vaunukataloogi.cpp \
    veturi.cpp \
    ajopoyta.cpp

RESOURCES += \
    ratapiha-kauko.qrc

OTHER_FILES += \
    pic/viivain.png \
    pic/ikkuna-uusi.png \
    pic/valintanuoli.png \
    pic/teksti.png \
    pic/pyyhekumi.png \
    pic/muokkaa.png \
    pic/kyna.png \
    pic/laituri-oikea.png \
    pic/laituri-vasen.png \
    pic/nayta-junanumerot.png \
    pic/nayta-raidenumerot.png \
    pic/scroll.png \
    pic/zoom-rajaus.png \
    pic/nopeusrajoitus.png \
    pic/ro-aja.png \
    pic/po-ajasn.png \
    pic/po-aja.png \
    pic/so-seis.png \
    pic/po-seis.png \
    pic/ro-seis.png \
    pic/eo-seis.png \
    pic/raideristeys.png \
    pic/raiteensulku.png \
    pic/silta.png \
    pic/eivalvottu.png \
    pic/sahko.png \
    pic/raiteensulku-pohjoiseen.png \
    pic/vihrea.png \
    pic/varojunaa.png \
    pic/vaihtokulkutienkursori.png \
    pic/vaihtokulkutieloppuunkursori.png \
    pic/vaihteenkaanto.png \
    pic/vaihdekursori.png \
    pic/raideopastin.png \
    pic/nuoliyhdistelma.xcf \
    pic/kulkutienpurkukursori.png \
    pic/kulkutienpurku.png \
    pic/kasiyhdistelma.xcf \
    pic/junakulkutienkursori.png \
    pic/junakulkutieloppuunkursori.png \
    pic/esiopastin.png \
    pic/vaunut.svg

FORMS += \
    ajopoyta.ui


