all:
	g++ -o krpci_test krpci_test.cpp krpci_base.cpp krpci_generated.cpp KRPC.pb.cc -lboost_system -lboost_thread `pkg-config --libs --cflags protobuf`
