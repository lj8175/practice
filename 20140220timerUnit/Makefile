SRCS =  main.cpp \
        timer_unit.cpp

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
