#pragma once
#include <int.c>

struct helium_loc {
    struct str_slice file_path;
    size_t line_num;
    size_t line_begin;
    size_t column;
    size_t pos;
    size_t len;
};

struct helium_lexer_loc {
    struct str_slice file_path;
    size_t line_num;
    size_t line_begin;
    size_t column;
    size_t pos;
};