QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mpvconfigurator
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    tabvideo.cpp \
    tabaudio.cpp \
    tabsubtitle.cpp \
    tabscreenshot.cpp \
    tabosc.cpp \
    setting.cpp \
    valueboxsetting.cpp \
    comboboxsetting.cpp \
    slidersetting.cpp \
    tabosd.cpp \
    tabwindow.cpp \
    tabmisc.cpp \
    tabextensions.cpp \
    unknownsettingstab.cpp \
    tabconfig.cpp

HEADERS  += mainwindow.hpp \
    tabvideo.hpp \
    tabaudio.hpp \
    tabsubtitle.hpp \
    tabscreenshot.hpp \
    tabosc.hpp \
    setting.hpp \
    valueboxsetting.hpp \
    comboboxsetting.hpp \
    slidersetting.h \
    tabosd.hpp \
    tabwindow.hpp \
    tabmisc.hpp \
    tabextensions.hpp \
    unknownsettingstab.hpp \
    tabconfig.hpp

FORMS    += mainwindow.ui \
    tabvideo.ui \
    tabaudio.ui \
    tabsubtitle.ui \
    tabscreenshot.ui \
    tabosc.ui \
    tabosd.ui \
    tabwindow.ui \
    tabmisc.ui \
    tabextensions.ui \
    unknownsettingstab.ui \
    tabconfig.ui

QMAKE_CXXFLAGS += -std=c++11

# OS X specific compilation requirements since Apple hates C++.
macx{
QMAKE_CXXFLAGS += -std=c++0x -stdlib=libc++
QMAKE_LFLAGS += -lc++
}

RESOURCES = resources.qrc

RC_FILE = icon_windows.rc       # Windows Icon
ICON = ./images/icon.icns       # OS X Icon
