#ifndef _INCLUDE_HELIUM_PARSER_C
#define _INCLUDE_HELIUM_PARSER_C

#include <helium/ast.c>
#include <helium/lexer.c>
#include <helium/loc.c>
#include <io.c>
#include <mem.c>
#include <da.c>

enum helium_syntax_err_type : int {
    helium_syntax_err_type_none,
    helium_syntax_err_type_no_nud,
    helium_syntax_err_type_malloc,
    helium_syntax_err_type_unmatched_paren_round_open,
    helium_syntax_err_type_unmatched_paren_round_close,
    helium_syntax_err_type_unmatched_paren_block_open,
    helium_syntax_err_type_unmatched_paren_block_close,
    helium_syntax_err_type_unmatched_paren_curly_open,
    helium_syntax_err_type_unmatched_paren_curly_close,
    helium_syntax_err_type_unclosed_tuple,
    helium_syntax_err_type_missing_semi,
    helium_syntax_err_type_expected_ident_after_dot,
    helium_syntax_err_type_unclosed_index,
    helium_syntax_err_type_indexless_index,
    helium_syntax_err_type_reached_end_expr,
    helium_syntax_err_type_reached_end_stmt,
    helium_syntax_err_type_func_expected_param_open,
    helium_syntax_err_type_func_expected_param_close,
    helium_syntax_err_type_func_expected_ident,
    helium_syntax_err_type_func_expected_type,
    helium_syntax_err_type_array_expected_close_paren,
};

enum helium_pratt_power : size_t {
    helium_pratt_power_min,
    helium_pratt_power_concat,
    helium_pratt_power_bitwise_or,
    helium_pratt_power_bitwise_xor,
    helium_pratt_power_bitwise_and,
    helium_pratt_power_equality,
    helium_pratt_power_unequality,
    helium_pratt_power_bitwise_shift,
    helium_pratt_power_additive,
    helium_pratt_power_multiplicative,
    helium_pratt_power_prefix,
    helium_pratt_power_call,
    helium_pratt_power_member,
    helium_pratt_power_index,
    helium_pratt_power_max,
};

struct helium_syntax_err {
    enum helium_syntax_err_type type;
    size_t token;
    struct helium_loc loc;
};

struct helium_parser;
typedef struct helium_ast_expr *(*helium_pratt_handler_nud_t)(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
typedef struct helium_ast_expr *(*helium_pratt_handler_led_t)(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, struct helium_ast_expr *left, enum helium_pratt_power power);
typedef struct helium_ast_stmt *(*helium_pratt_handler_stmt_t)(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);

struct helium_pratt_lookups {
    helium_pratt_handler_nud_t nuds[HELIUM_TOKEN_TYPE_MAX + 1];
    helium_pratt_handler_led_t leds[HELIUM_TOKEN_TYPE_MAX + 1];
    helium_pratt_handler_stmt_t stmts[HELIUM_TOKEN_TYPE_MAX + 1];
    enum helium_pratt_power powers[HELIUM_TOKEN_TYPE_MAX + 1];
};

struct helium_parser {
    struct helium_tokens tokens;
    struct helium_pratt_lookups lookups;
    struct str_slice source;
    size_t pos;
};
struct helium_ast_expr *helium_make_expr_f(struct allocator *allocator, struct helium_ast_expr expr);
struct helium_ast_expr *helium_pratt_expr(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, enum helium_pratt_power parse_power);
struct helium_ast_expr *helium_pratt_expr_ch(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_str(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_ident(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_num(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_binary(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, struct helium_ast_expr *left, enum helium_pratt_power power);
struct helium_ast_expr *helium_pratt_expr_bool(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_nothing(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_tuple(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_prefix(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_qualifiers(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_func(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);
struct helium_ast_expr *helium_pratt_expr_call(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, struct helium_ast_expr *left, enum helium_pratt_power power);
struct helium_ast_expr *helium_pratt_expr_member(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, struct helium_ast_expr *left, enum helium_pratt_power power);
struct helium_ast_expr *helium_pratt_expr_index(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, struct helium_ast_expr *left, enum helium_pratt_power power);
void helium_parser_lookups_set_default(struct helium_parser *parser);

struct helium_ast_stmt *helium_pratt_stmt_block(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err, struct helium_qualifiers qualifiers);
struct helium_ast_stmt *helium_pratt_stmt(struct allocator *allocator, struct helium_parser *parser, struct helium_syntax_err *err);

bool helium_parser_check_braces(struct allocator *allocator, struct helium_parser parser, struct helium_syntax_err *err);
void helium_syntax_err_pretty_print(FILE *out, struct helium_parser parser, struct helium_syntax_err err, bool colors);

#define helium_make_expr(allocator, ...)helium_make_expr_f(allocator, (struct helium_ast_expr)__VA_ARGS__)

#ifndef HELIUM_EXTERN

struct helium_ast_expr *helium_make_expr_f(struct allocator *allocator, struct helium_ast_expr expr) {
    struct helium_ast_expr *mem = allocator->alloc(allocator, sizeof(*mem));
    if (mem == NULL) {
        return NULL;
    }
    
    *mem = expr;
    
    return mem;
}

struct helium_ast_expr *helium_pratt_expr(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err,
    enum helium_pratt_power parse_power
) {
    struct helium_token token = parser->tokens.items[parser->pos];
    if (token.type == helium_token_type_end) {
        err->type = helium_syntax_err_type_reached_end_expr;
        err->token = parser->pos;
        return NULL;
    }
    
    helium_pratt_handler_nud_t nud = parser->lookups.nuds[token.type];
    if (nud == NULL) {
        err->type = helium_syntax_err_type_no_nud;
        err->token = parser->pos++;
        return NULL;
    }
    
    struct helium_ast_expr *left = nud(allocator, parser, err);
    if (left == NULL) {
        return NULL;
    }
    
    {
        token = parser->tokens.items[parser->pos];
        enum helium_pratt_power token_power = parser->lookups.powers[token.type];
        
        while (token_power > parse_power) {
            helium_pratt_handler_led_t led = parser->lookups.leds[token.type];
            if (led == NULL) {
                break;
            }
            
            left = led(allocator, parser, err, left, token_power);
            if (left == NULL) return NULL;
            
            token = parser->tokens.items[parser->pos];
            token_power = parser->lookups.powers[token.type];
        }
    }
    
    return left;
}

struct helium_ast_expr *helium_pratt_expr_ch(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct iterator_utf8 iter = {
        .slice = (struct str_slice){
            .buf = token.slice.buf + 1,
            .len = token.slice.len - 1,
        },
    };
    
    utf8_codepoint_t codepoint = iterator_utf8_eat(&iter);
    
    if (codepoint == '\\') {
        codepoint = iterator_utf8_eat(&iter);
        switch(codepoint) {
            case 'a': codepoint = '\a'; break;
            case 'b': codepoint = '\b'; break;
            case 'e': codepoint = '\e'; break;
            case 'f': codepoint = '\f'; break;
            case 'n': codepoint = '\n'; break;
            case 'r': codepoint = '\r'; break;
            case 't': codepoint = '\t'; break;
            case 'v': codepoint = '\v'; break;
        }
    }
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_ch,
        .ch = (struct helium_ast_expr_ch){
            .loc = token.loc,
            .value = codepoint,
        },
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    }
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_str(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct str str = {0};
    for (size_t i = 1; i < token.slice.len - 1; i++) {
        if (token.slice.buf[i] != '\\') {
            if (!str_append(allocator, &str, token.slice.buf + i, 1)) {
                err->type = helium_syntax_err_type_malloc;
                return NULL;
            };
            continue;
        }
        
        char append = token.slice.buf[++i];
        switch(append) {
            case 'a': append = '\a'; break;
            case 'b': append = '\b'; break;
            case 'e': append = '\e'; break;
            case 'f': append = '\f'; break;
            case 'n': append = '\n'; break;
            case 'r': append = '\r'; break;
            case 't': append = '\t'; break;
            case 'v': append = '\v'; break;
        }
        
        if (!str_append(allocator, &str, &append, 1)) {
            err->type = helium_syntax_err_type_malloc;
            return NULL;
        };
    }
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_str,
        .str = (struct helium_ast_expr_str){
            .loc = token.loc,
            .value = str,
        },
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        str_free(allocator, &str);
        return NULL;
    }
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_ident(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_ident,
        .ident = (struct helium_ast_expr_ident){
            .loc = token.loc,
            .value = token.slice,
        },
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    }
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_num(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    enum helium_ast_num_type type = helium_ast_num_type_integer;
    union helium_ast_num_value value = {0};
    size_t floating_point_div = 10;
    size_t float_size = 0;
    
    for (size_t i = 0; i < token.slice.len; i++) {
        char ch = token.slice.buf[i];
        if (ch == '.') {
            value.floating = (f64)value.integer;
            type = helium_ast_num_type_floating;
            continue;
        }
        
        if (type == helium_ast_num_type_integer) {
            value.integer *= 10;
            value.integer += ch - '0';
        } else {
            value.floating += (double)(ch - '0') / floating_point_div;
            floating_point_div *= 10;
            float_size++;
        }
    }
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_num,
        .num = (struct helium_ast_expr_num){
            .loc = token.loc,
            .value = value,
            .num_type = type,
            .float_size = float_size,
        }
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    };
    
    return expr;
};

struct helium_ast_expr *helium_pratt_expr_binary(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err,
    struct helium_ast_expr *left,
    enum helium_pratt_power power
) {
    struct helium_token operator = parser->tokens.items[parser->pos++];
    struct helium_ast_expr *right = helium_pratt_expr(allocator, parser, err, power);
    if (right == NULL) {
        helium_ast_expr_free(allocator, left);
        return NULL;
    }
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_binary,
        .binary = (struct helium_ast_expr_binary){
            .left = left,
            .right = right,
            .operator = operator,
        }
    });
    if (expr == NULL) {
        helium_ast_expr_free(allocator, left);
        helium_ast_expr_free(allocator, right);
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    };
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_bool(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_bool,
        .bool_ = (struct helium_ast_expr_bool){
            .loc = token.loc,
            .value = token.type == helium_token_type_keyword_true,
        }
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    };
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_nothing(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_nothing,
        .nothing = (struct helium_ast_expr_nothing){
            .loc = token.loc,
        }
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    };
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_tuple(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    size_t begin_index = parser->pos;
    struct helium_token paren_open = parser->tokens.items[begin_index];
    
    struct helium_token token = parser->tokens.items[begin_index + 1];
    if (token.type == helium_token_type_paren_round_close) {
        parser->pos += 2;
        
        struct helium_ast_expr *expr = helium_make_expr(allocator, {
            .ast_type = helium_ast_type_expr_tuple,
            .tuple = (struct helium_ast_expr_tuple){
                .begin = paren_open.loc,
                .end = token.loc,
            },
        });
        if (expr == NULL) {
            err->type = helium_syntax_err_type_malloc;
            return NULL;
        }
        
        return expr;
    }
    
    struct helium_ast_expr_tuple_elem *first = NULL;
    struct helium_ast_expr_tuple_elem *last = NULL;
    {
        bool is_first = true;
        do {
            parser->pos++;
            
            struct helium_ast_expr *expr = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
            if (expr == NULL) return NULL;
            
            token = parser->tokens.items[parser->pos];
            if (is_first && token.type == helium_token_type_paren_round_close) {
                parser->pos++;
                return expr;
            }
            
            if (first == NULL) {
                first = allocator->alloc(allocator, sizeof(*first));
                if (first == NULL) {
                    err->type == helium_syntax_err_type_malloc;
                    helium_ast_expr_free(allocator, expr);
                    goto fail;
                }
                
                first->next = first;
                first->prev = first;
                first->expr = expr;
                last = first;
            } else {
                struct helium_ast_expr_tuple_elem *node = allocator->alloc(allocator, sizeof(*node));
                if (node == NULL) {
                    err->type == helium_syntax_err_type_malloc;
                    helium_ast_expr_free(allocator, expr);
                    goto fail;
                }
                
                node->next = first;
                node->prev = last;
                node->expr = expr;
                last->next = node;
                last = node;
            }
            
            is_first = false;
        } while (token.type == helium_token_type_comma);
    }
    
    if (token.type != helium_token_type_paren_round_close) {
        err->type = helium_syntax_err_type_unclosed_tuple;
        err->token = begin_index;
        goto fail;
    }
    parser->pos++;
    
    struct helium_ast_expr *tuple = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_tuple,
        .tuple = (struct helium_ast_expr_tuple){
            .begin = paren_open.loc,
            .end = token.loc,
            .first = first,
            .last = last,
        },
    });
    if (tuple == NULL) {
        err->type == helium_syntax_err_type_malloc;
        goto fail;
    }
    
    return tuple;
    fail: {
        for (struct helium_ast_expr_tuple_elem *node = last; node != NULL;) {
            struct helium_ast_expr_tuple_elem *prev = node->prev;
            
            helium_ast_expr_free(allocator, node->expr);
            allocator->free(allocator, node, sizeof(*node));
            
            if (node == first) break;
            node = prev;
        }
        
        return NULL;
    }
}

struct helium_ast_expr *helium_pratt_expr_prefix(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct helium_ast_expr *value = helium_pratt_expr(allocator, parser, err, helium_pratt_power_prefix);
    if (value == NULL) return NULL;
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_prefix,
        .prefix = (struct helium_ast_expr_prefix){
            .operator = token,
            .value = value,
        }
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    };
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_qualifiers(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_token token = parser->tokens.items[parser->pos++];
    struct helium_ast_expr *value = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
    if (value == NULL) return NULL;
    
    struct helium_ast_expr *expr = value;
    if (expr->ast_type != helium_ast_type_expr_qualifiers) {
        expr = helium_make_expr(allocator, {
            .ast_type = helium_ast_type_expr_qualifiers,
            .qualifiers = (struct helium_ast_expr_qualifiers){
                .value = value,
            }
        });
        if (expr == NULL) {
            helium_ast_expr_free(allocator, value);
            err->type = helium_syntax_err_type_malloc;
            return NULL;
        };
    }
    struct helium_qualifiers *qualifiers = &expr->qualifiers.qualifiers;
    
    switch(token.type) {
        case helium_token_type_keyword_var: qualifiers->is_const = false; break;
        case helium_token_type_keyword_const:  qualifiers->is_const = true; break;
        case helium_token_type_keyword_runtime:  qualifiers->is_comptime = false; break;
        case helium_token_type_keyword_comptime:  qualifiers->is_comptime = true; break;
        case helium_token_type_keyword_static:  qualifiers->is_static = true; break;
    }
    
    return expr;
}

struct helium_ast_func_params helium_parse_func_params(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    *err = (struct helium_syntax_err){0};
    
    struct helium_token paren_open = parser->tokens.items[parser->pos];
    if (paren_open.type != helium_token_type_paren_round_open) {
        err->type = helium_syntax_err_type_func_expected_param_open;
        return (struct helium_ast_func_params){0};
    }
    
    struct helium_token token = parser->tokens.items[parser->pos + 1];
    if (token.type == helium_token_type_paren_round_close) {
        parser->pos += 2;
        return (struct helium_ast_func_params){
            .begin = paren_open.loc,
            .end = token.loc,
            .first = NULL,
            .last = NULL,
        };
    }
    
    struct helium_ast_func_param *first = NULL;
    struct helium_ast_func_param *last = NULL;
    
    do {
        token = parser->tokens.items[++parser->pos];
        if (token.type != helium_token_type_ident) {
            err->type = helium_syntax_err_type_func_expected_ident;
            err->token = parser->pos - 1;
            goto fail;
        }
        
        struct helium_ast_expr_ident name = {
            .loc = token.loc,
            .value = token.slice,
        };
        
        token = parser->tokens.items[++parser->pos];
        if (token.type != helium_token_type_colon) {
            err->type = helium_syntax_err_type_func_expected_type;
            err->token = parser->pos - 2;
            goto fail;
        }
        parser->pos++;
        
        struct helium_ast_expr *type = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
        if (type == NULL) goto fail;
        
        struct helium_ast_expr *default_value = NULL;
        token = parser->tokens.items[parser->pos];
        if (token.type == helium_token_type_equals) {
            parser->pos++;
            default_value = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
            if (default_value == NULL) goto fail;
        }
        
        struct helium_ast_func_param *param = allocator->alloc(allocator, sizeof(*param));
        if (param == NULL) {
            err->type = helium_syntax_err_type_malloc;
            goto fail;
        }
        
        param->name = name;
        param->type = type;
        param->default_value = default_value;
        param->next = first ? first : param;
        param->prev = last ? last : param;
        if (first == NULL) {
            first = param;
        }
        if (last) {
            last->next = param;
        }
        last = param;
        
        token = parser->tokens.items[parser->pos];
    } while(token.type == helium_token_type_comma);
    
    if (token.type != helium_token_type_paren_round_close) {
        err->type = helium_syntax_err_type_func_expected_param_close;
        err->token = parser->pos - 1;
        goto fail;
    }
    parser->pos++;
    
    return (struct helium_ast_func_params){
        .begin = paren_open.loc,
        .end = token.loc,
        .first = first,
        .last = last,
    };
fail:
    for (struct helium_ast_func_param *node = last; node != NULL;) {
        struct helium_ast_func_param *prev = node->prev;
        
        node->type = helium_ast_expr_free(allocator, node->type);
        if (node->default_value) {
            node->default_value = helium_ast_expr_free(allocator, node->default_value);
        }
        node = allocator->free(allocator, node, sizeof(*node));
        
        if (prev == last) break;
        node = prev;
    }
    return (struct helium_ast_func_params){0};
}

struct helium_ast_expr *helium_pratt_expr_func(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    *err = (struct helium_syntax_err){0};
    struct helium_token keyword_token = parser->tokens.items[parser->pos++];
    struct helium_ast_func_params params = helium_parse_func_params(allocator, parser, err);
    if (err->type != 0) {
        return NULL;
    }
    
    struct helium_ast_stmt *body = NULL;
    struct helium_ast_expr *return_type = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
    if (return_type == NULL) {
        goto fail;
    }
    
    struct helium_token token = parser->tokens.items[parser->pos];
    if (token.type == helium_token_type_paren_curly_open) {
        body = helium_pratt_stmt_block(allocator, parser, err, (struct helium_qualifiers){0});
        if (body == NULL) {
            goto fail;
        }
    }
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_func,
        .func = (struct helium_ast_expr_func){
            .keyword_loc = keyword_token.loc,
            .body = body,
            .return_type = return_type,
            .params = params,
        },
    });
    if (expr == NULL) {
        goto fail;
    }
    
    return expr;
fail:
    for (struct helium_ast_func_param *node = params.last; node != NULL;) {
        struct helium_ast_func_param *prev = node->prev;
        
        helium_ast_expr_free(allocator, node->type);
        if (node->default_value) {
            helium_ast_expr_free(allocator, node->default_value);
        }
        allocator->free(allocator, node, sizeof(*node));
        
        if (node == params.first) break;
        node = prev;
    }
    
    if (return_type) {
        helium_ast_expr_free(allocator, return_type);
    }
    
    if (body) {
        helium_ast_stmt_free(allocator, body);
    }
    
    return NULL;
}

struct helium_ast_expr *helium_pratt_expr_array(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_ast_expr *size = NULL;
    size_t open_paren_i = parser->pos;
    struct helium_token token = parser->tokens.items[++parser->pos];
    if (token.type != helium_token_type_paren_block_close) {
        size = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
        if (size == NULL) {
            return NULL;
        }
    }
    
    token = parser->tokens.items[parser->pos];
    if (token.type != helium_token_type_paren_block_close) {
        err->type = helium_syntax_err_type_array_expected_close_paren;
        err->token = open_paren_i;
        if (size) {
            helium_ast_expr_free(allocator, size);
        }
        return NULL;
    }
    parser->pos++;
    
    struct helium_ast_expr *contents = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
    if (contents == NULL) {
        if (size) {
            helium_ast_expr_free(allocator, size);
        }
        return NULL;
    }
    
    struct helium_ast_expr *array = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_array,
        .array = {
            .size = size,
            .contents = contents,
        },
    });
    if (array == NULL) {
        if (size) {
            helium_ast_expr_free(allocator, size);
        }
        return NULL;
    }
    
    return array;
}

struct helium_ast_expr *helium_pratt_expr_call(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err,
    struct helium_ast_expr *left,
    enum helium_pratt_power power
) {
    return helium_ast_expr_free(allocator, left);
}

struct helium_ast_expr *helium_pratt_expr_member(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err,
    struct helium_ast_expr *left,
    enum helium_pratt_power power
) {
    parser->pos++;
    struct helium_token token = parser->tokens.items[parser->pos++];
    if (token.type != helium_token_type_ident) {
        err->type = helium_syntax_err_type_expected_ident_after_dot;
        err->token = parser->pos - 1;
        helium_ast_expr_free(allocator, left);
        return NULL;
    }
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_member,
        .member = (struct helium_ast_expr_member){
            .indexe = left,
            .member = (struct helium_ast_expr_ident) {
                .loc = token.loc,
                .value = token.slice,
            },
        }
    });
    if (expr == NULL) {
        err->type = helium_syntax_err_type_malloc;
        helium_ast_expr_free(allocator, left);
        return NULL;
    }
    
    return expr;
}

struct helium_ast_expr *helium_pratt_expr_index(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err,
    struct helium_ast_expr *left,
    enum helium_pratt_power power
) {
    struct helium_token open = parser->tokens.items[parser->pos++];
    struct helium_token token = parser->tokens.items[parser->pos];
    if (token.type == helium_token_type_paren_block_close) {
        err->type = helium_syntax_err_type_indexless_index;
        err->token = parser->pos - 1;
        helium_ast_expr_free(allocator, left);
        return NULL;
    }
    
    struct helium_ast_expr *index = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
    if (index == NULL) {
        helium_ast_expr_free(allocator, left);
        return NULL;
    }
    
    token = parser->tokens.items[parser->pos];
    if (token.type != helium_token_type_paren_block_close) {
        err->type = helium_syntax_err_type_unclosed_index;
        err->token = parser->pos - 1;
        helium_ast_expr_free(allocator, left);
        helium_ast_expr_free(allocator, index);
        return NULL;
    }
    parser->pos++;
    
    struct helium_ast_expr *expr = helium_make_expr(allocator, {
        .ast_type = helium_ast_type_expr_index,
        .index = (struct helium_ast_expr_index){
            .indexe = left,
            .index = index,
        },
    });
    if (expr == NULL) {
        helium_ast_expr_free(allocator, left);
        helium_ast_expr_free(allocator, index);
        return NULL;
    }
    
    return expr;
}

void helium_parser_lookups_set_default(struct helium_parser *parser) {
    #define NUD(Tok, Fn) parser->lookups.nuds[Tok] = Fn
    #define LED(Tok, Fn, Pow) parser->lookups.leds[Tok] = Fn; parser->lookups.powers[Tok] = Pow
    
    NUD(helium_token_type_ch, helium_pratt_expr_ch);
    NUD(helium_token_type_str, helium_pratt_expr_str);
    NUD(helium_token_type_ident, helium_pratt_expr_ident);
    NUD(helium_token_type_num, helium_pratt_expr_num);
    NUD(helium_token_type_keyword_true, helium_pratt_expr_bool);
    NUD(helium_token_type_keyword_false, helium_pratt_expr_bool);
    NUD(helium_token_type_keyword_nothing, helium_pratt_expr_nothing);
    NUD(helium_token_type_paren_round_open, helium_pratt_expr_tuple);
    NUD(helium_token_type_plus, helium_pratt_expr_prefix);
    NUD(helium_token_type_minus, helium_pratt_expr_prefix);
    NUD(helium_token_type_star, helium_pratt_expr_prefix);
    NUD(helium_token_type_bitwise_not, helium_pratt_expr_prefix);
    NUD(helium_token_type_bitwise_and, helium_pratt_expr_prefix);
    NUD(helium_token_type_question_mark, helium_pratt_expr_prefix);
    NUD(helium_token_type_dot_dot_dot, helium_pratt_expr_prefix);
    NUD(helium_token_type_keyword_var, helium_pratt_expr_qualifiers);
    NUD(helium_token_type_keyword_const, helium_pratt_expr_qualifiers);
    NUD(helium_token_type_keyword_runtime, helium_pratt_expr_qualifiers);
    NUD(helium_token_type_keyword_comptime, helium_pratt_expr_qualifiers);
    NUD(helium_token_type_keyword_static, helium_pratt_expr_qualifiers);
    NUD(helium_token_type_keyword_func, helium_pratt_expr_func);
    NUD(helium_token_type_paren_block_open, helium_pratt_expr_array);
    LED(helium_token_type_bitwise_or, helium_pratt_expr_binary, helium_pratt_power_bitwise_or);
    LED(helium_token_type_bitwise_xor, helium_pratt_expr_binary, helium_pratt_power_bitwise_xor);
    LED(helium_token_type_bitwise_and, helium_pratt_expr_binary, helium_pratt_power_bitwise_and);
    LED(helium_token_type_equals_equals, helium_pratt_expr_binary, helium_pratt_power_equality);
    LED(helium_token_type_not_equals, helium_pratt_expr_binary, helium_pratt_power_equality);
    LED(helium_token_type_greater, helium_pratt_expr_binary, helium_pratt_power_unequality);
    LED(helium_token_type_greater_equals, helium_pratt_expr_binary, helium_pratt_power_unequality);
    LED(helium_token_type_less, helium_pratt_expr_binary, helium_pratt_power_unequality);
    LED(helium_token_type_less_equals, helium_pratt_expr_binary, helium_pratt_power_unequality);
    LED(helium_token_type_bitwise_shift_left, helium_pratt_expr_binary, helium_pratt_power_bitwise_shift);
    LED(helium_token_type_bitwise_shift_right, helium_pratt_expr_binary, helium_pratt_power_bitwise_shift);
    LED(helium_token_type_plus, helium_pratt_expr_binary, helium_pratt_power_additive);
    LED(helium_token_type_minus, helium_pratt_expr_binary, helium_pratt_power_additive);
    LED(helium_token_type_percent, helium_pratt_expr_binary, helium_pratt_power_multiplicative);
    LED(helium_token_type_slash, helium_pratt_expr_binary, helium_pratt_power_multiplicative);
    LED(helium_token_type_star, helium_pratt_expr_binary, helium_pratt_power_multiplicative);
    LED(helium_token_type_dot_dot, helium_pratt_expr_binary, helium_pratt_power_concat);
    LED(helium_token_type_paren_round_open, helium_pratt_expr_call, helium_pratt_power_call);
    LED(helium_token_type_dot, helium_pratt_expr_member, helium_pratt_power_member);
    LED(helium_token_type_paren_block_open, helium_pratt_expr_index, helium_pratt_power_member);
    
    #undef NUD
    #undef LED
}

struct helium_ast_stmt *helium_pratt_stmt_block(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err,
    struct helium_qualifiers qualifiers
) {
    struct helium_token begin = parser->tokens.items[parser->pos++];
    struct helium_token token = parser->tokens.items[parser->pos];
    
    struct helium_ast_stmt *stmt = allocator->alloc(allocator, sizeof(*stmt));
    if (stmt == NULL) {
        err->type = helium_syntax_err_type_malloc;
        return NULL;
    };
    
    struct helium_ast_stmt *first = NULL;
    struct helium_ast_stmt *last = NULL;
    while (token.type != helium_token_type_paren_curly_close) {
        struct helium_ast_stmt *stmt = helium_pratt_stmt(allocator, parser, err);
        if (stmt == NULL) goto fail;
        
        if (first == NULL) {
            first = stmt;
            last = stmt;
            stmt->next = stmt;
            stmt->prev = stmt;
        } else {
            stmt->next = first;
            stmt->prev = last;
            last->next = stmt;
            last = stmt;
        }
        token = parser->tokens.items[parser->pos];
    }
    parser->pos++;
    
    *stmt = (struct helium_ast_stmt){
        .ast_type = helium_ast_type_stmt_block,
        .block = (struct helium_ast_stmt_block){
            .begin = begin.loc,
            .end = token.loc,
            .first = first,
            .last = last,
            .qualifiers = qualifiers,
        },
    };
    
    return stmt;
fail:
    for (struct helium_ast_stmt *node = last; node != NULL;) {
        helium_ast_stmt_free(allocator, node);
        
        if (node == first) break;
        node = node->prev;
    }
    
    stmt = allocator->free(allocator, stmt, sizeof(*stmt));
    return NULL;
}

struct helium_ast_stmt *helium_pratt_stmt(
    struct allocator *allocator,
    struct helium_parser *parser,
    struct helium_syntax_err *err
) {
    struct helium_parser before_qualifiers = *parser;
    struct helium_token token = parser->tokens.items[parser->pos];
    struct helium_qualifiers qualifiers = {0};
    bool has_qualifiers = false;
    for (;;) {
        switch(token.type) {
            case helium_token_type_keyword_var:
                has_qualifiers = true;
                qualifiers.is_const = false;
                break;
            case helium_token_type_keyword_const:
                has_qualifiers = true;
                qualifiers.is_const = true;
                break;
            case helium_token_type_keyword_runtime:
                has_qualifiers = true;
                qualifiers.is_comptime = false;
                break;
            case helium_token_type_keyword_comptime:
                has_qualifiers = true;
                qualifiers.is_comptime = true;
                break;
            case helium_token_type_keyword_static:
                has_qualifiers = true;
                qualifiers.is_static = true;
                break;
            default: goto stop;
        }
        token = parser->tokens.items[++parser->pos];
        continue;
    stop:
        break;
    }
    
    struct helium_ast_stmt *stmt;
    switch (token.type) {
        case helium_token_type_paren_curly_open:
            stmt = helium_pratt_stmt_block(allocator, parser, err, qualifiers);
            break;
        default: {
            *parser = before_qualifiers;
            
            struct helium_ast_expr *expr = helium_pratt_expr(allocator, parser, err, helium_pratt_power_min);
            if (expr == NULL) return NULL;
            
            stmt = allocator->alloc(allocator, sizeof(*stmt));
            if (stmt == NULL) {
                helium_ast_expr_free(allocator, expr);
                err->type = helium_syntax_err_type_malloc;
                return NULL;
            };
            
            *stmt = (struct helium_ast_stmt){
                .ast_type = helium_ast_type_stmt_expr,
                .expr = expr,
            };
        }
    }
    
    token = parser->tokens.items[parser->pos];
    
    size_t semi_count = 0;
    while (token.type == helium_token_type_semi_colon) {
        token = parser->tokens.items[++parser->pos];
        semi_count++;
    }
    
    if (
        semi_count == 0 &&
        stmt->ast_type != helium_ast_type_stmt_block &&
        stmt->ast_type != helium_ast_type_stmt_if &&
        stmt->ast_type != helium_ast_type_stmt_for
    ) {
        err->type = helium_syntax_err_type_missing_semi;
        err->token = parser->pos - 1;
        return helium_ast_stmt_free(allocator, stmt);
    }
    
    return stmt;
}

bool helium_parser_check_braces(
    struct allocator *allocator,
    struct helium_parser parser,
    struct helium_syntax_err *err
) {
    struct stack_paren {
        size_t *items;
        size_t len;
        size_t cap;
    };
    struct stack_paren stack = {0};
    
    #define POPI() stack.items[--stack.len]
    #define POP() parser.tokens.items[POPI()]
    for (size_t i = 0; i < parser.tokens.len - 1; i++) {
        struct helium_token token = parser.tokens.items[i];
        
        switch(token.type) {
            case helium_token_type_paren_curly_open:
            case helium_token_type_paren_block_open:
            case helium_token_type_paren_round_open: {
                if (!da_push(allocator, &stack, i)) {
                    err->type = helium_syntax_err_type_malloc;
                    return false;
                }
            } break;
            case helium_token_type_paren_curly_close: {
                if (stack.len == 0) {
                    err->type = helium_syntax_err_type_unmatched_paren_curly_close;
                    err->token = i;
                    return false;
                }
                
                struct helium_token pop = POP();
                if (pop.type != helium_token_type_paren_curly_open) {
                    err->type = helium_syntax_err_type_unmatched_paren_curly_close;
                    err->token = i;
                    return false;
                }
            } break;
            case helium_token_type_paren_block_close: {
                if (stack.len == 0) {
                    err->type = helium_syntax_err_type_unmatched_paren_block_close;
                    err->token = i;
                    return false;
                }
                
                struct helium_token pop = POP();
                if (pop.type != helium_token_type_paren_block_open) {
                    err->type = helium_syntax_err_type_unmatched_paren_block_close;
                    err->token = i;
                    return false;
                }
            } break;
            case helium_token_type_paren_round_close: {
                if (stack.len == 0) {
                    err->type = helium_syntax_err_type_unmatched_paren_round_close;
                    err->token = i;
                    return false;
                }
                
                struct helium_token pop = POP();
                if (pop.type != helium_token_type_paren_round_open) {
                    err->type = helium_syntax_err_type_unmatched_paren_round_close;
                    err->token = i;
                    return false;
                }
            } break;
        }
    }
    
    if (stack.len != 0) {
        size_t i = stack.items[0];
        struct helium_token pop = parser.tokens.items[i];
        switch(pop.type) {
            case helium_token_type_paren_round_open: {
                err->type = helium_syntax_err_type_unmatched_paren_round_open;
                err->token = i;
            } break;
            case helium_token_type_paren_block_open: {
                err->type = helium_syntax_err_type_unmatched_paren_block_open;
                err->token = i;
            } break;
            case helium_token_type_paren_curly_open: {
                err->type = helium_syntax_err_type_unmatched_paren_curly_open;
                err->token = i;
            } break;
        }
        return false;
    }
    
    da_free(allocator, &stack);
    
    return true;
    #undef POPI
    #undef POP
}

void helium_syntax_err_pretty_print(
    FILE *out,
    struct helium_parser parser,
    struct helium_syntax_err err,
    bool colors
) {
    #define INTEGER_LEN(INT)({\
        size_t len = 0;\
        for (size_t iter = (INT);;) {\
            len++;\
            iter /= 10;\
            if (iter == 0) break;\
        }\
        len;\
    })
    
    const char *ANSI_RESET = (colors) ? "\e[0m"  : "";
    const char *ANSI_BOLD  = (colors) ? "\e[1m"  : "";
    const char *ANSI_FAINT = (colors) ? "\e[2m"  : "";
    const char *ANSI_RED   = (colors) ? "\e[31m" : "";
    
    switch(err.type) {
        case helium_syntax_err_type_none: break;
        case helium_syntax_err_type_no_nud: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s expected expression, got '%.*s'%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                token.slice.len,
                token.slice.buf,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ here\e[0m")
                : STR_SLICE_LIT("^ here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_malloc: break;
        case helium_syntax_err_type_unmatched_paren_round_open:
        case helium_syntax_err_type_unmatched_paren_round_close:
        case helium_syntax_err_type_unmatched_paren_block_open:
        case helium_syntax_err_type_unmatched_paren_block_close:
        case helium_syntax_err_type_unmatched_paren_curly_open:
        case helium_syntax_err_type_unmatched_paren_curly_close: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s unmatched parenthesis%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ here\e[0m")
                : STR_SLICE_LIT("^ here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_unclosed_tuple: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s unclosed tuple, did you forget a comma?%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ tuple begins here\e[0m")
                : STR_SLICE_LIT("^ tuple begins here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_missing_semi: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s All statements must be terminated by a semi colon%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected ';' here\e[0m")
                : STR_SLICE_LIT("^ expected ';' here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                1,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_expected_ident_after_dot: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Expected an identifier after '.'%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected identifier here\e[0m")
                : STR_SLICE_LIT("^ expected identifier here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_unclosed_index: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Expected ']' to close off indexing%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected ']' here\e[0m")
                : STR_SLICE_LIT("^ expected ']' here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_indexless_index: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Indexing requires an expression%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected expression here\e[0m")
                : STR_SLICE_LIT("^ expected expression here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_reached_end_stmt: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Tried to parse a statement but reached end of meaningful tokens%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected statement here\e[0m")
                : STR_SLICE_LIT("^ expected statement here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_reached_end_expr: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Tried to parse an expression but reached end of meaningful tokens%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected expression here\e[0m")
                : STR_SLICE_LIT("^ expected expression here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_func_expected_param_open: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Expected function parameters after 'func'%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected parameters here\e[0m")
                : STR_SLICE_LIT("^ expected parameters here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                2,
                padding,
                colors
            );
            
            fprintf(out, "%*shelp: some examples of function parameters:\n", padding, "");
            
            fprintf(out, " %*s| \n", padding, "");
            fprintf(out, " %*s| ()\n", padding, "");
            fprintf(out, " %*s| (x: i32)\n", padding, "");
            fprintf(out, " %*s| (depth: u64 = 0)\n", padding, "");
            fprintf(out, " %*s| (a: i32, b: i32)\n", padding, "");
            fprintf(out, " %*s| \n", padding, "");
        } break;
        case helium_syntax_err_type_func_expected_param_close: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Expected ')' to close off function parameters, did you forget a comma?%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected ')' here\e[0m")
                : STR_SLICE_LIT("^ expected ')' here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_func_expected_ident: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Function parameters must be identifiers%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected identifier here\e[0m")
                : STR_SLICE_LIT("^ expected identifier here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_func_expected_type: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Tried to parse an expression but reached end of meaningful tokens%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ expected statement here\e[0m")
                : STR_SLICE_LIT("^ expected statement here");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
        case helium_syntax_err_type_array_expected_close_paren: {
            struct helium_token token = parser.tokens.items[err.token];
            size_t padding = INTEGER_LEN(token.loc.line_num);
            
            fprintf(out, "%serror:%s%s Expected ']' to close off '['%s\n",
                ANSI_RED,
                ANSI_RESET,
                ANSI_BOLD,
                ANSI_RESET
            );
            
            struct str_slice message =
                (colors)
                ? STR_SLICE_LIT("\e[31m^ this parenthesis is not closed\e[0m")
                : STR_SLICE_LIT("^ this parenthesis is not closed");
            helium_loc_pretty_print(
                out,
                parser.source,
                token.loc,
                &message,
                0,
                padding,
                colors
            );
        } break;
    }
}

#endif // HELIUM_EXTERN
#endif // _INCLUDE_HELIUM_PARSER_C