TEMPLATE = app
TARGET = app
QT = core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
INCLUDEPATH += /usr/include/poppler/qt5
LIBS += -lpoppler-qt5
SOURCES += main.cpp file_watcher.cpp pdf_viewer.cpp
HEADERS += file_watcher.h pdf_viewer.h

