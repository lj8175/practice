SRCS =  ../arrgcd.cpp \
        test.cpp

BIN = main

CLEAN = *.o\
        $(BIN)

LIB = -L../../../components/gtest-1.6.0/lib -lgtest -lpthread -lm

INC = -I../../../components/gtest-1.6.0/include -I..

CPPFLAGS = -g -O2

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
