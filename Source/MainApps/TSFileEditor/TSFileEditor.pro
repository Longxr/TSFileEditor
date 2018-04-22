#-------------------------------------------------
#
# Project created by QtCreator 2018-04-04T10:31:33
#
#-------------------------------------------------

QT       += core gui xml
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TSFileEditor
TEMPLATE = app

win32-msvc*:QMAKE_CXXFLAGS += /wd"4100"
include(./../../Path.pri)

INCLUDEPATH += ../../Libraries

LIBS += -L$${DESTDIR} -lLibXlsxRW

SOURCES += main.cpp\
        MainWindow.cpp \
    XmlRW.cpp \
    ExcelRW.cpp \
    DataModel/TranslateModel.cpp \
    NetWorker.cpp \
    TranslateWorker.cpp

HEADERS  += MainWindow.h \
    XmlRW.h \
    ExcelRW.h \
    DataModel/TranslateModel.h \
    NetWorker.h \
    TranslateWorker.h \
    AppConfig.h

FORMS    += MainWindow.ui



