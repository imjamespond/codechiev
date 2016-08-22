cd build
cmake . ../codechiev -DCMAKE_BUILD_TYPE=Debug -DLOGGER_LEVEL=Trace -DBOOST_ROOT=../../boost-1-60/ -DROOT=../
make
