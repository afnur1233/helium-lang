#pragma once
#include <stdlib.h>
#include "int.c"

typedef struct Str {
	u8 *buf;
	u64 len;
	u64 cap;
} Str;

typedef struct StrSlice {
	u8 *buf;
	u64 len;
} StrSlice;

enum: i32 {
	str_result_ok,
	str_result_malloc,
	str_result_fgetpos,
	str_result_fsetpos,
	str_result_fseek,
	str_result_fread,
};
typedef int StrResult;

#define str_slice_from_lit(...)_str_slice_from_lit_par((__VA_ARGS__))
#define _str_slice_from_lit_par(par)_str_slice_from_lit par
#define _str_slice_from_lit(...)((StrSlice){ .buf = #__VA_ARGS__, .len = sizeof(StrSlice) - 1 })

StrResult str_alloc(Str *str, u64 len) {
	u64 free_space = str->cap - str->len;
	if (free_space >= len) {
		return str_result_ok;
	}
	
	u64 cap = str->len + len;
	if (str->buf == NULL) {
		str->buf = (u8*)malloc(sizeof(*str->buf) * cap);
	} else {
		str->buf = (u8*)realloc(str->buf, sizeof(*str->buf) * cap);
	}
	if (str->buf == NULL) {
		return str_result_malloc;
	}
	
	return str_result_ok;
}

StrResult str_append(Str *str, u8 *buf, u64 len) {
	StrResult res = str_alloc(str, len);
	if (res != str_result_ok) {
		return res;
	}
	
	for (u64 i = 0; i < len; i++) {
		str->buf[str->len++] = buf[i];
	}
	
	return str_result_ok;
}

void str_free(Str *str) {
	if (str->buf != NULL) {
		free(str->buf);
		str->buf = NULL;
		str->len = 0;
		str->cap = 0;
	}
}

StrResult str_read_entire_file(Str *str, FILE *stream) {
	fpos_t original;
	if (fgetpos(stream, &original) != 0) {
		return str_result_fgetpos;
	}
	if (fseek(stream, 0L, SEEK_END) != 0) {
		return str_result_fseek;
	}
	long size = ftell(stream);
	
	if (fsetpos(stream, &original) != 0) {
		return str_result_fsetpos;
	}
	
	StrResult res = str_alloc(str, size +1);
	if (res != str_result_ok) {
		return res;
	}
	
	u8 *write_pos = str->buf + str->len;
	u64 read_bytes = 0;
	while (read_bytes < size) {
		u64 read_bytes_this_iter = fread(write_pos, 1, size - read_bytes, stream);
		if (ferror(stream)) {
			//str_shrink_to_fit(str);
			return str_result_fread;
		}
		
		read_bytes += read_bytes_this_iter;
		write_pos += read_bytes_this_iter;
		str->len += read_bytes_this_iter;
		
		if (feof(stream)) {
			break;
		}
	}
	
	return str_result_ok;
}

inline __attribute__((always_inline)) StrSlice str_slice(const Str *str, u64 start, u64 len) {
	if (start >= str->len) {
		return (StrSlice){ .buf = str->buf + str->len, .len = 0 };
	}
	u64 end = start + len;
	if (end >= str->len) {
		end = str->len;
	}
	
	return (StrSlice){ .buf = str->buf + start, .len = end - start, };
}

inline __attribute__((always_inline)) StrSlice str_slice_slice(const StrSlice *str, u64 start, u64 len) {
	if (start >= str->len) {
		return (StrSlice){ .buf = str->buf + str->len, .len = 0 };
	}
	u64 end = start + len;
	if (end >= str->len) {
		end = str->len;
	}
	
	return (StrSlice){ .buf = str->buf + start, .len = end - start, };
}

inline __attribute__((always_inline)) StrSlice str_slice_until_delim(const Str *str, u64 offset, u8 delim) {
	if (offset >= str->len) {
		return (StrSlice){ .buf = str->buf + str->len, .len = 0 };
	}
	
	u64 len = 0;
	while (str->buf[len + offset] != delim) {
		len++;
	}
	
	return (StrSlice){ .buf = str->buf + offset, .len = len };
}

inline __attribute__((always_inline)) StrSlice str_slice_slice_until_delim(const StrSlice *str, u64 offset, u8 delim) {
	if (offset >= str->len) {
		return (StrSlice){ .buf = str->buf + str->len, .len = 0 };
	}
	
	u64 len = 0;
	while (str->buf[len + offset] != delim) {
		len++;
	}
	
	return (StrSlice){ .buf = str->buf + offset, .len = len };
}

inline __attribute__((always_inline)) u8 str_at(const Str *str, u64 i) {
	if (i >= str->len) {
		return '\0';
	}
	
	return str->buf[i];
}

inline __attribute__((always_inline)) void str_set(Str *str, u64 i, u8 ch) {
	if (i >= str->len) {
		return;
	}
	
	str->buf[i] = ch;
}

inline __attribute__((always_inline)) u8 str_slice_at(const StrSlice *slice, u64 i) {
	if (i >= slice->len) {
		return '\0';
	}
	
	return slice->buf[i];
}

inline __attribute__((always_inline)) void str_slice_set(StrSlice *slice, u64 i, u8 ch) {
	if (i >= slice->len) {
		return;
	}
	
	slice->buf[i] = ch;
}

inline __attribute__((always_inline)) bool str_eq(const Str *a, const Str *b) {
	if (a->len != b->len) {
		return false;
	}
	
	for (u64 i = 0; i < a->len; i++) {
		if (a->buf[i] != b->buf[i]) {
			return false;
		}
	}
	
	return true;
}

inline __attribute__((always_inline)) bool str_slice_eq(const StrSlice *a, const StrSlice *b) {
	if (a->len != b->len) {
		return false;
	}
	
	for (u64 i = 0; i < a->len; i++) {
		if (a->buf[i] != b->buf[i]) {
			return false;
		}
	}
	
	return true;
}
