
CXX = g++
CXXFLAGS = -O2 -g -Wall  -D__STDC_CONSTANT_MACROS
LDFLAGS = -lpthread 

OBJS = $(patsubst %.cpp,%.o,$(shell ls *.cpp))
TARGET = StreamTool
all: $(TARGET)

StreamTool : $(OBJS)
	$(CXX) $(CXXFLAGS) $(INCOPT) $^ $(LDFLAGS) -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCOPT) -c $? -o $@
clean:
	-rm -f *.o $(TARGET)
