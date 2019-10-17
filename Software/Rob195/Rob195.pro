TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt


SOURCES += \
    src/main.cpp \
    src/com_func.cpp

HEADERS += \
    src/com_func.h

LIBS += \
    -lboost_system \
    -lboost_thread \
    -lboost_filesystem \
    -lOpenNI \
    -lOpenNI2 \
    -lpcl_io \
    -lpcl_filters \
    -lpcl_kdtree \
    -lpcl_registration \
    -lpcl_features \
    -lpcl_segmentation \
    -lpcl_visualization \
    -lpcl_common \
    -lpcl_filters \
    -lstdc++ \
    -lpcl_octree \
    -lpthread \
    -loctomap \
    -loctomath \


INCLUDEPATH += \
    /usr/include/eigen3 \
    /usr/include/pcl-1.8 \
    /usr/include/ni \
    /usr/include/boost \
    /usr/include/vtk-6.3\
    /usr/include/openni2




