mkdir -p build
cd build
cmake . ../codechiev \
  -DCMAKE_BUILD_TYPE=Debug \
  -DLOGGER_LEVEL=Trace \
  -DBOOST_DIR=../../boost_1_65_0/
make