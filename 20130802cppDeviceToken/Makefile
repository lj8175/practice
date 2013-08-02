SRCS =  deviceToken.cpp \
        hex_string.cpp \
        push_encrypt.cpp

BIN = test

CLEAN = *.o \
        $(BIN)

LIB =  -lcrypto

INC = 

CPPFLAGS = -g3

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
