
TEMPLATE	= lib
CONFIG	+= qt plugin warn_off
#CONFIG	+= x86_64
VAA3DPATH = D:/v3d_external
INCLUDEPATH	+= $$VAA3DPATH/v3d_main/basic_c_fun

HEADERS	+= merge_same_point_plugin.h \
    neuron_tools.h \
    my_surf_objs.h
SOURCES	+= merge_same_point_plugin.cpp \
    neuron_tools.cpp \
    ../../../../v3d_external/v3d_main/basic_c_fun/basic_surf_objs.cpp \
    my_surf_objs.cpp
SOURCES	+= $$VAA3DPATH/v3d_main/basic_c_fun/v3d_message.cpp

TARGET	= $$qtLibraryTarget(merge_same_point)
DESTDIR	= $$VAA3DPATH/bin/plugins/neuron_tools/merge_same_point/
