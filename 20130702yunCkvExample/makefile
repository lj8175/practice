
BASEDIR=../
#INC = -I ${BASEDIR}/dependence/L5_API
INC = -I ${BASEDIR}
LIB =-L ${BASEDIR}/lib -L ${BASEDIR}/dependence/L5_API/lib -ltrmem -L ${BASEDIR} -lasn1c++ -lqos_client

BIN =  cmem_api_demo
DEBUG=-g
CC=g++ ${DEBUG} -Wall
##################################

OBJ = $(BIN).o

all: $(BIN)

$(BIN): ${OBJ}
	$(CC) $(FLG) -o $@ $(INC) $^ $(LIB)

%.o: %.cpp
	$(CC) $(FLG) $(INC) -c -o $@ $<

clean:
	rm -f *.o ${BIN}

