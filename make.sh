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
	-DCMAKE_BUILD_TYPE=Debug \
	-DLOGGER_LEVEL=Trace \
	-DBOOST_ROOT=$1 \
	-DPSQL=$2 \
	-DROOT=$3 
	
	make && make install

	if [ "$4" == "test" ]; then
		cd $PROJECT_PATH/build/base/tests
		ctest --output-on-failure .
		cd $PROJECT_PATH/build/net/tests
		ctest --output-on-failure .
	fi
}
function_update()
{
	git pull origin master
}

if [ "$1" = "build" ]; then
	function_update
	function_build ~/cpp/boost_1_61_0 /usr/pgsql-9.3 `pwd`
elif [ "$1" = "bui" ]; then
	function_update
	function_build ~/boost_1_59 /usr/pgsql-9.3 `pwd`
elif [ "$1" = "cop" ]; then
	git commit -a -m 'cm'
	git push local master
	ssh james@127.0.0.1 "cd ~/cpp/codechiev;
	./make.sh bui"
elif [ "$1" = "coph" ]; then
	git commit -a -m 'cm'
	git push yuyouidea master
	ssh -p9726 web1@yuyouidea.com "cd ~/cpp/codechiev;
	./make.sh build"
elif [ "$1" = "netrst" ]; then
	/etc/init.d/network restart
	netstat -plant
else
	echo "input build for build,\
zip for zip."
fi
