#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T09:05:47
#
#-------------------------------------------------

QT       += core gui

TARGET = GTgenerator
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp\
        gtvideo.cpp \
        abnormalrange.cpp \
    loadvideothread.cpp \
    window_addabnormalrange.cpp \
    roi_window.cpp

HEADERS  += mainwindow.h\
            gtvideo.h \
            abnormalrange.h \
    abnormalrange.h \
    gtvideo.h \
    loadvideothread.h \
    opencvheader.h \
    window_addabnormalrange.h \
    roi_window.h

FORMS    += mainwindow.ui \
    window_addabnormalrange.ui \
    roi_window.ui

INCLUDEPATH += /usr/local/include

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d
