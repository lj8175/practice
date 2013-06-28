SRCS =  stepmax.cpp \
        timer.cpp

BIN = main

CLEAN = *.o\
        $(BIN)

LINK = -lpthread -lm

CPPFLAGS = -g -O2

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LINK)

%o : %cpp
	g++ $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(CLEAN)
