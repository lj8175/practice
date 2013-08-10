SRCS =  test.cpp \

BIN = test

CLEAN = *.o \
        $(BIN)

LIB =  

INC = 

CPPFLAGS = -g3 -Wall

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
