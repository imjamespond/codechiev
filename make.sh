boost=${boost:-'boost_1_67_0'}
loglevel=${loglevel:-'Trace'}
dev=${profile:-'yes'}
buildpath='build'
buildtype='Debug'

if [ 'no' = "${dev}" ]; then 
  loglevel='Info'
  buildpath="${buildpath}-release"
  buildtype='Release'
fi;

mkdir -p $buildpath
cd $buildpath

cmake . ../codechiev \
  -DCMAKE_BUILD_TYPE=${buildtype} \
  -DLOGGER_LEVEL=${loglevel} \
  -DBOOST_DIR="../${boost}/"
make -j 4
