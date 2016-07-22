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

	cd $PROJECT_PATH/build/base/tests
	ctest --output-on-failure .
}
function_update()
{
	git pull origin master
}

if [ "$1" = "build" ]; then
	function_update
	function_build ~/cpp/boost_1_61_0 `pwd`
elif [ "$1" = "bui" ]; then
	function_build ~/boost_1_59 `pwd`
else
	echo "input build for build,\
zip for zip."
fi
