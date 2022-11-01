QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += printsupport xml

TARGET = arhFDTD
TEMPLATE = app

QMAKE_CXXFLAGS += -fopenmp #/openmp
LIBS += -fopenmp

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Structure.cpp \
    main.cpp \
    mainwindow.cpp \
    projecttree.cpp \
    stdafx.cpp

HEADERS += \
    Constants.h \
    Structure.h \
    mainwindow.h \
    projecttree.h \
    stdafx.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



unix:!macx: LIBS += -L$$PWD/../../../../../../usr/local/qwt-6.2.0/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../../../usr/local/qwt-6.2.0/include
DEPENDPATH += $$PWD/../../../../../../usr/local/qwt-6.2.0/include

unix:!macx: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lopencv_world

INCLUDEPATH += $$PWD/../../../../../../usr/local/include/opencv4
DEPENDPATH += $$PWD/../../../../../../usr/local/include/opencv4
