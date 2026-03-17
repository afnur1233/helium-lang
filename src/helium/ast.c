#pragma once
#include <helium/loc.c>
#include <helium/lexer.c>

enum helium_ast_type : int {
    helium_ast_type_expr_ch,
    helium_ast_type_expr_str,
    helium_ast_type_expr_ident,
    helium_ast_type_expr_num,
    helium_ast_type_expr_bool,
    helium_ast_type_expr_nothing,
    helium_ast_type_expr_binary,
    helium_ast_type_expr_prefix,
    helium_ast_type_expr_qualifiers,
    helium_ast_type_expr_func,
    helium_ast_type_expr_tuple,
    helium_ast_type_expr_type_array,
    helium_ast_type_expr_init,
    helium_ast_type_expr_call,
    helium_ast_type_expr_member,
    helium_ast_type_expr_index,
    helium_ast_type_expr_struct,
    helium_ast_type_expr_union,
    helium_ast_type_expr_enum,
    helium_ast_type_expr_typeof,
    helium_ast_type_expr_oper,
    helium_ast_type_expr_block,
    helium_ast_type_stmt_expr,
    helium_ast_type_stmt_block,
    helium_ast_type_stmt_if,
    helium_ast_type_stmt_for,
    helium_ast_type_stmt_return,
    helium_ast_type_stmt_break,
    helium_ast_type_stmt_continue,
};

struct helium_ast_expr_ch;
struct helium_ast_expr_str;
struct helium_ast_expr_ident;
struct helium_ast_expr_num;
struct helium_ast_type_expr_bool;
struct helium_ast_type_expr_nothing;
struct helium_ast_expr_binary;
struct helium_ast_expr_prefix;
struct helium_ast_expr_qualifiers;
struct helium_ast_expr_func;
struct helium_ast_expr_tuple;
struct helium_ast_expr_array;
struct helium_ast_expr_init;
struct helium_ast_expr_call;
struct helium_ast_expr_member;
struct helium_ast_expr_index;
struct helium_ast_expr_struct;
union helium_ast_expr;
struct helium_ast_stmt_block;
struct helium_ast_stmt_variable;
struct helium_ast_stmt_if;
struct helium_ast_stmt_for;
struct helium_ast_stmt_return;
struct helium_ast_stmt_break;
struct helium_ast_stmt_continue;
union helium_ast_stmt;

struct helium_ast_expr_ch {
    enum helium_ast_type ast_type;
    struct helium_loc loc;
    utf8_codepoint_t value;
};

struct helium_ast_expr_str {
    enum helium_ast_type ast_type;
    struct helium_loc loc;
    struct str value;
};

struct helium_ast_expr_ident {
    enum helium_ast_type ast_type;
    struct helium_loc loc;
    struct str_slice value;
};

enum helium_ast_num_type : bool {
    helium_ast_num_type_integer,
    helium_ast_num_type_floating,
};

struct helium_ast_expr_num {
    enum helium_ast_type ast_type;
    struct helium_loc loc;
    union {
        double floating;
        i64 integer;
    } value;
    enum helium_ast_num_type num_type;
};

struct helium_ast_expr_bool {
    enum helium_ast_type ast_type;
    struct helium_loc loc;
    bool value;
};

struct helium_ast_expr_nothing {
    enum helium_ast_type ast_type;
    struct helium_loc loc;
};

struct helium_ast_expr_binary {
    enum helium_ast_type ast_type;
    union helium_ast_expr *left;
    union helium_ast_expr *right;
    enum helium_token_type operator;
};

struct helium_ast_expr_prefix {
    enum helium_ast_type ast_type;
    union helium_ast_expr *value;
    enum helium_token_type operator;
};

struct helium_qualifiers {
    bool is_const;
    bool is_comptime;
    bool is_static;
};

struct helium_ast_expr_qualifiers {
    enum helium_ast_type ast_type;
    union helium_ast_expr *value;
    struct helium_qualifiers qualifiers;
};

struct helium_ast_func_param {
    struct helium_ast_expr_ident name;
    union helium_ast_expr *type;
    union helium_ast_expr *default_value;
};

struct helium_ast_func_params {
    struct helium_ast_func_param *params;
    size_t len;
};

struct helium_ast_expr_func {
    enum helium_ast_type ast_type;
    struct helium_ast_stmt_block *body;
    union helium_ast_expr *return_type;
    struct helium_ast_func_params params;
};

struct helium_ast_expr_tuple {
    enum helium_ast_type ast_type;
    union helium_ast_expr *values;
    size_t len;
};

struct helium_ast_expr_array {
    enum helium_ast_type ast_type;
    union helium_ast_expr *size;
    union helium_ast_expr *contents;
};

struct helium_ast_expr_init_field {
    struct helium_ast_expr_ident *name;
    union helium_ast_expr *value;
};

struct helium_ast_expr_init_fields {
    enum helium_ast_type ast_type;
    struct helium_ast_expr_init_field *fields;
    size_t len;
};

struct helium_ast_expr_init {
    enum helium_ast_type ast_type;
    union helium_ast_expr *type;
    struct helium_ast_expr_init_fields fields;
};

struct helium_ast_expr_call {
    enum helium_ast_type ast_type;
    union helium_ast_expr *calle;
    struct helium_ast_expr_init_fields args;
};

struct helium_ast_expr_member {
    enum helium_ast_type ast_type;
    union helium_ast_expr *indexe;
    struct helium_ast_expr_ident member;
};

struct helium_ast_expr_index {
    enum helium_ast_type ast_type;
    union helium_ast_expr *indexe;
    union helium_ast_expr *index;
};

struct helium_ast_indexable_field {
    bool is_variable;
    bool is_public;
    union {
        struct helium_ast_stmt_variable *variable;
        struct {
            struct helium_ast_expr_ident name;
            union helium_ast_expr *type;
            union helium_ast_expr *default_value;
        } field;
    } as;
};

struct helium_ast_expr_struct {
    enum helium_ast_type ast_type;
    struct helium_ast_indexable_field *fields;
    size_t len;
};

struct helium_union_tag {
    bool is_enum;
    union helium_ast_expr *expr;
};

struct helium_ast_expr_union {
    enum helium_ast_type ast_type;
    struct helium_union_tag tag;
    struct helium_ast_indexable_field *fields;
    size_t len;
};

struct helium_ast_enum_field {
    bool is_variable;
    bool is_public;
    union {
        struct helium_ast_stmt_variable *variable;
        struct {
            struct helium_ast_expr_ident name;
            union helium_ast_expr *value;
        } field;
    } as;
};

struct helium_ast_expr_enum {
    enum helium_ast_type ast_type;
    union helium_ast_expr *type;
    struct helium_ast_enum_field *fields;
    size_t len;
};

struct helium_ast_expr_typeof {
    enum helium_ast_type ast_type;
    struct helium_loc keyword_loc;
    union helium_ast_expr *value;
};

struct helium_ast_expr_oper {
    enum helium_ast_type ast_type;
    enum helium_token_type value;
    struct helium_loc loc;
};

union helium_ast_expr {
    enum helium_ast_type ast_type;
    struct helium_ast_expr_ch ch;
    struct helium_ast_expr_str str;
    struct helium_ast_expr_ident ident;
    struct helium_ast_expr_num num;
    struct helium_ast_expr_bool bool_;
    struct helium_ast_expr_nothing nothing;
    struct helium_ast_expr_binary binary;
    struct helium_ast_expr_prefix prefix;
    struct helium_ast_expr_qualifiers qualifiers;
    struct helium_ast_expr_func func;
    struct helium_ast_expr_tuple tuple;
    struct helium_ast_expr_array array;
    struct helium_ast_expr_init init;
    struct helium_ast_expr_call call;
    struct helium_ast_expr_member member;
    struct helium_ast_expr_index index;
    struct helium_ast_expr_struct struct_;
    struct helium_ast_expr_union union_;
    struct helium_ast_expr_enum enum_;
    struct helium_ast_expr_typeof typeof_;
    struct helium_ast_expr_oper oper;
    struct helium_ast_stmt_block block;
};

struct helium_ast_stmt_block {
    enum helium_ast_type ast_type;
    union helium_ast_stmt *stmts;
    struct helium_qualifiers qualifiers;
};

struct helium_ast_stmt_variable {
    enum helium_ast_type ast_type;
    struct helium_qualifiers qualifiers;
    struct helium_ast_expr_ident name;
    union helium_ast_expr *type;
    union helium_ast_expr *default_value;
};

struct helium_ast_stmt_if {
    enum helium_ast_type ast_type;
    struct helium_loc keyword_loc;
    union helium_ast_expr *condition;
    union helium_ast_stmt *case_true;
    union helium_ast_stmt *case_false;
};

struct helium_ast_stmt_for {
    enum helium_ast_type ast_type;
    struct helium_loc keyword_loc;
    union helium_ast_expr *leading;
    union helium_ast_expr *condition;
    union helium_ast_expr *post;
};

struct helium_ast_stmt_return {
    enum helium_ast_type ast_type;
    struct helium_loc keyword_loc;
    union helium_ast_expr *value;
};

struct helium_ast_stmt_break {
    enum helium_ast_type ast_type;
    struct helium_loc keyword_loc;
};

struct helium_ast_stmt_continue {
    enum helium_ast_type ast_type;
    struct helium_loc keyword_loc;
};

union helium_ast_stmt {
    enum helium_ast_type ast_type;
    union helium_ast_expr expr;
    struct helium_ast_stmt_block block;
    struct helium_ast_stmt_variable variable;
    struct helium_ast_stmt_if if_;
    struct helium_ast_stmt_for for_;
};