#pragma once
#include <stdlib.h>

#define arr_sizeof(xs)((sizeof(xs))/(sizeof(xs[0])))

struct allocator {
    void *(*alloc)(struct allocator *, size_t size, size_t align);
    void *(*realloc)(struct allocator *, void *mem, size_t align, size_t old, size_t new);
    void *(*resize)(struct allocator *, void *mem, size_t align, size_t old, size_t new);
    bool  (*can_resize)(struct allocator *, void *mem, size_t align, size_t old, size_t new);
    void *(*free)(struct allocator *, void *mem, size_t size, size_t align);
    void *inner;
};

size_t align_number_to_alignment(size_t number, size_t alignment) {
    if (number == 0) {
        return number;
    }
    
    size_t align_offset = alignment - (number % alignment);
    
    return number + align_offset;
}

bool _mem_noresize(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    return false;
}

void *_allocator_libc_alloc(struct allocator *_, size_t size, size_t align) {
    return malloc(size);
}

void *_allocator_libc_realloc(struct allocator *_, void *mem, size_t align, size_t old, size_t new) {
    return realloc(mem, new);
}

void *_allocator_libc_free(struct allocator *_, void *mem, size_t size, size_t align) {
    if (mem == NULL) {
        return NULL;
    }
    free(mem);
    return NULL;
}

struct allocator allocator_libc = {
    .alloc = _allocator_libc_alloc,
    .realloc = _allocator_libc_realloc,
    .resize = NULL,
    .can_resize = _mem_noresize,
    .free = _allocator_libc_free,
    .inner = NULL,
};


struct allocator_fixed_arena {
    char *buffer;
    size_t size;
    size_t position;
};

void *_allocator_fixed_arena_alloc(struct allocator *allocator, size_t size, size_t align) {
    struct allocator_fixed_arena *self = (struct allocator_fixed_arena *)allocator->inner;
    
    size_t aligned_position = align_number_to_alignment(self->position, align);
    size_t free_space = self->size - aligned_position;
    if (size > free_space) {
        return NULL;
    }
    
    self->position = aligned_position + size;
    return self->buffer + aligned_position;
}

void *_allocator_fixed_arena_realloc(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    return _allocator_fixed_arena_alloc(allocator, new, align);
}

void *_allocator_fixed_arena_resize(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    struct allocator_fixed_arena *self = (struct allocator_fixed_arena *)allocator->inner;
    
    size_t mem_position = (char *)mem - self->buffer;
    if (mem_position >= self->size) {
        return NULL;
    }
    
    size_t mem_position_end = mem_position + old;
    if (mem_position_end != self->position) {
        return NULL;
    }
    
    size_t new_position = self->position + new;
    if (new_position >= self->size) {
        return NULL;
    }
    
    return mem;
}

bool _allocator_fixed_arena_can_resize(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    struct allocator_fixed_arena *self = (struct allocator_fixed_arena *)allocator->inner;
    
    size_t mem_position = (char *)mem - self->buffer;
    if (mem_position >= self->size) {
        return false;
    }
    
    size_t mem_position_end = mem_position + old;
    if (mem_position_end != self->position) {
        return false;
    }
    
    size_t new_position = self->position + new;
    if (new_position >= self->size) {
        return false;
    }
    
    return true;
}

struct allocator allocator_from_fixed_arena(struct allocator_fixed_arena *allocator) {
    return (struct allocator){
        .alloc = _allocator_fixed_arena_alloc,
        .realloc = _allocator_fixed_arena_realloc,
        .resize = _allocator_fixed_arena_resize,
        .can_resize = _allocator_fixed_arena_can_resize,
        .free = NULL,
        .inner = (void *)allocator,
    };
}

struct allocator_tracking {
    struct allocator *allocator;
    struct {
        size_t calls_alloc;
        size_t calls_realloc;
        size_t calls_resize;
        size_t calls_can_resize;
        size_t calls_free;
        size_t bytes_allocated;
        size_t bytes_freed;
        size_t bytes_live;
        size_t bytes_peak;
    } stats;
};

void *_allocator_tracking_alloc(struct allocator *allocator, size_t size, size_t align) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_alloc += 1;
    
    void *ptr = self->allocator->alloc(self->allocator, size, align);
    if (ptr != NULL) {
        self->stats.bytes_allocated += size;
        self->stats.bytes_live += size;
        if (self->stats.bytes_allocated > self->stats.bytes_peak) {
            self->stats.bytes_peak = self->stats.bytes_allocated;
        }
    }
    
    return ptr;
}

void *_allocator_tracking_realloc(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_realloc += 1;
    
    void *ptr = self->allocator->realloc(self->allocator, mem, align, old, new);
    if (ptr != NULL) {
        if (old > new) {
            self->stats.bytes_allocated -= old - new;
            self->stats.bytes_live -= old - new;
        } else {
            self->stats.bytes_allocated += new - old;
            self->stats.bytes_live += new - old;
        }
        if (self->stats.bytes_allocated > self->stats.bytes_peak) {
            self->stats.bytes_peak = self->stats.bytes_allocated;
        }
    }
    
    return ptr;
}

void *_allocator_tracking_resize(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_resize += 1;
    
    void *ptr = self->allocator->resize(self->allocator, mem, align, old, new);
    if (ptr != NULL) {
        if (old > new) {
            self->stats.bytes_allocated -= old - new;
            self->stats.bytes_live -= old - new;
        } else {
            self->stats.bytes_allocated += new - old;
            self->stats.bytes_live += new - old;
        }
        if (self->stats.bytes_allocated > self->stats.bytes_peak) {
            self->stats.bytes_peak = self->stats.bytes_allocated;
        }
    }
    
    return ptr;
}

bool  _allocator_tracking_can_resize(struct allocator *allocator, void *mem, size_t align, size_t old, size_t new) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_can_resize += 1;
    
    return self->allocator->can_resize(self->allocator, mem, align, old, new);
}

void *_allocator_tracking_free(struct allocator *allocator, void *mem, size_t size, size_t align) {
    struct allocator_tracking *self = (struct allocator_tracking *)allocator->inner;
    self->stats.calls_free += 1;
    self->stats.bytes_freed += size;
    self->stats.bytes_live -= size;
    
    return self->allocator->free(self->allocator, mem, size, align);
}

struct allocator allocator_from_tracking(struct allocator_tracking *tracking) {
    struct allocator *allocator = tracking->allocator;
    return (struct allocator){
        .alloc = (allocator->alloc) ? _allocator_tracking_alloc : NULL,
        .realloc = (allocator->realloc) ? _allocator_tracking_realloc : NULL,
        .resize = (allocator->resize) ? _allocator_tracking_resize : NULL,
        .can_resize = (allocator->can_resize) ? _allocator_tracking_can_resize : NULL,
        .free = (allocator->free) ? _allocator_tracking_free : NULL,
        .inner = (void *)tracking,
    };
}