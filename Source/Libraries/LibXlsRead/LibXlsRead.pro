#-------------------------------------------------
#
# Project created by QtCreator 2015-01-20T14:58:10
#
#-------------------------------------------------

QT       -= gui

TARGET = LibXlsRead
TEMPLATE = lib

DEFINES += LIBXLSREAD_LIBRARY

# ///////////////////////////////////////////////////////////////////////////////////

include(./../../Path.pri)
# ///////////////////////////////////////////////////////////////////////////////////
LIBS += -liconv

INCLUDEPATH += ../

SOURCES += LibXlsRead.cpp \
    xls.c \
    xls2csv.c \
    xlstool.c \
    endian.c \
    getopt.c \
    ole.c

HEADERS += LibXlsRead.h\
        libxlsread_global.h \
    libxls/brdb.c.h \
    libxls/brdb.h \
    libxls/endian.h \
    libxls/ole.h \
    libxls/xlsstruct.h \
    libxls/xlstool.h \
    libxls/xlstypes.h \
    config.h \
    libxls/xls.h
