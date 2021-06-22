#!/bin/bash 

curpath=`pwd`
boostpath="${curpath}/${boost:-boost}"
echo $boostpath

buildpath='build' 
buildtype='Debug'

mkdir -p $buildpath
cd $buildpath

cmake . ../ \
-DLOGGER_LEVEL=${loglevel} \
-DCMAKE_BUILD_TYPE=${buildtype} 
#-DBOOST_DIR=${boostpath} \

make -j 4