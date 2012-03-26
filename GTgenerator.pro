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
    roi_window.cpp \
    configuration.cpp

HEADERS  += mainwindow.h\
            gtvideo.h \
            abnormalrange.h \
    abnormalrange.h \
    gtvideo.h \
    loadvideothread.h \
    opencvheader.h \
    window_addabnormalrange.h \
    roi_window.h \
    libseg.h \
    configuration.h \
    sharedtypes.h \
    configparam.h

FORMS    += mainwindow.ui \
    window_addabnormalrange.ui \
    roi_window.ui \
    configuration.ui

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/MATLAB/R2011a/extern/include

LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy
LIBS += -L/usr/local/lib -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_features2d -lopencv_legacy
LIBS += -L/opt/MATLAB/MATLAB_Compiler_Runtime/v715/runtime/glnx86 -lmwmclmcrrt
LIBS += -L./ -lseg
# copy the libseg.so file to /usr/lib
# OR add ./../GTgenerator to LD_LIBRARY_PATH under Build Environment under Projects

RESOURCES += \
    ResFiles.qrc
