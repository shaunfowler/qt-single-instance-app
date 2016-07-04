QT += core
QT += network
QT -= gui

CONFIG += c++11

TARGET = qt-single-instance-app
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    singleinstance.cpp

HEADERS += \
    singleinstance.h
