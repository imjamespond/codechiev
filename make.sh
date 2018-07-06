mkdir -p build
cd build
cmake . ../codechiev \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLOGGER_LEVEL=Debug \
  -DBOOST_DIR=../boost_1_67_0/
make
