all:
	g++ -o wrapperTest wrapperTest.cpp krpcWrapper.cpp KRPC.pb.cc -lboost_system -lboost_thread `pkg-config --libs --cflags protobuf`
