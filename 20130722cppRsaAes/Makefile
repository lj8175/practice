SRCS =  push_encrypt.cpp \
        main.cpp

BIN = main

CLEAN = *.o\
        $(BIN)

INC = 

LIB = -lcrypto

CPPFLAGS = -g -Wall

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
