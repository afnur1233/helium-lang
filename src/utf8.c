#pragma once
#include <int.c>
#include <str.c>
#include <mem.c>
#include <ctype.h>

struct iterator_utf8 {
    struct str_slice slice;
    size_t pos;
};

typedef u32 utf8_codepoint_t;

bool utf8_isnewln(utf8_codepoint_t ch) {
    return
        ch == 0x000A ||
        ch == 0x000B ||
        ch == 0x000C ||
        ch == 0x000D ||
        ch == 0x0085 ||
        ch == 0x2028 ||
        ch == 0x2029;
}

bool utf8_isspace(utf8_codepoint_t ch) {
    return
        utf8_isnewln(ch) ||
        ch == 0x0009 ||
        ch == 0x0020 ||
        ch == 0x00A0 ||
        ch == 0x1680 ||
        ch == 0x2000 ||
        ch == 0x2001 ||
        ch == 0x2002 ||
        ch == 0x2003 ||
        ch == 0x2004 ||
        ch == 0x2005 ||
        ch == 0x2006 ||
        ch == 0x2007 ||
        ch == 0x2008 ||
        ch == 0x2009 ||
        ch == 0x200A ||
        ch == 0x202F ||
        ch == 0x205F ||
        ch == 0x3000;
}

bool iterator_utf8_can_eat(struct iterator_utf8 *iterator) {
    if (iterator == NULL) return false;
    return !(iterator->pos >= iterator->slice.len);
}

size_t iterator_utf8_get_next_len(struct iterator_utf8 *iterator) {
    if (!iterator_utf8_can_eat(iterator)) {
        return 0;
    }
    
    char ch = str_slice_at(&iterator->slice, iterator->pos);
    
    if ((ch & 0b11100000) == 0b11000000) {
        return 2;
    }
    
    if ((ch & 0b11110000) == 0b11100000) {
        return 3;
    }
    
    if ((ch & 0b11111000) == 0b11110000) {
        return 4;
    }
    
    return 1;
}

utf8_codepoint_t iterator_utf8_peek(struct iterator_utf8 *iterator) {
    if (!iterator_utf8_can_eat(iterator)) {
        return '\0';
    }
    
    utf8_codepoint_t codepoint = str_slice_at(&iterator->slice, iterator->pos);
    size_t len = iterator_utf8_get_next_len(iterator);
    
    codepoint &= 0xFF >> len;
    
    for (size_t i = 1; i < len; i++) {
        codepoint <<= 6;
        codepoint |= str_slice_at(&iterator->slice, iterator->pos + i) & 0b00111111;
    }
    
    return codepoint;
}

utf8_codepoint_t iterator_utf8_eat(struct iterator_utf8 *iterator) {
    utf8_codepoint_t codepoint = iterator_utf8_peek(iterator);
    iterator->pos += iterator_utf8_get_next_len(iterator);
    
    return codepoint;
}