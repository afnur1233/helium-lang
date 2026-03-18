#pragma once
#include <int.c>
#include <str.c>
#include <utf8.c>
#include <helium/loc.c>

enum helium_token_type : int {
    helium_token_type_end,
    helium_token_type_shebang,
    helium_token_type_comment,
    helium_token_type_unkown,
    helium_token_type_ch,
    helium_token_type_ch_unfinished,
    helium_token_type_ch_too_long,
    helium_token_type_str,
    helium_token_type_str_unfinished,
    helium_token_type_str_multiline,
    helium_token_type_ident,
    helium_token_type_num,
    helium_token_type_num_malformed,
    helium_token_type_paren_round_open,
    helium_token_type_paren_round_close,
    helium_token_type_paren_block_open,
    helium_token_type_paren_block_close,
    helium_token_type_paren_curly_open,
    helium_token_type_paren_curly_close,
    helium_token_type_plus,
    helium_token_type_minus,
    helium_token_type_percent,
    helium_token_type_slash,
    helium_token_type_star,
    helium_token_type_bitwise_not,
    helium_token_type_bitwise_xor,
    helium_token_type_bitwise_and,
    helium_token_type_bitwise_or,
    helium_token_type_bitwise_shift_left,
    helium_token_type_bitwise_shift_right,
    helium_token_type_plus_equals,
    helium_token_type_minus_equals,
    helium_token_type_percent_equals,
    helium_token_type_slash_equals,
    helium_token_type_star_equals,
    helium_token_type_bitwise_xor_equals,
    helium_token_type_bitwise_and_equals,
    helium_token_type_bitwise_or_equals,
    helium_token_type_bitwise_shift_left_equals,
    helium_token_type_bitwise_shift_right_equals,
    helium_token_type_colon,
    helium_token_type_equals,
    helium_token_type_equals_equals,
    helium_token_type_not_equals,
    helium_token_type_less,
    helium_token_type_greater,
    helium_token_type_less_equals,
    helium_token_type_greater_equals,
    helium_token_type_exclamation_mark,
    helium_token_type_question_mark,
    helium_token_type_dot,
    helium_token_type_dot_dot,
    helium_token_type_dot_dot_equals,
    helium_token_type_dot_dot_dot,
    helium_token_type_comma,
    helium_token_type_semi_colon,
    helium_token_type_keyword_nothing,
    helium_token_type_keyword_not,
    helium_token_type_keyword_and,
    helium_token_type_keyword_or,
    helium_token_type_keyword_if,
    helium_token_type_keyword_else,
    helium_token_type_keyword_enum,
    helium_token_type_keyword_union,
    helium_token_type_keyword_struct,
    helium_token_type_keyword_type,
    helium_token_type_keyword_typeof,
    helium_token_type_keyword_var,
    helium_token_type_keyword_const,
    helium_token_type_keyword_runtime,
    helium_token_type_keyword_comptime,
    helium_token_type_keyword_static,
    helium_token_type_keyword_for,
    helium_token_type_keyword_while,
    helium_token_type_keyword_return,
    helium_token_type_keyword_continue,
    helium_token_type_keyword_break,
    helium_token_type_keyword_func,
    helium_token_type_keyword_pub,
};

struct helium_token {
    enum helium_token_type type;
    struct helium_loc loc;
    struct str_slice data;
};

struct helium_lexer {
    struct iterator_utf8 iterator;
    struct helium_lexer_loc loc;
};

struct helium_keyword {
    enum helium_token_type type;
    struct str_slice value;
};

#define X(strlit, T) ((struct helium_keyword){ .type = T, .value = STR_SLICE_LIT(strlit) })
struct helium_keyword helium_keywords[] = {
    X("not", helium_token_type_keyword_not),
    X("and", helium_token_type_keyword_and),
    X("or", helium_token_type_keyword_or),
    X("nothing", helium_token_type_keyword_nothing),
    X("if", helium_token_type_keyword_if),
    X("else", helium_token_type_keyword_else),
    X("enum", helium_token_type_keyword_enum),
    X("union", helium_token_type_keyword_union),
    X("struct", helium_token_type_keyword_struct),
    X("type", helium_token_type_keyword_type),
    X("typeof", helium_token_type_keyword_typeof),
    X("var", helium_token_type_keyword_var),
    X("const", helium_token_type_keyword_const),
    X("runtime", helium_token_type_keyword_runtime),
    X("comptime", helium_token_type_keyword_comptime),
    X("static", helium_token_type_keyword_static),
    X("for", helium_token_type_keyword_for),
    X("while", helium_token_type_keyword_while),
    X("return", helium_token_type_keyword_return),
    X("continue", helium_token_type_keyword_continue),
    X("break", helium_token_type_keyword_break),
    X("func", helium_token_type_keyword_func),
    X("pub", helium_token_type_keyword_pub)
};
#undef X

const char *helium_str_token_type(enum helium_token_type type) {
    switch (type) {
        case helium_token_type_end: return "end";
        case helium_token_type_shebang: return "shebang";
        case helium_token_type_comment: return "comment";
        case helium_token_type_unkown: return "unkown";
        case helium_token_type_ch: return "ch";
        case helium_token_type_ch_unfinished: return "ch(unfinished)";
        case helium_token_type_ch_too_long: return "ch(too_long)";
        case helium_token_type_str: return "str";
        case helium_token_type_str_unfinished: return "str(unfinished)";
        case helium_token_type_str_multiline: return "str_multiline";
        case helium_token_type_ident: return "ident";
        case helium_token_type_num: return "num";
        case helium_token_type_num_malformed: return "num(malformed)";
        case helium_token_type_paren_round_open: return "paren_round_open";
        case helium_token_type_paren_round_close: return "paren_round_close";
        case helium_token_type_paren_block_open: return "paren_block_open";
        case helium_token_type_paren_block_close: return "paren_block_close";
        case helium_token_type_paren_curly_open: return "paren_curly_open";
        case helium_token_type_paren_curly_close: return "paren_curly_close";
        case helium_token_type_plus: return "plus";
        case helium_token_type_minus: return "minus";
        case helium_token_type_percent: return "percent";
        case helium_token_type_slash: return "slash";
        case helium_token_type_star: return "star";
        case helium_token_type_bitwise_not: return "bitwise_not";
        case helium_token_type_bitwise_xor: return "bitwise_xor";
        case helium_token_type_bitwise_and: return "bitwise_and";
        case helium_token_type_bitwise_or: return "bitwise_or";
        case helium_token_type_bitwise_shift_left: return "bitwise_shift_left";
        case helium_token_type_bitwise_shift_right: return "bitwise_shift_right";
        case helium_token_type_plus_equals: return "plus_equals";
        case helium_token_type_minus_equals: return "minus_equals";
        case helium_token_type_percent_equals: return "percent_equals";
        case helium_token_type_slash_equals: return "slash_equals";
        case helium_token_type_star_equals: return "star_equals";
        case helium_token_type_bitwise_xor_equals: return "bitwise_xor_equals";
        case helium_token_type_bitwise_and_equals: return "bitwise_and_equals";
        case helium_token_type_bitwise_or_equals: return "bitwise_or_equals";
        case helium_token_type_bitwise_shift_left_equals: return "bitwise_shift_left_equals";
        case helium_token_type_bitwise_shift_right_equals: return "bitwise_shift_right_equals";
        case helium_token_type_colon: return "colon";
        case helium_token_type_equals: return "equals";
        case helium_token_type_equals_equals: return "equals_equals";
        case helium_token_type_not_equals: return "not_equals";
        case helium_token_type_less: return "less";
        case helium_token_type_greater: return "greater";
        case helium_token_type_less_equals: return "less_equals";
        case helium_token_type_greater_equals: return "greater_equals";
        case helium_token_type_exclamation_mark: return "exclamation_mark";
        case helium_token_type_question_mark: return "question_mark";
        case helium_token_type_dot: return "dot";
        case helium_token_type_dot_dot: return "dot_dot";
        case helium_token_type_dot_dot_equals: return "dot_dot_equals";
        case helium_token_type_dot_dot_dot: return "dot_dot_dot";
        case helium_token_type_comma: return "comma";
        case helium_token_type_semi_colon: return "semi_colon";
        case helium_token_type_keyword_not: return "keyword(not)";
        case helium_token_type_keyword_and: return "keyword(and)";
        case helium_token_type_keyword_or: return "keyword(or)";
        case helium_token_type_keyword_nothing: return "keyword(nothing)";
        case helium_token_type_keyword_if: return "keyword(if)";
        case helium_token_type_keyword_else: return "keyword(else)";
        case helium_token_type_keyword_enum: return "keyword(enum)";
        case helium_token_type_keyword_union: return "keyword(union)";
        case helium_token_type_keyword_struct: return "keyword(struct)";
        case helium_token_type_keyword_type: return "keyword(type)";
        case helium_token_type_keyword_typeof: return "keyword(typeof)";
        case helium_token_type_keyword_var: return "keyword(var)";
        case helium_token_type_keyword_const: return "keyword(const)";
        case helium_token_type_keyword_runtime: return "keyword(runtime)";
        case helium_token_type_keyword_comptime: return "keyword(comptime)";
        case helium_token_type_keyword_static: return "keyword(static)";
        case helium_token_type_keyword_for: return "keyword(for)";
        case helium_token_type_keyword_while: return "keyword(while)";
        case helium_token_type_keyword_return: return "keyword(return)";
        case helium_token_type_keyword_continue: return "keyword(continue)";
        case helium_token_type_keyword_break: return "keyword(break)";
        case helium_token_type_keyword_func: return "keyword(func)";
        case helium_token_type_keyword_pub: return "keyword(pub)";
    }
    return "";
}

bool helium_can_lex(struct helium_lexer lexer) {
    while (utf8_isspace(iterator_utf8_peek(&lexer.iterator))) {
        iterator_utf8_eat(&lexer.iterator);
    }
    
    return iterator_utf8_can_eat(&lexer.iterator);
}


bool helium_can_eat(struct helium_lexer *lexer) {
    return iterator_utf8_can_eat(&lexer->iterator);
}

utf8_codepoint_t helium_lexer_peek(struct helium_lexer *lexer) {
    return iterator_utf8_peek(&lexer->iterator);
}

utf8_codepoint_t helium_lexer_eat(struct helium_lexer *lexer) {
    utf8_codepoint_t ch = iterator_utf8_eat(&lexer->iterator);
    lexer->loc.column++;
    lexer->loc.pos = lexer->iterator.pos;
    
    if (utf8_isnewln(ch)) {
        lexer->loc.line_num++;
        lexer->loc.line_begin = lexer->loc.pos;
        lexer->loc.column = 0;
    }
    
    return ch;
}

struct helium_token helium_lex(struct helium_lexer *lexer) {
    while (utf8_isspace(helium_lexer_peek(lexer))) {
        helium_lexer_eat(lexer);
    }
    
    struct helium_lexer_loc old_loc = lexer->loc;
    
    #define MAKE_LEN() lexer->loc.pos - old_loc.pos
    #define MAKE_LOC() \
        ((struct helium_loc){ \
            .file_path = old_loc.file_path, \
            .line_num = old_loc.line_num, \
            .line_begin = old_loc.line_begin, \
            .column = old_loc.column, \
            .pos = old_loc.pos, \
            .len = MAKE_LEN(), \
        })
    
    enum helium_token_type type = helium_token_type_unkown;
    utf8_codepoint_t ch = helium_lexer_eat(lexer);
    
    switch (ch) {
        case '\0': type = helium_token_type_end; break;
        case '#': {
            ch = helium_lexer_peek(lexer);
            if (ch != '!') {
                break;
            }
            
            while (ch != '\n' && ch != '\0') {
                helium_lexer_eat(lexer);
                ch = helium_lexer_peek(lexer);
            }
            
            type = helium_token_type_shebang;
        } break;
        case '/': {
            type = helium_token_type_slash;
            ch = helium_lexer_peek(lexer);
            if (ch == '/') {
                while (ch != '\n' && ch != '\0') {
                    helium_lexer_eat(lexer);
                    ch = helium_lexer_peek(lexer);
                }
                
                type = helium_token_type_comment;
            } else if (ch == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_slash_equals;
            }
            
        } break;
        case '\'': {
            type = helium_token_type_ch_unfinished;
            
            size_t len = 0;
            utf8_codepoint_t prev = ch;
            ch = helium_lexer_peek(lexer);
            while ((ch != '\0' && ch != '\n') && !(prev != '\\' && ch == '\'')) {
                prev = helium_lexer_eat(lexer);
                ch = helium_lexer_peek(lexer);
                if (prev != '\\') {
                    len++;
                }
            }
            
            if (ch == '\'') {
                ch = helium_lexer_eat(lexer);
                type = helium_token_type_ch;
                if (len != 1) {
                    type = helium_token_type_ch_too_long;
                }
            }
        } break;
        case '\"': {
            type = helium_token_type_str_unfinished;
            
            utf8_codepoint_t prev = ch;
            ch = helium_lexer_peek(lexer);
            while ((ch != '\0' && ch != '\n') && !(prev != '\\' && ch == '\"')) {
                prev = helium_lexer_eat(lexer);
                ch = helium_lexer_peek(lexer);
            }
            
            if (ch == '\"') {
                ch = helium_lexer_eat(lexer);
                type = helium_token_type_str;
            }
        } break;
        case '\\': {
            ch = helium_lexer_peek(lexer);
            if (ch != '\\') {
                break;
            }
            
            while (ch != '\n' && ch != '\0') {
                helium_lexer_eat(lexer);
                ch = helium_lexer_peek(lexer);
            }
            
            type = helium_token_type_str_multiline;
        } break;
        case '_':
        case 'a'...'z':
        case 'A'...'Z': {
            type = helium_token_type_ident;
            ch = helium_lexer_peek(lexer);
            while (isalnum(ch) || ch == '_') {
                helium_lexer_eat(lexer);
                ch = helium_lexer_peek(lexer);
            }
            
            struct str_slice slice = { .buf = lexer->iterator.slice.buf + old_loc.pos, .len = MAKE_LEN() };
            for (size_t i = 0; i < arr_sizeof(helium_keywords); i++) {
                if (!str_slice_eq(&slice, &helium_keywords[i].value)) continue;
                type = helium_keywords[i].type;
                
                break;
            }
        } break;
        case '0'...'9': {
            type = helium_token_type_num;
            switch (ch = helium_lexer_peek(lexer)) {
                case 'B':
                case 'b': {
                    helium_lexer_eat(lexer);
                    ch = helium_lexer_peek(lexer);
                    
                    while (ch == '0' || ch == '1' || ch == '_') {
                        helium_lexer_eat(lexer);
                        ch = helium_lexer_peek(lexer);
                    }
                } break;
                
                case 'O':
                case 'o': {
                    helium_lexer_eat(lexer);
                    ch = helium_lexer_peek(lexer);
                    
                    while ((ch >= '0' && ch <= '7') || ch == '_') {
                        helium_lexer_eat(lexer);
                        ch = helium_lexer_peek(lexer);
                    }
                } break;
                
                case 'X':
                case 'x': {
                    helium_lexer_eat(lexer);
                    ch = helium_lexer_peek(lexer);
                    
                    while (
                        (ch >= '0' && ch <= '9') ||
                        (ch >= 'a' && ch <= 'f') ||
                        (ch >= 'A' && ch <= 'F') ||
                        ch == '_'
                    ) {
                        helium_lexer_eat(lexer);
                        ch = helium_lexer_peek(lexer);
                    }
                } break;
                
                default: {
                    size_t count_dot = 0;
                    while (isdigit(ch) || ch == '_' || ch == '.') {
                        if (ch == '.') {
                            struct helium_lexer peeker = *lexer;
                            helium_lexer_eat(&peeker);
                            if (!isdigit(helium_lexer_peek(&peeker))) {
                                break;
                            }
                            
                            count_dot++;
                        }
                        
                        helium_lexer_eat(lexer);
                        ch = helium_lexer_peek(lexer);
                    }
                    
                    size_t postfix_start = lexer->loc.pos;
                    while (isalnum(ch) || ch == '_') {
                        helium_lexer_eat(lexer);
                        ch = helium_lexer_peek(lexer);
                    }
                    size_t postfix_end = lexer->loc.pos;
                    
                    
                    if (count_dot > 1 || (postfix_end - postfix_start) > 0) {
                        type = helium_token_type_num_malformed;
                    }
                }
            }
        } break;
        case '(': type = helium_token_type_paren_round_open; break;
        case ')': type = helium_token_type_paren_round_close; break;
        case '[': type = helium_token_type_paren_block_open; break;
        case ']': type = helium_token_type_paren_block_close; break;
        case '{': type = helium_token_type_paren_curly_open; break;
        case '}': type = helium_token_type_paren_curly_close; break;
        case '+': {
            type = helium_token_type_plus;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_plus_equals;
            }
        } break;
        case '-': {
            type = helium_token_type_minus;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_minus_equals;
            }
        } break;
        case '%': {
            type = helium_token_type_percent;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_percent_equals;
            }
        } break;
        case '*': {
            type = helium_token_type_star;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_star_equals;
            }
        } break;
        case '~': type = helium_token_type_bitwise_not; break;
        case '^': {
            type = helium_token_type_bitwise_xor;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_bitwise_xor_equals;
            }
        } break;
        case '&': {
            type = helium_token_type_bitwise_and;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_bitwise_and_equals;
            }
        } break;
        case '|': {
            type = helium_token_type_bitwise_or;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_bitwise_or_equals;
            }
        } break;
        case '<': {
            type = helium_token_type_less;
            ch = helium_lexer_peek(lexer);
            if (ch == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_less_equals;
            } else if (ch == '<') {
                helium_lexer_eat(lexer);
                type = helium_token_type_bitwise_shift_left;
                if (helium_lexer_peek(lexer) == '=') {
                    helium_lexer_eat(lexer);
                    type = helium_token_type_bitwise_shift_left_equals;
                }
            }
        } break;
        case '>': {
            type = helium_token_type_greater;
            ch = helium_lexer_peek(lexer);
            if (ch == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_greater_equals;
            } else if (ch == '>') {
                helium_lexer_eat(lexer);
                type = helium_token_type_bitwise_shift_right;
                if (helium_lexer_peek(lexer) == '=') {
                    helium_lexer_eat(lexer);
                    type = helium_token_type_bitwise_shift_right_equals;
                }
            }
        } break;
        case ':': type = helium_token_type_colon; break;
        case '=': {
            type = helium_token_type_equals;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_equals_equals;
            }
        } break;
        case '!': {
            type = helium_token_type_exclamation_mark;
            if (helium_lexer_peek(lexer) == '=') {
                helium_lexer_eat(lexer);
                type = helium_token_type_not_equals;
            }
        } break;
        case '?': type = helium_token_type_question_mark; break;
        case '.': {
            type = helium_token_type_dot;
            if (helium_lexer_peek(lexer) == '.') {
                helium_lexer_eat(lexer);
                type = helium_token_type_dot_dot;
                ch = helium_lexer_peek(lexer);
                if (ch == '=') {
                    helium_lexer_eat(lexer);
                    type = helium_token_type_dot_dot_equals;
                } else if (ch == '.') {
                    helium_lexer_eat(lexer);
                    type = helium_token_type_dot_dot_dot;
                }
            }
        } break;
        case ',': type = helium_token_type_comma; break;
    }
    
    return (struct helium_token){
        .type = type,
        .data = (struct str_slice){
            .buf = lexer->iterator.slice.buf + old_loc.pos,
            .len = MAKE_LEN()
        },
        .loc = MAKE_LOC(),
    };
    #undef MAKE_LOC
    #undef MAKE_LEN
}