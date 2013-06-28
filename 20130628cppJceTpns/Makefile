SRCS =  main.cpp \
        ../20130618cppSetMapVector/pdu_header.cpp

BIN = main

CLEAN = *.o \
        */*.o \
        $(BIN)

INC = -I../../tlib -I../20130618cppSetMapVector

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
