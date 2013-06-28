SRCS = pthreadSignal.cpp \
       safemap.cpp

BIN = safemap

CLEAN = *.o\
        $(BIN)\
        ip.txt

LINK = -lpthread

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	g++ -o $@ $^ $(LINK)

%o : %cpp
	g++ -c $< -o $@

clean:
	rm -rf $(CLEAN)
