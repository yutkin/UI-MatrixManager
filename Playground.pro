#-------------------------------------------------
#
# Project created by QtCreator 2015-10-02T21:34:01
#
#-------------------------------------------------

QT       += core gui
CONFIG   += C++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Playground
TEMPLATE = app


SOURCES  += src/main.cpp \
            src/FrontView.cpp \
            src/AddMatrixDialog.cpp \
            src/MatrixViewDialog.cpp \
            src/MatrixListModel.cpp \
            src/FindMatrixDialog.cpp

HEADERS  += src/Matrix.hpp \
            src/MatrixExtended.hpp \
            src/MatrixViewDialog.hpp \
            src/AddMatrixDialog.hpp \
            src/FrontView.hpp \
            src/PositionEnum.hpp \
            src/MatrixListModel.hpp \
            src/FindMatrixDialog.hpp

FORMS    += ui/FrontView.ui \
            ui/AddMatrixDialog.ui \
            ui/MatrixViewDialog.ui \
            ui/FindMatrixDialog.ui
