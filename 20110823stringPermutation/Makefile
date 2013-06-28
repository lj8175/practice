all: a.out b.out

a.out: permutation.cpp
	g++ $< -o $@ -g

b.out: permutation.c
	gcc $< -o $@ -g

clean:
	rm a.out
	rm b.out
	rm out.data
