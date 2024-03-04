#! /bin/bash


basepath=$(cd `dirname $0`; pwd);
cp -f $basepath/lib/libsoftkeyboard.so.1.9.1   $basepath/../../../../nvr_client/libs/softkeyboard/libsoftkeyboard.so.1.9.1
cp -f $basepath/include/*                      $basepath/../../../../nvr_client/anviz_codes/Intellisight/lib_third/softkeyboard/include
