
RM       := rm -rf
CPP      := g++
CPPFLAGS := -std=c++11

SOCKET_HDR  = Socket.h

SERVER_SRC  = server.cc
SERVER_TARG = server.out

CLIENT_SRC  = client.cc
CLIENT_TARG = client.out

server: $(SOCKET_HDR) $(SERVER_SRC)
	$(CPP) $(CPPFLAGS) -o $(SERVER_TARG) $(SERVER_SRC)

client: $(SOCKET_HDR) $(CLIENT_SRC)
	$(CPP) $(CPPFLAGS) -o $(CLIENT_TARG) $(CLIENT_SRC)

all: server client

clean:
	$(RM) *.out
