#ifndef ALLOC_INCLUDED
#define ALLOC_INCLUDED
#ifdef __cplusplus
extern "C"
{
#endif
void gc_initialize(void);
void *alloc(size_t);
size_t get_heap_size(void);
void register_root(void *, void *);
#ifdef __cplusplus
}
#endif
#endif

