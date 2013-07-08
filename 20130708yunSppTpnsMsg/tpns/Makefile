OBJS =  main.o \
        tpns.o \
        pdu_header.o

BIN = main

CLEAN = *.o \
        */*.o \
        $(BIN)

INC = -I./

LINK = -lpthread -lm

CXXFLAGS = -g -O2

#need not change
all: $(BIN)

$(BIN) : $(OBJS)
	g++ -o $@ $^ $(LINK)

%.o : %.cpp
	g++ $(CXXFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
