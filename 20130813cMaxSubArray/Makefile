SRCS =  maxsubarr.c \

BIN = test

CLEAN = *.o \
        $(BIN)

LIB =  

INC = 

CFLAGS = -g3 -Wall -std=c99

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.c=.o)
	gcc $(CFLAGS) -o $@ $^ $(LIB)

%o : %c
	gcc $(CFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
