#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#if _MSC_VER >= 1200
#ifndef _CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#endif
#include <crtdbg.h>
#endif
struct node
{
	struct node *next;
	void *obj;
	size_t size;
	size_t flags;
};
struct node_block
{
	struct node_block *next;
	/** extra space **/
};
struct hash_table
{
	struct node **buckets;
	struct node *marked_list;
	struct node *follow_list;
	struct node *free_list;
	struct node *unlinked_list;
	size_t begin_addr;
	size_t end_addr;
	size_t log_size;
	size_t length;
};
struct static_root
{
	struct static_root *next;
	size_t begin_addr;
	size_t end_addr;
};
struct stack_root
{
	int stack_grows_up;
	size_t begin_addr;
	size_t end_addr;
};
struct gcdata
{
	struct hash_table htab;
	struct node_block *node_block_list;
	struct static_root *statics;
	struct stack_root stack;
	size_t static_size;
	size_t follow_scan_size;
	size_t pending_free_size;
	size_t expanded_heap_size;
	size_t total_heap_size;
	size_t allocd_before_gc;
	size_t allocd_bytes;
	size_t marked_bytes;
	size_t free_bytes;
};
static const struct node sentinel_node = {NULL, NULL, 0, 0};
static struct gcdata *global_gcdata = NULL;
#define CEIL(n)    ((n) + WMASK & ~WMASK)
#define FLOOR(n)   ((n) & ~WMASK)
enum constant
{
	WSIZE = sizeof(size_t),
	WMASK = sizeof(size_t) - 1,
	LOG_SIZE = 8,
	ATOM_FLAG = 1 << 0,
	LOG_OFFSET_IGNORE = 8,
	FREE_SPACE_DIVISOR = 5,
};
enum collect_level
{
	LEVEL_LITTLE,
	LEVEL_FAST,
	LEVEL_DEEP,
	LEVEL_FULL,
};
static void garbage_collect(struct gcdata *, enum collect_level);
static size_t approx_sp(struct gcdata *gc)
{
	size_t value = (size_t)&gc;
	return value;
}
static size_t stack_size(struct gcdata *gc)
{
	return gc->stack.end_addr - gc->stack.begin_addr;
}
static int guess_collect(struct gcdata *gc, size_t objsize)
{
	size_t size = WSIZE << gc->htab.log_size;
	size += (gc->marked_bytes + gc->allocd_bytes - gc->follow_scan_size) >> 2;
	size += gc->static_size + ((stack_size(gc) + gc->follow_scan_size) << 1);
	return (size / FREE_SPACE_DIVISOR) <= (gc->allocd_bytes + objsize);
}
static size_t guess_expand_heap(struct gcdata *gc, size_t objsize)
{
	size_t divisor = FREE_SPACE_DIVISOR + 1;
	size_t size = gc->marked_bytes + gc->allocd_bytes;
	return (size / divisor) <= gc->free_bytes ? 0 :
		(gc->allocd_bytes >> 3) + (objsize << 2) +
		gc->free_bytes * divisor +
		gc->static_size;
}
static int guess_expand_htable(struct gcdata *gc, size_t hint)
{
	return hint > (3u << (gc->htab.log_size - 2));
}
static size_t hash(struct gcdata *gc, size_t key)
{
	size_t code = ((key >> gc->htab.log_size) ^ key) >> LOG_OFFSET_IGNORE;
	size_t mask = (1 << gc->htab.log_size) - 1;
	return code & mask;
}
static struct node **find_reference(struct gcdata *gc, size_t addr)
{
	struct node **pnext = &gc->htab.buckets[hash(gc, addr)];
	while ((size_t)(*pnext)->obj > addr)
		pnext = &(*pnext)->next;
	if (addr - (size_t)(*pnext)->obj >= (*pnext)->size) {
		pnext = &gc->htab.buckets[hash(gc, addr - (1 << LOG_OFFSET_IGNORE))];
		while ((size_t)(*pnext)->obj > addr)
			pnext = &(*pnext)->next;
		if (addr - (size_t)(*pnext)->obj >= (*pnext)->size)
			return NULL;
	}
	return pnext;
}
static void scan_region(struct gcdata *gc, size_t begin_addr, size_t end_addr)
{
	size_t offset_ignore = gc->htab.end_addr - gc->htab.begin_addr + (1 << LOG_OFFSET_IGNORE);
	size_t *begin_region = (size_t *)begin_addr;
	size_t *region = (size_t *)end_addr;
	while (region-- > begin_region) {
		size_t addr = *region;
		if (addr - gc->htab.begin_addr < offset_ignore) {
			struct node **pnext = find_reference(gc, addr);
			if (pnext != NULL) {
				struct node *np = *pnext;
				*pnext = np->next;
				if (np->flags & ATOM_FLAG) {
					np->next = gc->htab.marked_list;
					gc->htab.marked_list = np;
				} else {
					np->next = gc->htab.follow_list;
					gc->htab.follow_list = np;
				}
				--gc->htab.length;
			}
		}
	}
}
static void scan_roots(struct gcdata *gc)
{
	size_t begin_addr = gc->stack.begin_addr;
	size_t end_addr = approx_sp(gc);
	struct static_root *np;
	jmp_buf mark;
	setjmp(mark);
	for (np = gc->statics; np; np = np->next)
		scan_region(gc, np->begin_addr, np->end_addr);
	if (!gc->stack.stack_grows_up) {
		begin_addr = end_addr;
		end_addr = gc->stack.end_addr;
	}
	scan_region(gc, CEIL(begin_addr), FLOOR(end_addr));
}
static void scan_followable(struct gcdata *gc)
{
	struct node *np;
	while ((np = gc->htab.follow_list) != NULL) {
		gc->htab.follow_list = np->next;
		np->next = gc->htab.marked_list;
		gc->htab.marked_list = np;
		scan_region(gc, (size_t)np->obj, (size_t)np->obj + FLOOR(np->size));
	}
}
static struct node *alloc_node(struct gcdata *gc, void *obj, size_t size, size_t vtable)
{
	struct node *np = gc->htab.unlinked_list;
	gc->htab.unlinked_list = np->next;
	np->next = NULL;
	np->obj = obj;
	np->size = size;
	np->flags = 0;
	if (vtable)
		gc->follow_scan_size += FLOOR(size);
	else
		np->flags |= ATOM_FLAG;
	memset(obj, 0, size);
	return np;
}
static void insert_node(struct gcdata *gc, struct node *np)
{
	size_t addr = (size_t)np->obj;
	struct node **pnext = &gc->htab.buckets[hash(gc, addr)];
	while ((size_t)(*pnext)->obj > addr)
		pnext = &(*pnext)->next;
	np->next = *pnext;
	*pnext = np;
	if (gc->htab.begin_addr > addr)
		gc->htab.begin_addr = addr;
	if (gc->htab.end_addr < addr)
		gc->htab.end_addr = addr;
	++gc->htab.length;
}
static size_t refill_marked(struct gcdata *gc, size_t max_count)
{
	size_t count = 0;
	struct node *np;
	while (count < max_count && (np = gc->htab.marked_list) != NULL) {
		gc->htab.marked_list = np->next;
		insert_node(gc, np);
		++count;
	}
	return count;
}
static size_t refill_follow(struct gcdata *gc, size_t max_count)
{
	size_t count = 0;
	struct node *np;
	while (count < max_count && (np = gc->htab.follow_list) != NULL) {
		gc->htab.follow_list = np->next;
		insert_node(gc, np);
		++count;
	}
	return count;
}
static size_t refill_some(struct gcdata *gc, size_t max_count)
{
	if (gc->htab.follow_list)
		return refill_follow(gc, max_count);
	else if (gc->htab.marked_list)
		return refill_marked(gc, max_count);
	else
		return 0;
}
static void clear_all(struct gcdata *gc)
{
	struct node **bp = gc->htab.buckets + (1 << gc->htab.log_size);
	while (bp-- > gc->htab.buckets) {
		if (*bp != &sentinel_node) {
			struct node **pnext = bp;
			while (*pnext != &sentinel_node) {
				gc->marked_bytes -= (*pnext)->size;
				pnext = &(*pnext)->next;
			}
			*pnext = gc->htab.free_list;
			gc->htab.free_list = *bp;
			*bp = (struct node *)&sentinel_node;
		}
	}
	gc->htab.begin_addr = ~0u;
	gc->htab.end_addr = 0;
	gc->htab.length = 0;
}
static size_t free_pending(struct gcdata *gc, size_t min_free_bytes)
{
	size_t count = 0;
	struct node *np;
	while ((np = gc->htab.free_list) != NULL) {
		gc->htab.free_list = np->next;
		np->next = gc->htab.unlinked_list;
		gc->htab.unlinked_list = np;
		++count;
		free(np->obj);
		np->obj = NULL;
		if (!(np->flags & ATOM_FLAG))
			gc->follow_scan_size -= FLOOR(np->size);
		np->flags = 0;
		gc->pending_free_size -= np->size;
		gc->free_bytes += np->size;
		np->size = 0;
		if (gc->free_bytes >= min_free_bytes)
			break;
	}
	return count;
}
static void *inner_malloc(struct gcdata *gc, size_t size)
{
	void *ptr;
	if ((ptr = malloc(size)) != NULL)
		if (gc->free_bytes > size)
			gc->free_bytes -= size;
		else {
			gc->total_heap_size += size - gc->free_bytes;
			gc->free_bytes = 0;
		}
	return ptr;
}
static void *inner_gc_malloc(struct gcdata *gc, size_t size)
{
	void *ptr;
	if ((ptr = inner_malloc(gc, size)) == NULL) {
		garbage_collect(gc, LEVEL_FAST);
		if ((ptr = inner_malloc(gc, size)) == NULL) {
			garbage_collect(gc, LEVEL_DEEP);
			ptr = inner_malloc(gc, size);
		}
	}
	return ptr;
}
static size_t mark_objects(struct gcdata *gc, int full)
{
	size_t nobjs_before_gc;
	refill_follow(gc, ~0u);
	if (full)
		refill_marked(gc, ~0u);
	if ((nobjs_before_gc = gc->htab.length) > 0) {
		scan_roots(gc);
		scan_followable(gc);
		gc->marked_bytes += gc->allocd_bytes;
	}
	return nobjs_before_gc;
}
static void sweep_objects(struct gcdata *gc)
{
	gc->pending_free_size += gc->marked_bytes;
	clear_all(gc);
	gc->pending_free_size -= gc->marked_bytes;
}
static int alloc_nodes(struct gcdata *gc)
{
	size_t count = 1 << (gc->htab.log_size - 2);
	size_t bytes = sizeof(struct node_block) + sizeof(struct node) * count;
	struct node_block *bp = inner_gc_malloc(gc, bytes);
	if (bp != NULL) {
		struct node *np = (struct node *)(bp + 1);
		np->next = gc->htab.unlinked_list;
		np->obj = NULL;
		np->flags = 0;
		np->size = 0;
		while (count-- > 1) {
			++np;
			np->next = np - 1;
			np->obj = NULL;
			np->flags = 0;
			np->size = 0;
		}
		gc->htab.unlinked_list = np;
		bp->next = gc->node_block_list;
		gc->node_block_list = bp;
		return 1;
	}
	return 0;
}
static struct node **alloc_buckets(struct gcdata *gc, size_t log_size)
{
	size_t bytes = WSIZE << log_size;
	size_t count = 1 << log_size;
	struct node **buckets = inner_gc_malloc(gc, bytes);
	if (buckets != NULL)
		while (count-- > 0)
			buckets[count] = (struct node *)&sentinel_node;
	return buckets;
}
static void do_htable_resize(struct gcdata *gc, struct node **buckets)
{
	free(gc->htab.buckets);
	gc->htab.buckets = buckets;
	gc->free_bytes += WSIZE << gc->htab.log_size;
	++gc->htab.log_size;
}
static void resize_htable(struct gcdata *gc, size_t nobjs_before_gc)
{
	struct node **buckets;
	if (guess_expand_htable(gc, nobjs_before_gc))
		if ((buckets = alloc_buckets(gc, gc->htab.log_size + 1)) != NULL)
			do_htable_resize(gc, buckets);
}
static int try_htable_resize(struct gcdata *gc)
{
	struct node **buckets = alloc_buckets(gc, gc->htab.log_size + 1);
	if (buckets != NULL) {
		size_t marked_bytes;
		garbage_collect(gc, LEVEL_DEEP);
		gc->htab.free_list = gc->htab.marked_list;
		marked_bytes = gc->marked_bytes;
		clear_all(gc);
		gc->marked_bytes = marked_bytes;
		gc->htab.marked_list = gc->htab.free_list;
		gc->htab.free_list = NULL;
		do_htable_resize(gc, buckets);
		return 1;
	}
	return 0;
}
static void garbage_collect(struct gcdata *gc, enum collect_level level)
{
	size_t nobjs_before_gc;
	switch (level) {
	case LEVEL_FULL:
	case LEVEL_DEEP:
		nobjs_before_gc = mark_objects(gc, level == LEVEL_FULL);
		if (gc->htab.length > 0)
			sweep_objects(gc);
		else
			resize_htable(gc, nobjs_before_gc);
		gc->allocd_before_gc += gc->allocd_bytes;
		gc->allocd_bytes = 0;
		/* fall through */
	case LEVEL_FAST:
	case LEVEL_LITTLE:
		free_pending(gc, level == LEVEL_LITTLE ? 0 : ~0u);
		break;
	}
}
static int insert_static_root(struct gcdata *gc, struct static_root **pnext,
	size_t begin_addr, size_t end_addr)
{
	struct static_root *np = inner_gc_malloc(gc, sizeof(struct static_root));
	if (np != NULL) {
		gc->static_size = end_addr - begin_addr;
		np->begin_addr = begin_addr;
		np->end_addr = end_addr;
		np->next = *pnext;
		*pnext = np;
		return 1;
	}
	return 0;
}
static int merge_static_root(struct gcdata *gc, struct static_root **pnext,
	size_t begin_addr, size_t end_addr)
{
	if ((*pnext)->begin_addr > begin_addr) {
		gc->static_size += (*pnext)->begin_addr - begin_addr;
		(*pnext)->begin_addr = begin_addr;
	}
	if ((*pnext)->end_addr < end_addr) {
		struct static_root *np = (*pnext)->next;
		while (np && end_addr >= np->begin_addr) {
			struct static_root *next = np->next;
			if (end_addr < np->end_addr)
				end_addr = np->end_addr;
			gc->static_size -= np->end_addr - np->begin_addr;
			gc->free_bytes  += sizeof(struct static_root);
			free(np);
			np = next;
		}
		*pnext = np;
		gc->static_size += end_addr - (*pnext)->end_addr;
		(*pnext)->end_addr = end_addr;
	}
	return 1;
}
static int add_static_root(struct gcdata *gc, size_t begin_addr, size_t end_addr)
{
	struct static_root **pnext = &gc->statics;
	while (*pnext && (*pnext)->end_addr < begin_addr)
		pnext = &(*pnext)->next;
	if (!*pnext || (*pnext)->begin_addr > end_addr)
		return insert_static_root(gc, pnext, begin_addr, end_addr);
	else
		return merge_static_root(gc, pnext, begin_addr, end_addr);
}
static struct gcdata *alloc_gcdata(struct gcdata **pgcdata)
{
	struct gcdata *gc = malloc(sizeof(struct gcdata));
	if (gc != NULL) {
		memset(gc, 0, sizeof(struct gcdata));
		if ((gc->htab.buckets = alloc_buckets(gc, LOG_SIZE)) != NULL) {
			gc->htab.begin_addr = ~0u;
			gc->htab.end_addr = 0;
			gc->htab.log_size = LOG_SIZE;
			if ((size_t)pgcdata < approx_sp(gc))
				gc->stack.stack_grows_up = 1;
			gc->stack.begin_addr = ~0u;
			gc->stack.end_addr = 0;
		} else {
			free(gc);
			gc = NULL;
		}
	}
	return gc;
}
static void acquire_collector(struct gcdata **pgcdata)
{
	struct gcdata *gc;
	if ((gc = global_gcdata) == NULL) {
		if ((gc = alloc_gcdata(pgcdata)) == NULL) {
			fprintf(stderr, "error: gc: initialize failed\n");
			exit(-1);
		}
		global_gcdata = gc;
	}
	if (gc->stack.begin_addr > (size_t)pgcdata)
		gc->stack.begin_addr = (size_t)pgcdata - WSIZE;
	if (gc->stack.end_addr < (size_t)pgcdata)
		gc->stack.end_addr = (size_t)pgcdata + (WSIZE << 1);
	*pgcdata = gc;
}
static void expand_heap(struct gcdata *gc, size_t incsize)
{
	void *ptr;
	incsize = incsize > (WSIZE << LOG_OFFSET_IGNORE) ? CEIL(incsize) : (WSIZE << LOG_OFFSET_IGNORE);
	gc->expanded_heap_size = gc->total_heap_size + incsize;
	while ((ptr = malloc(gc->free_bytes + incsize)) == NULL && (incsize >>= 1) != 0)
		;
	if (ptr != NULL) {
		free(ptr);
		gc->free_bytes += incsize;
		gc->total_heap_size += incsize;
		gc->expanded_heap_size = gc->total_heap_size;
	}
}
static int alloc_begin(struct gcdata *gc, size_t size)
{
	int level = LEVEL_LITTLE;
	size_t incsize;
	if (!gc->allocd_bytes)
		;
	else if (guess_expand_htable(gc, gc->htab.length)) {
		garbage_collect(gc, LEVEL_DEEP);
		if (try_htable_resize(gc))
			level = LEVEL_DEEP;
	} else if (guess_collect(gc, size)) {
		garbage_collect(gc, LEVEL_DEEP);
		level = LEVEL_DEEP;
	} else if ((incsize = guess_expand_heap(gc, size)) != 0 &&
		gc->expanded_heap_size <= gc->total_heap_size)
		expand_heap(gc, incsize);
	return level;
}
static void alloc_end(struct gcdata *gc, int level)
{
	if (level < LEVEL_DEEP) {
		refill_some(gc, 10);
		if (gc->htab.free_list)
			garbage_collect(gc, LEVEL_LITTLE);
	}
}
static void *general_malloc(struct gcdata *gc, size_t size, size_t vtable)
{
	void *obj = NULL;
	int level = alloc_begin(gc, size);
	while (level <= LEVEL_FULL) {
		if (gc->htab.unlinked_list || alloc_nodes(gc))
			obj = inner_gc_malloc(gc, size);
		if (obj != NULL)
			break;
		garbage_collect(gc, (enum collect_level)level++);
	}
	if (obj == NULL) {
		fprintf(stderr, "error: gc: out of memory\n");
		exit(-1);
	}
	gc->allocd_bytes += size;
	insert_node(gc, alloc_node(gc, obj, size, vtable));
	alloc_end(gc, level);
	return obj;
}
static void gc_uninitialize(void)
{
	struct static_root *np;
	struct node_block *bp;
	struct gcdata *gc;
	acquire_collector(&gc);
	np = gc->statics;
	while (np != NULL) {
		struct static_root *next = np->next;
		free(np);
		np = next;
	}
	gc->statics = NULL;
	garbage_collect(gc, LEVEL_FULL);
	free_pending(gc, ~0u);
	bp = gc->node_block_list;
	while (bp != NULL) {
		struct node_block *next = bp->next;
		free(bp);
		bp = next;
	}
	gc->node_block_list = NULL;
	free(gc->htab.buckets);
	gc->htab.buckets = NULL;
	free(gc);
}
extern void gc_initialize(void)
{
	struct gcdata *gc;
	acquire_collector(&gc);
	atexit(gc_uninitialize);
#if _MSC_VER >= 1200
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_DELAY_FREE_MEM_DF
		| _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDERR);
#endif
}
extern void *alloc(size_t size)
{
	if (size > 0) {
		struct gcdata *gc;
		acquire_collector(&gc);
		return general_malloc(gc, size, ~0u);
	}
	return NULL;
}
extern size_t get_heap_size(void)
{
	struct gcdata *gc;
	acquire_collector(&gc);
	return gc->marked_bytes + gc->allocd_bytes +
		gc->pending_free_size + gc->free_bytes;
}
extern void register_root(void *low, void *high)
{
	if (low && high) {
		size_t begin_addr = CEIL((size_t)low);
		size_t end_addr = FLOOR((size_t)high);
		if (begin_addr < end_addr) {
			struct gcdata *gc;
			acquire_collector(&gc);
			add_static_root(gc, begin_addr, end_addr);
		}
	}
}

