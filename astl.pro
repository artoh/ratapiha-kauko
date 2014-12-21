 
QT	+= gui
QT      += widgets
QT	+= network
QT	+= sql

TARGET = astl

TEMPLATE = app

SOURCES += astl/main.cpp \
    astl/raidetieto.cpp \
    astl/raiteenpaa.cpp

HEADERS += \
    astl/raidetieto.h \
    astl/raiteenpaa.h

