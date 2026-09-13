CXX = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -pthread
INCLUDES = -I bootpd/src -I thirdparty
LDFLAGS = -pthread
BINDIR = bin

all:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(LDFLAGS) -o $(BINDIR)/bootpd \
		bootpd/src/main.cpp \
		bootpd/src/Bootpd/Bootpd.cpp \
		bootpd/src/Bootpd/Common/Functions.cpp \
		bootpd/src/Bootpd/Network/Server.cpp \
		bootpd/src/Bootpd/Network/ServerManager.cpp \
		bootpd/src/Bootpd/Network/Socket.cpp \
		thirdparty/tinyxml2/tinyxml2.cpp

clean:
	rm -f $(BINDIR)/bootpd
	rm -rf *.o
	
.PHONY: all clean
