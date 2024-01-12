TARGET = py_dict_builder
TEMPLATE = app

CONFIG += static

CONFIG(debug,debug|release){
TARGET = $$join(TARGET,,,d)
}


CONFIG += warn_off

#DESTDIR  = $$PWD/../lib
#改为根据编译KIT来存放编译结果
OUT_PWD_LIST=$$split(OUT_PWD, /)
KIT_NAME_INDEX=$$num_add($$size(OUT_PWD_LIST), -2)  ##从0开始,位置是个数-1,这里的编译名称在倒数第2位
OUT_PWD_LAST_PWD=$$member(OUT_PWD_LIST, $$KIT_NAME_INDEX, $$KIT_NAME_INDEX)
OUT_PWD_LAST_PWD_LIST=$$split(OUT_PWD_LAST_PWD, -)
KIT_NAME=$$first(OUT_PWD_LAST_PWD_LIST)

DESTDIR  = $$PWD/../_build/$$KIT_NAME/bin


#message( OUT_PWD_LIST $$OUT_PWD_LIST )
#message( OUT_PWD $$OUT_PWD size: $$size(OUT_PWD_LIST) )
#message( KIT_NAME_INDEX $$num_add($$size(OUT_PWD_LIST), -2) )
#message( member_get $$member(OUT_PWD_LIST, $$KIT_NAME_INDEX, $$KIT_NAME_INDEX ) )
#message( KIT_NAME $$KIT_NAME )

INCLUDEPATH = $$PWD/include

DEFINES += HAVE_PINYIN
DEFINES += ___BUILD_MODEL___

SOURCES += \
    $$PWD/command/pinyinime_dictbuilder.cpp

SOURCES += \
    share/dictbuilder.cpp \
    share/dictlist.cpp \
    share/dicttrie.cpp \
    share/lpicache.cpp \
    share/matrixsearch.cpp \
    share/mystdlib.cpp \
    share/ngram.cpp \
    share/pinyinime.cpp \
    share/searchutility.cpp \
    share/spellingtable.cpp \
    share/spellingtrie.cpp \
    share/splparser.cpp \
    share/sync.cpp \
    share/userdict.cpp \
    share/utf16char.cpp \
    share/utf16reader.cpp

HEADERS += \
    include/atomdictbase.h \
    include/dictbuilder.h \
    include/dictdef.h \
    include/dictlist.h \
    include/dicttrie.h \
    include/lpicache.h \
    include/matrixsearch.h \
    include/mystdlib.h \
    include/ngram.h \
    include/pinyinime.h \
    include/searchutility.h \
    include/spellingtable.h \
    include/spellingtrie.h \
    include/splparser.h \
    include/sync.h \
    include/userdict.h \
    include/utf16char.h \
    include/utf16reader.h

DEFINES += \
    QT_NO_CAST_TO_ASCII \
    QT_ASCII_CAST_WARNINGS \
    QT_NO_CAST_FROM_ASCII \
    QT_NO_CAST_FROM_BYTEARRAY

OTHER_FILES +=\
    data/rawdict_utf16_65105_freq.txt \
    data/valid_utf16.txt

#load(qt_helper_lib)



CONFIG += qt
QT = core
!win32 {
    CONFIG *= thread
}


_TAR_PATH = $$PWD/../_build/$$KIT_NAME/bin

#编译完成后
QMAKE_POST_LINK += cp $${PWD}/data/*.txt $${_TAR_PATH};
QMAKE_POST_LINK += cp $${PWD}/command/run_dick_builder.sh  $${_TAR_PATH}/run_dick_builder.sh;

_RUN_FILE=$${_TAR_PATH}/run_dick_builder.sh
QMAKE_POST_LINK += chmod +x $$_RUN_FILE;

#
#QMAKE_POST_LINK += "char_1=`printf %x 35`;"
#QMAKE_POST_LINK += char_2=`printf "\\\\x\\$\char_1"`;
#
#QMAKE_POST_LINK += echo \"\$\{char_2\}! /bin/bash;\" > $$_RUN_FILE;
#QMAKE_POST_LINK += echo \" \" >> run_dick_builder.sh;
#QMAKE_POST_LINK += echo \"export LD_LIBRARY_PATH=/home/dengss/Qt5152/5.15.2/gcc_64/lib:$LD_LIBRARY_PATH;\" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \"export LD_LIBRARY_PATH=/home/dengss/Buildroot/qt4.8.7/lib:$LD_LIBRARY_PATH;\" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \"export LD_LIBRARY_PATH=/mnt/dde1/codes/open.sources.tools/qt4.8.7/icu_dist_4-2/lib:$LD_LIBRARY_PATH;\" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \" \" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \" \" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \"./py_dict_builder; \" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \" \" >> $$_RUN_FILE;
#QMAKE_POST_LINK += echo \" \" >> $$_RUN_FILE;
#
#QMAKE_POST_LINK += chmod +x $$_RUN_FILE;

#编译前
#QMAKE_PRE_LINK += ""

DISTFILES += \
    command/run_dick_builder.sh

