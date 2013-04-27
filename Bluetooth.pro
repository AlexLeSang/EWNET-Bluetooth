QT       += gui

debug {
    QT       += testlib
    CONFIG   += console
    QMAKE_CXX = ccache g++
}

HEADERS += \
    Master.hpp \
    Slave.hpp \
    Environment.hpp \
    Defines.hpp \
    Message.hpp \
    Sleep.hpp \
    RawDataWrapper.hpp \
    SimulationManager.hpp \
    BluetoothNode.hpp \
    CRC.hpp \
    Rand.hpp \
    CoreFacade.hpp \
    MainWindow.hpp

SOURCES += \
    Master.cpp \
    Slave.cpp \
    Environment.cpp \
    main.cpp \
    Message.cpp \
    Sleep.cpp \
    RawDataWrapper.cpp \
    SimulationManager.cpp \
    BluetoothNode.cpp \
    CRC.cpp \
    Rand.cpp \
    CoreFacade.cpp \
    MainWindow.cpp


QMAKE_CXXFLAGS += -std=c++0x

FORMS += \
    MainWindow.ui

OTHER_FILES += \
    Doxyfile

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.0.2/lib/release/ -lqwt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/qwt-6.0.2/lib/debug/ -lqwt
else:unix:!macx: LIBS += -L$$PWD/../../../../usr/local/qwt-6.1.0-rc3/lib/ -lqwt

INCLUDEPATH += $$PWD/../../../../usr/local/qwt-6.1.0-rc3/include
DEPENDPATH += $$PWD/../../../../usr/local/qwt-6.1.0-rc3/include
