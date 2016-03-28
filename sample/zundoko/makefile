
RM       := rm -rf
CPP      := g++
CPPFLAGS := -std=c++11

SOCKET_HDR  = zundoko.h

SERVER_SRC  = server.cc
SERVER_TARG = server.out

CLIENT_SRC  = client.cc
CLIENT_TARG = client.out

all: server client main

main: main.cc
	$(CPP) $(CPPFLAGS) main.cc -lpgen2

server: $(SOCKET_HDR) $(SERVER_SRC)
	$(CPP) $(CPPFLAGS) -o $(SERVER_TARG) $(SERVER_SRC)

client: $(SOCKET_HDR) $(CLIENT_SRC)
	$(CPP) $(CPPFLAGS) -o $(CLIENT_TARG) $(CLIENT_SRC)

clean:
	$(RM) *.out
