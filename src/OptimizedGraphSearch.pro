#-------------------------------------------------
#
# Project created by QtCreator 2013-02-10T20:09:29
#
#-------------------------------------------------

QT       += core
QT       -= gui

TARGET = OptimizedGraphSearch
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    edge/edge.cpp \
    geometry/geometryhelper.cpp \
    edge/edgeintersections.cpp \
    edgecontainer/spatialindexingedgecontainer.cpp \
    graph/roadgraph.cpp \
    edgecontainer/simpleedgecontainer.cpp

HEADERS += \
    edge/edge.h \
    geometry/point.h \
    geometry/boundingbox.h \
    edge/intersection.h \
    geometry/geometryhelper.h \
    edge/edgeintersections.h \
    edgecontainer/iedgecontainer.h \
    edgecontainer/spatialindexingedgecontainer.h \
    graph/RoadGraph.h \
    edgecontainer/simpleedgecontainer.h
