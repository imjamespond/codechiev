mkdir -p build
cd build

LOGLV=${LOGLV:-'Trace'}

cmake . ../codechiev \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLOGGER_LEVEL=${LOGLV} \
  -DBOOST_DIR=../boost_1_67_0/
make
