SRCS =  heap_sort.cpp
SRCS2 =  heap_sort_recursion.cpp

BIN = main main2

CLEAN = *.o\
        $(BIN)

INC = 

LIB = 

CPPFLAGS = -g

#need not change
all: $(BIN)
main : $(SRCS:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

main2 : $(SRCS2:.cpp=.o)
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
