QT       += core gui gui-private sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialog001.cpp \
    dialog002.cpp \
    main.cpp \
    mainwindow.cpp \
    widget001.cpp

HEADERS += \
    dialog001.h \
    dialog002.h \
    mainwindow.h \
    widget001.h


DEFINES += VIRTUALKEYBOARD_DEBUG=1


FORMS += \
    dialog001.ui \
    dialog002.ui \
    mainwindow.ui \
    widget001.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


#DESTDIR  = $$PWD/../lib
#改为根据编译KIT来存放编译结果
OUT_PWD_LIST=$$split(OUT_PWD, /)
KIT_NAME_INDEX=$$num_add($$size(OUT_PWD_LIST), -2)  ##从0开始,位置是个数-1,这里的编译名称在倒数第2位
OUT_PWD_LAST_PWD=$$member(OUT_PWD_LIST, $$KIT_NAME_INDEX, $$KIT_NAME_INDEX)
OUT_PWD_LAST_PWD_LIST=$$split(OUT_PWD_LAST_PWD, -)
KIT_NAME=$$first(OUT_PWD_LAST_PWD_LIST)
#DESTDIR  = $$PWD/../$$KIT_NAME/bin
#DESTDIR  = $$PWD/bin/$$KIT_NAME
DESTDIR  = $$PWD/../_build/$$KIT_NAME/bin


CONFIG(debug, debug|release){
    TARGET = $$join(TARGET,,,d)
    LIBS += -L$$PWD/../_build/$$KIT_NAME/lib -lsoftkeyboardd
}
else{
    LIBS += -L$$PWD/../_build/$$KIT_NAME/lib -lsoftkeyboard
}


INCLUDEPATH += $$PWD/../softkeyboard_googlepinyin
HEADERS += \
    $$PWD/../softkeyboard_googlepinyin/softkeyboard_global.h
    $$PWD/../softkeyboard_googlepinyin/softkeyboard.h




DISTFILES += \
    debug_run_test.sh \
    run_test.sh


#编译完成后
_TAR_PATH = $$PWD/../_build/$$KIT_NAME/bin
CONFIG(debug, debug|release){
    QMAKE_POST_LINK += cp $${PWD}/debug_run_test.sh  $${_TAR_PATH}/debug_run_test.sh;
    _RUN_FILE=$${_TAR_PATH}/debug_run_test.sh
    QMAKE_POST_LINK += chmod +x $$_RUN_FILE;
}
else{
    QMAKE_POST_LINK += cp $${PWD}/run_test.sh  $${_TAR_PATH}/run_test.sh;
    _RUN_FILE=$${_TAR_PATH}/run_test.sh
    QMAKE_POST_LINK += chmod +x $$_RUN_FILE;
}

