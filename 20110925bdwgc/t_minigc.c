#include <string.h>
#include <stdio.h>
#include "minigc.h"
static void test(void)
{
	char *v = NULL;
	unsigned long i;
	size_t size = 256;
	v = alloc(sizeof(char) * size);
	memset(v, 0, size);
	memset(v, '.', size - 1);
	strncpy(v, "abcdef", 6);
	v += size - 1;
	for(i = 0; i < 1000000; ++i)
	{
		int **p = (int **)alloc(sizeof(int *));
		int *q = (int *)alloc(sizeof(int));
		*p = NULL;//(int *) GC_REALLOC(q, 2 * sizeof(int));
		q = NULL;
		if (i % 100000 == 0)
			printf("Heap size = %d\n", get_heap_size());
		//GC_gcollect();
	}
	v -= size - 1;
	puts(v);
}
int main(void)
{
	char *v = NULL;
	gc_initialize();
	register_root(&v, &v + 1);
	v = alloc(sizeof(char) * 100);
	strcpy(v, "hello, world!");
	++v;
	test();
	puts(--v);
	return 0;
}

