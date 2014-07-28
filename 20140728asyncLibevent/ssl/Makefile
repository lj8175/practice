SRCS = server.cpp

BIN = server

CLEAN = *.o\
        $(BIN)


INC = 

LIB = -levent_openssl -levent -lssl -lcrypto

CPPFLAGS = -g -O2 -DNDEBUG


#need not change
all: $(BIN) client
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN) client

#add special
client : client.cpp
	g++ $< -o $@ $(LIB)

