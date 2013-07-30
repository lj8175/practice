SRCS =  arrays.cpp \
        test.cpp

BIN = test

CLEAN = *.o \
        $(BIN)

LIB = -L./lib -lgtest -lpthread -lm 

INC = -I./include 

CPPFLAGS = -g

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
