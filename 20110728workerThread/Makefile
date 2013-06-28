a.out: main.o componentbase.o workqueue.o thread.o list.o
	g++ $^ -o $@ -g -lpthread

main.o: main.cpp
	g++ -c $< -o $@ -g

componentbase.o: componentbase.cpp componentbase.h
	g++ -c $< -o $@ -g


workqueue.o: workqueue.cpp workqueue.h
	g++ -c $< -o $@ -g

thread.o: thread.cpp thread.h
	g++ -c $< -o $@ -g

list.o: list.c list.h
	gcc -c $< -o $@ -g

clean:
	rm *.o
	rm a.out
