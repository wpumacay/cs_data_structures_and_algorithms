TEMPLATE = app
TARGET = cs_ds_project

QT = core gui
greaterThan( QT_MAJOR_VERSION, 4 ): QT += widgets

SOURCES += \
    main.cpp \
    engine/qt/lcirclerenderer.cpp \
    engine/qt/lwindow.cpp \
    engine/LConfiguration.cpp \
    engine/LSolver.cpp \
    itsvnd/neighborhood/Neighborhoods.cpp

HEADERS += \
    engine/qt/lcirclerenderer.h \
    engine/qt/lwindow.h \
    engine/LCommon.h \
    engine/LCircle.h \
    engine/LConfiguration.h \
    engine/LSolver.h \
    itsvnd/optimization/LBaseOptimizer.h \
    itsvnd/optimization/LGradientDescentOptimizer.h \
    itsvnd/optimization/LBS_BFGSoptimizer.h \
    itsvnd/neighborhood/Neighborhoods.h \
    itsvnd/intensification/LBaseIntensifier.h \
    itsvnd/intensification/LVNDintensifier.h

LIBS += -llapack -lblas -larmadillo
