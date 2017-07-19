TEMPLATE = app
TARGET = cs_ds_project

QT = core gui
greaterThan( QT_MAJOR_VERSION, 4 ): QT += widgets

SOURCES += \
    main.cpp \
    engine/qt/lcirclerenderer.cpp \
    engine/qt/lwindow.cpp \
    engine/LConfiguration.cpp

HEADERS += \
    engine/qt/lcirclerenderer.h \
    engine/qt/lwindow.h \
    engine/LCommon.h \
    engine/LCircle.h \
    engine/LConfiguration.h \
    itsvnd/optimization/BFGS_optimizer.h

LIBS += -llapack -lblas -larmadillo