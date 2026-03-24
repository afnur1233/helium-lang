#ifndef _INCLUDE_MEM_C
#define _INCLUDE_MEM_C

#include <stdlib.h>

struct allocator {
    void *inner;
    void *(*alloc)(struct allocator *, size_t size);
    void *(*realloc)(struct allocator *, void *mem, size_t old, size_t new);
    void *(*free)(struct allocator *, void *mem, size_t size);
};

void *_allocator_libc_alloc(struct allocator *_, size_t size) {
    return malloc(size);
}

void *_allocator_libc_realloc(struct allocator *_, void *mem, size_t old, size_t new) {
    return realloc(mem, new);
}

void *_allocator_libc_free(struct allocator *_, void *mem, size_t size) {
    if (mem == NULL) {
        return NULL;
    }
    free(mem);
    return NULL;
}

struct allocator allocator_libc = {
    .inner = NULL,
    .alloc = _allocator_libc_alloc,
    .realloc = _allocator_libc_realloc,
    .free = _allocator_libc_free,
};


struct allocator_fixed_arena {
    char *buffer;
    size_t size;
    size_t position;
};

void *_allocator_fixed_arena_alloc(struct allocator *allocator, size_t size) {
    struct allocator_fixed_arena *self = (struct allocator_fixed_arena *)allocator->inner;
    if (size > self->size - self->position) {
        return NULL;
    }
    
    void *mem = self->buffer + self->position;
    self->position += size;
    
    return mem;
}

void *_allocator_fixed_arena_realloc(struct allocator *allocator, void *mem, size_t old, size_t new) {
    return _allocator_fixed_arena_alloc(allocator, new);
}

struct allocator allocator_from_fixed_arena(struct allocator_fixed_arena *allocator) {
    return (struct allocator){
        .inner = (void *)allocator,
        .alloc = _allocator_fixed_arena_alloc,
        .realloc = _allocator_fixed_arena_realloc,
        .free = NULL,
    };
}

struct allocator_tracking {
    struct allocator *allocator;
    struct {
        size_t calls_alloc;
        size_t calls_realloc;
        size_t calls_free;
    } stats;
};

void *_allocator_tracking_alloc(struct allocator *allocator, size_t size) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_alloc += 1;
    return self->allocator->alloc(self->allocator, size);
}

void *_allocator_tracking_realloc(struct allocator *allocator, void *mem, size_t old, size_t new) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_realloc += 1;
    return self->allocator->realloc(self->allocator, mem, old, new);;
}

void *_allocator_tracking_free(struct allocator *allocator, void *mem, size_t size) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_free += 1;
    return self->allocator->free(self->allocator, mem, size);
}

struct allocator allocator_from_tracking(struct allocator_tracking *tracking) {
    struct allocator *allocator = tracking->allocator;
    return (struct allocator){
        .inner = (void *)tracking,
        .alloc = (allocator->alloc) ? _allocator_tracking_alloc : NULL,
        .realloc = (allocator->realloc) ? _allocator_tracking_realloc : NULL,
        .free = (allocator->free) ? _allocator_tracking_free : NULL,
    };
}

#endif // _INCLUDE_MEM_C