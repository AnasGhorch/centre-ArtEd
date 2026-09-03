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
    mainwindow.h
