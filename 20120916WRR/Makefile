SRCS =  wrrscheduler.cpp \
        stepscheduler.cpp \
        levelscheduler.cpp \
        randomscheduler.cpp \
        timer.cpp \
        main.cpp

BIN = main

CLEAN = *.o\
        $(BIN)

INC = -I../../tlib

LIB = -lpthread -lm -L../../tlib -ltlib

CPPFLAGS = -g -O2

#need not change
all: $(BIN)
$(BIN) : $(SRCS:.cpp=.o)
	cd ../../tlib; make; cd -;
	g++ $(CPPFLAGS) -o $@ $^ $(LIB)

%o : %cpp
	g++ $(CPPFLAGS) $(INC) -c $< -o $@

clean:
	rm -rf $(CLEAN)
