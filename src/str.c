#pragma once
#include <mem.c>

struct str {
    char *buf;
    size_t len;
    size_t cap;
};

struct str_slice {
    char *buf;
    size_t len;
};

#define STR_SLICE_LIT(...)((struct str_slice){.buf=(__VA_ARGS__),.len=sizeof(__VA_ARGS__)-1})

bool str_reserve(struct allocator *allocator, struct str *str, size_t new_cap) {
    if (
        allocator == NULL ||
        str == NULL ||
        allocator->alloc == NULL ||
        allocator->realloc == NULL
    ) return false;
    
    if (new_cap == str->cap) {
        return true;
    }
    
    if (str->buf == NULL) {
        str->buf = allocator->alloc(
            allocator,
            sizeof(*str->buf) * new_cap,
            alignof(str->buf)
        );
    } else if (str->cap < new_cap) {
        if (
            allocator->can_resize != NULL &&
            allocator->resize != NULL &&
            allocator->can_resize(
                allocator,
                str->buf,
                alignof(str->buf),
                sizeof(*str->buf) * str->cap,
                sizeof(*str->buf) * new_cap
            )
        ) {
            str->buf = allocator->resize(
                allocator,
                str->buf,
                alignof(str->buf),
                sizeof(*str->buf) * str->cap,
                sizeof(*str->buf) * new_cap
            );
        } else {
            str->buf = allocator->realloc(
                allocator,
                str->buf,
                alignof(str->buf),
                sizeof(*str->buf) * str->cap,
                sizeof(*str->buf) * new_cap
            );
        }
    }
    
    if (str->buf == NULL) {
        str->len = 0;
        str->cap = 0;
        return false;
    }
    str->cap = new_cap;
    
    return true;
}

bool str_append(struct allocator *allocator, struct str *str, char *buf, size_t len) {
    if (
        allocator == NULL ||
        str == NULL ||
        allocator->alloc == NULL ||
        allocator->realloc == NULL
    ) return false;
    
    if (!str_reserve(allocator, str, str->len + len)) {
        return false;
    }
    
    for (size_t i = 0; i < len; i++) {
        str->buf[str->len++] = buf[i];
    }
    
    return true;
}

void str_clear(struct str *str) {
    if (
        str == NULL
    ) return;
    
    str->len = 0;
}

void str_free(struct allocator *allocator, struct str *str) {
    if (
        str == NULL ||
        str->buf == NULL ||
        allocator->free == NULL
    ) return;
    
    str->buf = allocator->free(
        allocator,
        str->buf,
        sizeof(*str->buf) * str->cap,
        alignof(str->buf)
    );
    str->cap = 0;
    str->len = 0;
}

char str_at(struct str *str, size_t i) {
    if (i >= str->len) {
        return '\0';
    }
    return str->buf[i];
}

char str_slice_at(struct str_slice *slice, size_t i) {
    if (i >= slice->len) {
        return '\0';
    }
    return slice->buf[i];
}

char str_set(struct str *str, size_t i, char ch) {
    if (i >= str->len) {
        return '\0';
    }
    return str->buf[i] = ch;
}

char str_slice_set(struct str_slice *slice, size_t i, char ch) {
    if (i >= slice->len) {
        return '\0';
    }
    return slice->buf[i] = ch;
}

bool str_slice_eq(struct str_slice *a, struct str_slice *b) {
    if (a == NULL || b == NULL || a->len != b->len) {
        return false;
    }
    
    for (size_t i = 0; i < a->len; i++) {
        if (a->buf[i] != b->buf[i]) {
            return false;
        }
    }
    
    return true;
}