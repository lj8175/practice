SRCS =  main.cpp \
        poller_unit.cpp \
        tcp_listener.cpp \
        tcpconn_handler.cpp

BIN = main

CLEAN = *.o\
        $(BIN)

INC = 

LIB = 

CPPFLAGS = -g -Wall 

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
