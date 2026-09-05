QT       += core gui widgets sql
CONFIG   += c++17
TEMPLATE = app
TARGET   = centre_formation

SOURCES += \
    main.cpp \
    database.cpp \
    formateurdao.cpp \
    coursdao.cpp \
    formateurdialog.cpp \
    coursdialog.cpp \
    formateurwidget.cpp \
    courswidget.cpp \
    statistiqueswidget.cpp \
    simplecharts.cpp \
    courspdf.cpp \
    mainwindow.cpp

HEADERS += \
    database.h \
    formateur.h \
    cours.h \
    formateurdao.h \
    coursdao.h \
    formateurdialog.h \
    coursdialog.h \
    formateurwidget.h \
    courswidget.h \
    statistiqueswidget.h \
    simplecharts.h \
    courspdf.h \
    mainwindow.h

RESOURCES += resources.qrc
