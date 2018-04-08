#-------------------------------------------------
#
# Project created by QtCreator 2015-01-20T15:14:16
#
#-------------------------------------------------

QT       -= gui

TARGET = LibXlsxRW
TEMPLATE = lib

# ///////////////////////////////////////////////////////////////////////////////////

include(./../../Path.pri)
# ///////////////////////////////////////////////////////////////////////////////////

INCLUDEPATH += ../LibXlsxRW

DEFINES += LIBXLSXRW_LIBRARY
include(xlsx/qtxlsx.pri)
SOURCES += LibXlsxRW.cpp

HEADERS += LibXlsxRW.h\
    libxlsxrw_global.h
