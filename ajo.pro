 
QT	+= gui
QT      += widgets
QT	+= network

TARGET = ajo

TEMPLATE = app

SOURCES += ajo/main.cpp \ 
    ajo/ajopoyta.cpp

HEADERS += ratapiha.h \ 
    ajo/ajopoyta.h
    
FORMS += \ 
    ajo/ajopoyta.ui

OTHER_FILES +=

RESOURCES += \
    ajo.qrc
