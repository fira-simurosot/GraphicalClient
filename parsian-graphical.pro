#-------------------------------------------------
#
# Project created by QtCreator 2018-06-24T20:39:57
#
#-------------------------------------------------

QT       += core gui opengl network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = parsian-graphical
TEMPLATE = app

CONFIG+= c+11

SOURCES += main.cpp\
        mainwindow.cpp \
    gltext.cpp \
    soccerview.cpp \
    field_default_constants.cpp \
    field.cpp \
    msg/cpp/messages_parsian_simurosot_data_wrapper.pb.cc \
    msg/cpp/messages_parsian_simurosot_debugs.pb.cc \
    msg/cpp/messages_parsian_simurosot_worldmodel.pb.cc

HEADERS  += mainwindow.h \
    gltext.h \
    soccerview.h \
    timer.h \
    field_default_constants.h \
    geometry.h \
    field.h \
    bbox.h \
    geomalgo.h \
    gvector.h \
    pose.h \
    quaternion.h \
    range.h \
    util.h \
    msg/cpp/messages_parsian_simurosot_data_wrapper.pb.h \
    msg/cpp/messages_parsian_simurosot_debugs.pb.h \
    msg/cpp/messages_parsian_simurosot_worldmodel.pb.h

MOC_DIR = objs
OBJECTS_DIR = objs

INCLUDEPATH+=msg/cpp
INCLUDEPATH+=/usr/local/Cellar/protobuf/3.6.1.3_1/include
LIBS+=-L/usr/local/Cellar/protobuf/3.6.1.3_1/lib/ -lprotobuf -lprotobuf-lite

RESOURCES += \
    resource.qrc
