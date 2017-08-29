cd build
cmake . ../codechiev -DCMAKE_BUILD_TYPE=Debug -DLOGGER_LEVEL=Trace -DBOOST_ROOT=../../boost_1_65_0/ -DROOT=../
make
