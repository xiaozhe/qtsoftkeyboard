#! /bin/bash

current_path=$1;
tar_include_path=$2;

mkdir -p $tar_include_path;
rm -rf $tar_include_path/*;

cp $current_path/softkeyboard_global.h  $tar_include_path;
cp $current_path/softkeyboard.h         $tar_include_path;


lib_src_path=$3;
tar_lib_path=$4;
lib_version=$5;
lib_name=$6;

mkdir -p $tar_lib_path;
rm -rf $tar_lib_path/*;

cp ${lib_src_path}/lib${lib_name}*.so.${lib_version}   ${tar_lib_path};
cd ${tar_lib_path};
# ln -s lib${lib_name}*.so.${lib_version}
ln -s libsoftkeyboard.so.1.9.1 libsoftkeyboard.so.1.9;
ln -s libsoftkeyboard.so.1.9   libsoftkeyboard.so.1  ;
ln -s libsoftkeyboard.so.1     libsoftkeyboard.so    ;




