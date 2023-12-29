TEMPLATE = app
TARGET = app
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -lpoppler-qt5
SOURCES += window.cpp

