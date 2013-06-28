SRCS =  lclient.cpp \
        main.cpp

BIN = main

CLEAN = *.o\
        $(BIN)

LIB = -lpthread

CPPFLAGS = -g -O2 -DNDEBUG

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(CLEAN)
