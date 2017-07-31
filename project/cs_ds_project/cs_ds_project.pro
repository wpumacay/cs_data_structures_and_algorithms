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
    engine/matrix/LMatrix.h \
    engine/LCommon.h \
    engine/LCircle.h \
    engine/LConfiguration.h \
    engine/LSolver.h \
    itsvnd/optimization/LBaseOptimizer.h \
    itsvnd/optimization/LGradientDescentOptimizer.h \
    itsvnd/neighborhood/Neighborhoods.h \
    itsvnd/intensification/LBaseIntensifier.h \
    itsvnd/intensification/LVNDIntensifier.h \
    itsvnd/intensification/LTSIntensifier.h \
    itsvnd/intensification/LTS_VNDintensifier.h

LIBS += -llapack -lblas -larmadillo
