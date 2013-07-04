SRCS =  client.cpp \
        command.cpp \
        concretecommand.cpp \
        invoker.cpp \
        receiver.cpp

BIN = main

CLEAN = *.o \
        */*.o \
        $(BIN)

INC = -I../../tlib

LIB = -lpthread -lm -L../../tlib 

CPPFLAGS = -g -O2 -std=c++0x

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
