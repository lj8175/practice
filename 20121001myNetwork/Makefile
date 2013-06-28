SRCS = server.cpp

BIN = server 

CLEAN = *.o\
        $(BIN)

INC = -I../../tlib

LIB = -lpthread -lm -L../../tlib -ltlib

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
	g++ $< -o $@

