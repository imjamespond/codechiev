boost=${boost:-'boost_1_67_0'}
loglevel=${loglevel:-'Trace'}
release=${release:-'no'}
buildpath='build'
buildtype='Debug'

if [ 'yes' = "${release}" ]; then 
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
