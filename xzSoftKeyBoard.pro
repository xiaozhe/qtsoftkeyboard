TEMPLATE   = subdirs

#google pinyin库, 编译成静态库
SUBDIRS += $$PWD/pinyin/pinyin.pro 

#google pinyin 字典生成工具
SUBDIRS += $$PWD/pinyin/pinyin_dict_builder.pro

#软键盘,可以静态,也可以动态
SUBDIRS += $$PWD/softkeyboard_googlepinyin/softkeyboard.pro 

#测试APP
SUBDIRS += $$PWD/test_two_dialog/test_two_dialog.pro 



CONFIG += ordered
