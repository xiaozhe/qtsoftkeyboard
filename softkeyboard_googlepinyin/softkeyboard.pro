
QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = softkeyboard
TEMPLATE = lib
#CONFIG += static

CONFIG += c++11 #qt5
QMAKE_CXXFLAGS += -std=c++11 #qt4


#DESTDIR  = $$PWD/../lib
#改为根据编译KIT来存放编译结果
OUT_PWD_LIST=$$split(OUT_PWD, /)
KIT_NAME_INDEX=$$num_add($$size(OUT_PWD_LIST), -2)  ##从0开始,位置是个数-1,这里的编译名称在倒数第2位
OUT_PWD_LAST_PWD=$$member(OUT_PWD_LIST, $$KIT_NAME_INDEX, $$KIT_NAME_INDEX)
OUT_PWD_LAST_PWD_LIST=$$split(OUT_PWD_LAST_PWD, -)
KIT_NAME=$$first(OUT_PWD_LAST_PWD_LIST)


VERSION_APP_LIB=1.9.1
### 输出定义到代码里可读VERSION_APP_LIB
#DEFINES += VERSION_APP_LIB=\"\\\"$$VERSION_APP_LIB\\\"\"
### 没有空格,可以用:
DEFINES += VERSION_APP_LIB=\\\"_$$VERSION_APP_LIB\\\"
VERSION = $$VERSION_APP_LIB

DESTDIR  = $$PWD/../_build/$$KIT_NAME/lib

CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
    LIBS += -L$$PWD/../_build/$$KIT_NAME/lib -lqtpinyind

    DEFINES += XZSOFTKEYBOARD_DEBUG=1
}
else{
    LIBS += -L$$PWD/../_build/$$KIT_NAME/lib -lqtpinyin
}

INCLUDEPATH += $$PWD/../pinyin/include

DEFINES += LIB_SOFTKEYBOARD
#DEFINES += LIB_STATIC_SOFTKEYBOARD

SOURCES += \
    $$PWD/softkeyboard.cpp \
    $$PWD/CandidatesListWidget.cpp \
    $$PWD/pinyindecoderservice.cpp \
    keyboard_pinyin.cpp

HEADERS += \
    $$PWD/softkeyboard.h \
    $$PWD/CandidatesListWidget.h \
    $$PWD/pinyindecoderservice_p.h \
    $$PWD/softkeyboard_global.h \
    define_debug_output.h \
    keyboard_pinyin.h

FORMS += \
    $$PWD/softkeyboard.ui

RESOURCES += \
    $$PWD/resource/softkeyboard_res.qrc

DISTFILES += \
    copy_publish_files.sh



CONFIG(release, debug|release){
    publish_include = $$PWD/../_publish/$$KIT_NAME/softkeyboard/include
    publish_lib = $$PWD/../_publish/$$KIT_NAME/softkeyboard/lib

    QMAKE_POST_LINK += $$PWD/copy_publish_files.sh $$PWD $$publish_include $$DESTDIR $$publish_lib $$VERSION_APP_LIB $$TARGET;

    exists($$PWD/../_publish/$$KIT_NAME/softkeyboard/copy_to_nvrclient.sh){
        QMAKE_POST_LINK += $$PWD/../_publish/$$KIT_NAME/softkeyboard/copy_to_nvrclient.sh;
    }
}




