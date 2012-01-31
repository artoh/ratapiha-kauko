
QT  += core qui sql

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
    editoriraiteenpaa.h

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
    editoriraiteenpaa.cpp

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
    pic/raiteensulku-pohjoiseen.png


