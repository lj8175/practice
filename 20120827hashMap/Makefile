SRCS =  hashmap.cpp \
        timer.cpp

BIN = hashmap

CLEAN = *.o\
        $(BIN)

LINK = -lpthread

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ -o $@ $^ $(LINK)

%o : %cpp
	g++ -c $< -o $@

clean:
	rm -rf $(CLEAN)
