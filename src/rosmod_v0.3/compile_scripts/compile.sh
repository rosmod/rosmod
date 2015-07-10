echo "Cross-Compiling ROSMOD Package:" $1
cd $1
source /opt/ros/indigo/setup.bash
catkin_make
WORKSPACE_PATH=$1
DEVEL_PATH=$1/devel
DEVEL_LIB_PATH=$1/devel/lib
BUILD_PATH=$1/build
mv $DEVEL_LIB_PATH/*.so $WORKSPACE_PATH/../../04-Binaries/$2/.
mv $DEVEL_LIB_PATH/node/node_main $WORKSPACE_PATH/../../04-Binaries/$2/.
rm -rf $WORKSPACE_PATH/devel
rm -rf $WORKSPACE_PATH/build
echo "Cross-compilation complete!"
