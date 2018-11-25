boost=${boost:-'boost_1_67_0'}
loglevel=${loglevel:-'Trace'}
profile=${profile:-'dev'}
buildpath='build'

if [ 'Info' = "${loglevel}" ]; then 
  buildpath="${buildpath}-release"
fi;

mkdir -p $buildpath
cd $buildpath

cmake . ../codechiev \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLOGGER_LEVEL=${loglevel} \
  -DBOOST_DIR="../${boost}/"
make
