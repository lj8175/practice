SRCS =  main.cpp \
        tpns.cpp \
        pdu_header.cpp

BIN = main

CLEAN = *.o \
        */*.o \
        $(BIN)

INC = 

LIB = -lpthread -lm 

CPPFLAGS = -g -O2 -std=c++0x

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
