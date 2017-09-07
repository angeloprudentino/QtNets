#! /bin/bash

#############################################
#
#   BUILD SCRIPT for QtNets Porject
#
#############################################

function gen_conf
{
	OUT="qtnets.conf"

	echo "#!/bin/sh" > $OUT
	echo "" >> $OUT
	echo "export QMAKE=\"/home/angelo/Qt/5.7/gcc_64/bin/qmake\"" >> $OUT
	echo "export PROJ_ROOT=\"/home/angelo/Projects/QtNets\"" >> $OUT
	echo "export APP_PROJ_DIR=\"\$PROJ_ROOT/Projects\"" >> $OUT
	echo "export LIB_PROJ_DIR=\"\$PROJ_ROOT/Projects/Lib\"" >> $OUT
	echo "export PLUGIN_PROJ_DIR=\"\$PROJ_ROOT/Projects/Plugin\"" >> $OUT
	echo "" >> $OUT
	echo "export ROOT_OUT_DIR=\"\$PROJ_ROOT/OUT\"" >> $OUT
	echo "export APP_OUT_DIR=\"\$ROOT_OUT_DIR/\$MODE\"" >> $OUT
	echo "export LIB_OUT_DIR=\"\$APP_OUT_DIR/Libs\"" >> $OUT
	echo "export PLUGIN_OUT_DIR=\"\$APP_OUT_DIR/Plugins\"" >> $OUT
	echo "export INCLUDE_OUT_DIR=\"\$APP_OUT_DIR/Include\"" >> $OUT
	echo "" >> $OUT
	echo "export SHARED_ROOT=\"\$PROJ_ROOT/Shared\"" >> $OUT
}

function print_help
{
	echo "*******************************************"
	echo "*******************************************"
	echo "**  only 1 mandatory parameter:"
	echo "**    - \"Debug\" or \"Release\""
	echo "*******************************************"
	echo "*******************************************"
}

#check if help required
if [ "$1" == "help" ]; then
	print_help
	exit;
fi;

#check if param is correct
if [ "$1" == "Debug" ]; then
	MODE="Debug"
	BUILD_CONF="CONFIG+=debug CONFIG+=qml_debug"
else
    if [ "$1" == "Release" ]; then
    	MODE="Release"
	else
		print_help
		exit;
	fi
fi

#generate conf if not existing
if [ ! -f qtnets.conf ]; then
	gen_conf
    echo "***************************************************************"
    echo "***************************************************************"
	echo "**"
	echo "** default $MODE qtnets.conf generated; check it and re-run"
	echo "**"
    echo "***************************************************************"
    echo "***************************************************************"
	exit
fi;
source ./qtnets.conf

#now ready to build
PWD=$(pwd)

mkdir $ROOT_OUT_DIR &> /dev/null
mkdir $APP_OUT_DIR &> /dev/null
mkdir $LIB_OUT_DIR &> /dev/null
mkdir $INCLUDE_OUT_DIR &> /dev/null
mkdir $PLUGIN_OUT_DIR &> /dev/null

#copy include
mkdir $INCLUDE_OUT_DIR/QN && cp -r $SHARED_ROOT/QN/*.h $INCLUDE_OUT_DIR/QN
mkdir $INCLUDE_OUT_DIR/QN/QNData && cp -r $SHARED_ROOT/QN/QNData/*.h $INCLUDE_OUT_DIR/QN/QNData
mkdir $INCLUDE_OUT_DIR/QN/QNDraw && cp -r $SHARED_ROOT/QN/QNDraw/*.h $INCLUDE_OUT_DIR/QN/QNDraw
mkdir $INCLUDE_OUT_DIR/QN/QNPersistence && cp -r $SHARED_ROOT/QN/QNPersistence/*.h $INCLUDE_OUT_DIR/QN/QNPersistence
mkdir $INCLUDE_OUT_DIR/QN/QNStyle && cp -r $SHARED_ROOT/QN/QNStyle/*.h $INCLUDE_OUT_DIR/QN/QNStyle
mkdir $INCLUDE_OUT_DIR/Smile && cp -r $SHARED_ROOT/Smile/*.h $INCLUDE_OUT_DIR/Smile
mkdir $INCLUDE_OUT_DIR/Smile/SmileData && cp -r $SHARED_ROOT/Smile/SmileData/*.h $INCLUDE_OUT_DIR/Smile/SmileData
mkdir $INCLUDE_OUT_DIR/Smile/SmileXML && cp -r $SHARED_ROOT/Smile/SmileXML/*.h $INCLUDE_OUT_DIR/Smile/SmileXML
mkdir $INCLUDE_OUT_DIR/Style && cp -r $SHARED_ROOT/Style/*.h $INCLUDE_OUT_DIR/Style
mkdir $INCLUDE_OUT_DIR/Style/StyleXML && cp -r $SHARED_ROOT/Style/StyleXML/*.h $INCLUDE_OUT_DIR/Style/StyleXML
mkdir $INCLUDE_OUT_DIR/Utils && cp -r $SHARED_ROOT/Utils/*.h $INCLUDE_OUT_DIR/Utils

#build QtNetsData
cd $LIB_PROJ_DIR/QtNetsData && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsData/QtNetsData.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p *.so.1 $LIB_OUT_DIR

#build QtNetsPersistence
cd $LIB_PROJ_DIR/QtNetsPersistence && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsPersistence/QtNetsPersistence.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p *.so.1 $LIB_OUT_DIR

#build QtNetsDraw
cd $LIB_PROJ_DIR/QtNetsDraw && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsDraw/QtNetsDraw.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p *.so.1 $LIB_OUT_DIR

#build QtNetsStyle
cd $LIB_PROJ_DIR/QtNetsStyle && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsStyle/QtNetsStyle.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p *.so.1 $LIB_OUT_DIR

#build SmilePersistencyPlugin
cd $PLUGIN_PROJ_DIR/SmilePersistencyPlugin && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile SmilePersistencyPlugin/SmilePersistencyPlugin.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p *.so.1 $PLUGIN_OUT_DIR

#build BaseStylePlugin
cd $PLUGIN_PROJ_DIR/BaseStylePlugin && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile BaseStylePlugin/BaseStylePlugin.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p *.so.1 $PLUGIN_OUT_DIR

#build QtNetsEditor
cd $APP_PROJ_DIR/QtNetsEditor && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsEditor/QtNetsEditor.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p QtNetsEditor $APP_OUT_DIR

#build QtNetsTest
cd $APP_PROJ_DIR/QtNetsTest && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsTest/QtNetsTest.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p QtNetsTest $APP_OUT_DIR

#build QtNetsBenchmark
cd $APP_PROJ_DIR/QtNetsBenchmark && mkdir $MODE
$QMAKE -spec linux-g++ $BUILD_CONF -o $MODE/Makefile QtNetsBenchmark/QtNetsBenchmark.pro || exit 1
cd $MODE
make qmake_all && make || exit 1
cp -p QtNetsBenchmark $APP_OUT_DIR

#generate launch and profile scripts
PROFILE="$APP_OUT_DIR/profile.sh"
echo "#!/bin/sh" > $PROFILE
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$LIB_OUT_DIR" >> $PROFILE
echo "export LD_LIBRARY_PATH" >> $PROFILE
echo "" >> $PROFILE
echo "valgrind -v \"./QtNetsEditor\"" >> $PROFILE
chmod +x $PROFILE

LAUNCH="$APP_OUT_DIR/launch.sh"
echo "#!/bin/sh" > $LAUNCH
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$LIB_OUT_DIR" >> $LAUNCH
echo "export LD_LIBRARY_PATH" >> $LAUNCH
echo "" >> $LAUNCH
echo "./QtNetsEditor" >> $LAUNCH
chmod +x $LAUNCH

TEST="$APP_OUT_DIR/test.sh"
echo "#!/bin/sh" > $TEST
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$LIB_OUT_DIR" >> $TEST
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$PLUGIN_OUT_DIR" >> $TEST
echo "export LD_LIBRARY_PATH" >> $TEST
echo "" >> $TEST
echo "./QtNetsTest" >> $TEST
chmod +x $TEST

BENCH="$APP_OUT_DIR/benchmark.sh"
echo "#!/bin/sh" > $BENCH
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$LIB_OUT_DIR" >> $BENCH
echo "LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:$PLUGIN_OUT_DIR" >> $BENCH
echo "export LD_LIBRARY_PATH" >> $BENCH
echo "" >> $BENCH
echo "./QtNetsBenchmark" >> $BENCH
chmod +x $BENCH

cd $PWD
exit;