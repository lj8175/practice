all: tinygc minigc

tinygc : tinygc.cpp
	g++ $< -o $@ -g

minigc : minigc.o t_minigc.o
	gcc $^ -o $@
minigc.o:  minigc.c
t_minigc.o: t_minigc.c


clean:
	rm *.o
	rm tinygc minigc
