a.out: contain.c
	gcc $< -o $@

clean:
	rm a.out

r:
	@ make -s
	@ ./a.out | tee output.scn
