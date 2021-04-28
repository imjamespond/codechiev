echo $PATH

buildpath='build'

mkdir -p $buildpath
cd $buildpath

cmake . ../ \
  -DCMAKE_BUILD_TYPE=${buildtype} \
  -DLOGGER_LEVEL=${loglevel}

make -j 4