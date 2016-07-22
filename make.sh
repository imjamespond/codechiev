echo shell begin
echo parameter num $#
echo proceed id $$
echo shell name $0

PROJECT=codechiev
PROJECT_PATH=`pwd`

function_build()
{
mkdir -p build
cd build/
cmake . ../$PROJECT \
-DBOOST_ROOT=$1 \
-DROOT=$2 
make && make install

#./build/server
#./build/server 2>&1 > log.txt &
cd $PROJECT_PATH/build/base/tests
ctest --output-on-failure .
}

function_git()
{
}

if [ "$1" = "build" ]; then
	function_build ~/cpp/boost_1_61 `pwd`
elif [ "$1" = "svn" ]; then
	function_git
else
	echo "input build for build,\
zip for zip."
fi
