QT += core
QT -= gui

CONFIG += c++11

TARGET = raytracing
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    Vec3f.cpp

HEADERS += \
    color.h \
    ray_math_algo.h \
    object.h \
    Vec3f.h \
    material.h \
    ray.h \
    zhuang.h \
    ball.h \
    plain.h \
    light.h \
    pointlight.h \
    squarelight.h \
    camera.h \
    scene.h \
    raytracing.h \
    triangle.h \
    poly.h \
    bdbox.h \
    kdtreenode.h \
    kdtree.h \
    pathtracing.h \
    stb_image.h \
    stb_image_write.h \
    stb_image_resize.h \
    myimg.h
QMAKE_CXXFLAGS += -fopenmp
LIBS += -lgomp -lpthread

DISTFILES += \
    scene/floor.bmp \
    scene/floor3.bmp \
    scene/kamen.bmp \
    scene/marble.bmp \
    scene/ball1.txt \
    scene/ball2.txt \
    scene/ball3.txt \
    scene/ball4.txt \
    scene/ball5.txt \
    scene/ball6.txt \
    scene/camera.txt \
    scene/camera2.txt \
    scene/camera3.txt \
    scene/cornellball1.txt \
    scene/cornellball1rt.txt \
    scene/cornellball2.txt \
    scene/cornellballdiff.txt \
    scene/cornellplain1.txt \
    scene/cornellplain2.txt \
    scene/cornellplain3.txt \
    scene/cornellplain4.txt \
    scene/cornellplain5.txt \
    scene/cornellplain6.txt \
    scene/cornellsphere1.txt \
    scene/cornellsphere2.txt \
    scene/cornellsphere3.txt \
    scene/cornellsphere4.txt \
    scene/cornellsphere5.txt \
    scene/cornellsphere6.txt \
    scene/plain1.txt \
    scene/poly1.txt \
    scene/poly2.txt \
    scene/poly3.txt \
    scene/scene4.txt \
    scene/scene5.txt \
    scene/scene6.txt \
    scene/squarelight1.txt \
    scene/squarelight2.txt \
    scene/squarelight3.txt \
    scene/squarelight4.txt \
    scene/squarelight5.txt
