all: fn3 fn2 fn

fn3: fn3.c
	gcc $< -o $@ -DDEBUG

fn2: fn2.c
	gcc $< -o $@


fn: fn.c
	gcc $< -o $@

clean:
	rm fn3 fn2 fn
