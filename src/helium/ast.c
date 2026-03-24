#ifndef _INCLUDE_HELIUM_AST_C
#define _INCLUDE_HELIUM_AST_C

#include <mem.c>
#include <helium/loc.c>
#include <helium/lexer.c>
#include <io.c>

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
    helium_ast_type_expr_array,
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
    helium_ast_type_stmt_variable,
    helium_ast_type__stmt_assignment,
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
struct helium_ast_expr_bool;
struct helium_ast_expr_nothing;
struct helium_ast_expr_binary;
struct helium_ast_expr_prefix;
struct helium_ast_expr_qualifiers;
struct helium_ast_expr_func;
struct helium_ast_expr_tuple;
struct helium_ast_expr_array;
struct helium_ast_expr_call;
struct helium_ast_expr_member;
struct helium_ast_expr_index;
struct helium_ast_expr_struct;
struct helium_ast_expr;
struct helium_ast_stmt_block;
struct helium_ast_stmt_variable;
struct helium_ast_stmt_assignment;
struct helium_ast_stmt_if;
struct helium_ast_stmt_for;
struct helium_ast_stmt_return;
struct helium_ast_stmt_break;
struct helium_ast_stmt_continue;
struct helium_ast_stmt;

struct helium_ast_expr_ch {
    struct helium_loc loc;
    utf8_codepoint_t value;
};

struct helium_ast_expr_str {
    struct helium_loc loc;
    struct str value;
};

struct helium_ast_expr_ident {
    struct helium_loc loc;
    struct str_slice value;
};

enum helium_ast_num_type : bool {
    helium_ast_num_type_integer,
    helium_ast_num_type_floating,
};

union helium_ast_num_value {
    f64 floating;
    u64 integer;
};

struct helium_ast_expr_num {
    struct helium_loc loc;
    size_t float_size;
    union helium_ast_num_value value;
    enum helium_ast_num_type num_type;
};

struct helium_ast_expr_bool {
    struct helium_loc loc;
    bool value;
};

struct helium_ast_expr_nothing {
    struct helium_loc loc;
};

struct helium_ast_expr_binary {
    struct helium_ast_expr *left;
    struct helium_ast_expr *right;
    struct helium_token operator;
};

struct helium_ast_expr_prefix {
    struct helium_ast_expr *value;
    struct helium_token operator;
};

struct helium_qualifiers {
    bool is_const;
    bool is_comptime;
    bool is_static;
};

struct helium_ast_expr_qualifiers {
    struct helium_ast_expr *value;
    struct helium_qualifiers qualifiers;
};

struct helium_ast_func_param {
    struct helium_ast_func_param *next;
    struct helium_ast_func_param *prev;
    struct helium_ast_expr_ident name;
    struct helium_ast_expr *type;
    struct helium_ast_expr *default_value;
};

struct helium_ast_func_params {
    struct helium_loc begin;
    struct helium_loc end;
    struct helium_ast_func_param *first;
    struct helium_ast_func_param *last;
};

struct helium_ast_expr_func {
    struct helium_loc keyword_loc;
    struct helium_ast_stmt *body;
    struct helium_ast_expr *return_type;
    struct helium_ast_func_params params;
};

struct helium_ast_expr_tuple_elem {
    struct helium_ast_expr_tuple_elem *next;
    struct helium_ast_expr_tuple_elem *prev;
    struct helium_ast_expr *expr;
};

struct helium_ast_expr_tuple {
    struct helium_loc begin;
    struct helium_loc end;
    struct helium_ast_expr_tuple_elem *first;
    struct helium_ast_expr_tuple_elem *last;
};

struct helium_ast_expr_array {
    struct helium_ast_expr *size;
    struct helium_ast_expr *contents;
};

struct helium_ast_expr_arg_field {
    struct helium_ast_expr *name;
    struct helium_ast_expr *value;
};

struct helium_ast_expr_arg_fields {
    struct helium_ast_expr_arg_field *first;
    struct helium_ast_expr_arg_field *last;
};

struct helium_ast_expr_arg {
    struct helium_ast_expr *type;
    struct helium_ast_expr_arg_fields fields;
};

struct helium_ast_expr_call {
    struct helium_ast_expr *calle;
    struct helium_ast_expr_arg_fields args;
};

struct helium_ast_expr_member {
    struct helium_ast_expr *indexe;
    struct helium_ast_expr_ident member;
};

struct helium_ast_expr_index {
    struct helium_ast_expr *indexe;
    struct helium_ast_expr *index;
};

struct helium_ast_indexable_field {
    bool is_variable;
    bool is_public;
    union {
        struct helium_ast_stmt_variable *variable;
        struct {
            struct helium_ast_expr_ident name;
            struct helium_ast_expr *type;
            struct helium_ast_expr *default_value;
        } field;
    } as;
};

struct helium_ast_expr_struct {
    struct helium_ast_indexable_field *fields;
    size_t len;
};

struct helium_union_tag {
    bool is_enum;
    struct helium_ast_expr *expr;
};

struct helium_ast_expr_union {
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
            struct helium_ast_expr *value;
        } field;
    } as;
};

struct helium_ast_expr_enum {
    struct helium_ast_expr *type;
    struct helium_ast_enum_field *fields;
    size_t len;
};

struct helium_ast_expr_typeof {
    struct helium_loc keyword_loc;
    struct helium_ast_expr *value;
};

struct helium_ast_expr_oper {
    struct helium_token value;
    struct helium_loc loc;
};

struct helium_ast_stmt_block {
    struct helium_loc begin;
    struct helium_loc end;
    struct helium_ast_stmt *first;
    struct helium_ast_stmt *last;
    struct helium_qualifiers qualifiers;
};

struct helium_ast_expr {
    enum helium_ast_type ast_type;
    union {
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
};

struct helium_ast_stmt_variable {
    struct helium_qualifiers qualifiers;
    struct helium_ast_expr *name;
    struct helium_ast_expr *type;
    struct helium_ast_expr *value;
};

struct helium_ast_stmt_assignment {
    struct helium_ast_expr *to;
    struct helium_ast_expr *value;
};

struct helium_ast_stmt_if {
    struct helium_loc keyword_loc;
    struct helium_ast_expr *condition;
    struct helium_ast_stmt *case_true;
    struct helium_ast_stmt *case_false;
};

struct helium_ast_stmt_for {
    struct helium_loc keyword_loc;
    struct helium_ast_expr *leading;
    struct helium_ast_expr *condition;
    struct helium_ast_expr *post;
};

struct helium_ast_stmt_return {
    struct helium_loc keyword_loc;
    struct helium_ast_expr *value;
};

struct helium_ast_stmt_break {
    struct helium_loc keyword_loc;
};

struct helium_ast_stmt_continue {
    struct helium_loc keyword_loc;
};

struct helium_ast_stmt {
    struct helium_ast_stmt *next;
    struct helium_ast_stmt *prev;
    enum helium_ast_type ast_type;
    union {
        struct helium_ast_expr *expr;
        struct helium_ast_stmt_block block;
        struct helium_ast_stmt_variable variable;
        struct helium_ast_stmt_if if_;
        struct helium_ast_stmt_for for_;
    };
};

void *helium_ast_stmt_free(struct allocator *allocator, struct helium_ast_stmt *stmt);
void *helium_ast_expr_free(struct allocator *allocator, struct helium_ast_expr *expr) {
    switch(expr->ast_type) {
        case helium_ast_type_expr_ch: break;
        case helium_ast_type_expr_str: {
            str_free(allocator, &expr->str.value);
        } break;
        case helium_ast_type_expr_ident: break;
        case helium_ast_type_expr_num: break;
        case helium_ast_type_expr_bool: break;
        case helium_ast_type_expr_nothing: break;
        case helium_ast_type_expr_binary: {
            helium_ast_expr_free(allocator, expr->binary.left);
            helium_ast_expr_free(allocator, expr->binary.right);
        } break;
        case helium_ast_type_expr_prefix: {
            helium_ast_expr_free(allocator, expr->prefix.value);
        } break;
        case helium_ast_type_expr_qualifiers: {
            helium_ast_expr_free(allocator, expr->qualifiers.value);
        } break;
        case helium_ast_type_expr_tuple: {
            for (struct helium_ast_expr_tuple_elem *node = expr->tuple.last; node != NULL;) {
                struct helium_ast_expr_tuple_elem *prev = node->prev;
                
                helium_ast_expr_free(allocator, node->expr);
                allocator->free(allocator, node, sizeof(*node));
                
                if (node == expr->tuple.first) break;
                node = prev;
            }
        } break;
        case helium_ast_type_expr_array: {
            if (expr->array.size) {
                helium_ast_expr_free(allocator, expr->array.size);
            }
            helium_ast_expr_free(allocator, expr->array.contents);
        } break;
        case helium_ast_type_expr_member: {
            helium_ast_expr_free(allocator, expr->member.indexe);
        } break;
        case helium_ast_type_expr_index: {
            helium_ast_expr_free(allocator, expr->index.indexe);
            helium_ast_expr_free(allocator, expr->index.index);
        } break;
        case helium_ast_type_expr_func: {
            helium_ast_stmt_free(allocator, expr->func.body);
            helium_ast_expr_free(allocator, expr->func.return_type);
        
            struct helium_ast_func_params *params = &expr->func.params;
            for (struct helium_ast_func_param *node = params->last; node != NULL;) {
                struct helium_ast_func_param *prev = node->prev;
                
                helium_ast_expr_free(allocator, node->type);
                if (node->default_value) {
                    helium_ast_expr_free(allocator, node->default_value);
                }
                allocator->free(allocator, node, sizeof(*node));
                
                if (node == params->first) break;
                node = prev;
            }
        } break;
    }
    
    return allocator->free(allocator, expr, sizeof(*expr));
}

void *helium_ast_stmt_free(struct allocator *allocator, struct helium_ast_stmt *stmt) {
    switch(stmt->ast_type) {
        case helium_ast_type_stmt_expr: {
            helium_ast_expr_free(allocator, stmt->expr);
        } break;
        case helium_ast_type_stmt_block: {
            for (struct helium_ast_stmt *node = stmt->block.last; node != NULL;) {
                struct helium_ast_stmt *prev = node->prev;
                
                helium_ast_stmt_free(allocator, node);
                
                if (node == stmt->block.first) break;
                node = prev;
            }
        } break;
        case helium_ast_type_stmt_variable: break;
        case helium_ast_type_stmt_if: break;
        case helium_ast_type_stmt_for: break;
    }
    
    return allocator->free(allocator, stmt, sizeof(*stmt));
}
    
#define TAB_LEN 4
#define DO_TABS() for (size_t i = 0; i < (depth * TAB_LEN); i++) putc(' ', out);
#define DO_MORE_DEPTH(VAR) for (bool _cond=((VAR++),true);_cond;_cond=((VAR--),false))

void helium_ast_pretty_print_expr(FILE *out, struct helium_ast_expr *expr, size_t depth);
void helium_ast_pretty_print_stmt(FILE *out, struct helium_ast_stmt *stmt, size_t depth);
void helium_ast_pretty_println_stmt(FILE *out, struct helium_ast_stmt *stmt, size_t depth);
void helium_ast_pretty_println_expr(FILE *out, struct helium_ast_expr *expr, size_t depth) {
    helium_ast_pretty_print_expr(out, expr, depth);
    fprintf(out, "\n");
}

void helium_ast_pretty_print_expr(FILE *out, struct helium_ast_expr *expr, size_t depth) {
    if (expr == NULL) {
        fprintf(out, "%p", NULL);
        return;
    }
    
    switch(expr->ast_type) {
        case helium_ast_type_expr_ch: {
            fprintf(out, "character('");
            switch(expr->ch.value) {
                case '\a': fprintf(out, "\\a"); break;
                case '\b': fprintf(out, "\\b"); break;
                case '\e': fprintf(out, "\\e"); break;
                case '\f': fprintf(out, "\\f"); break;
                case '\n': fprintf(out, "\\n"); break;
                case '\r': fprintf(out, "\\r"); break;
                case '\t': fprintf(out, "\\t"); break;
                case '\v': fprintf(out, "\\v"); break;
                default: fprintf(out, "%c", expr->ch.value);
            }
            fprintf(out, "')");
        } break;
        case helium_ast_type_expr_str: {
            fprintf(out, "string(\"");
            for (size_t i = 0; i < expr->str.value.len; i++) {
                char ch = expr->str.value.buf[i];
                switch(ch) {
                    case '\a': fprintf(out, "\\a"); break;
                    case '\b': fprintf(out, "\\b"); break;
                    case '\e': fprintf(out, "\\e"); break;
                    case '\f': fprintf(out, "\\f"); break;
                    case '\n': fprintf(out, "\\n"); break;
                    case '\r': fprintf(out, "\\r"); break;
                    case '\t': fprintf(out, "\\t"); break;
                    case '\v': fprintf(out, "\\v"); break;
                    default: fprintf(out, "%c", ch);
                }
            }
            fprintf(out, "\")");
        } break;
        case helium_ast_type_expr_ident: {
            fprintf(out, "identifier(%.*s)", expr->ident.value.len, expr->ident.value.buf);
        } break;
        case helium_ast_type_expr_num: {
            fprintf(out, "number(");
            if (expr->num.num_type == helium_ast_num_type_integer) {
                fprintf(out, "u64 %llu", expr->num.value.integer);
            } else {
                fprintf(out, "f64 %.*f", expr->num.float_size, expr->num.value.floating);
            }
            fprintf(out, ")");
        } break;
        case helium_ast_type_expr_bool: {
            fprintf(out, "bool(%s)", expr->bool_.value ? "true" : "false");
        } break;
        case helium_ast_type_expr_nothing: {
            fprintf(out, "nothing");
        } break;
        case helium_ast_type_expr_binary: {
            fprintf(out, "binary (%.*s) {\n", expr->binary.operator.slice.len, expr->binary.operator.slice.buf);
            DO_MORE_DEPTH(depth) {
                DO_TABS(); fprintf(out, "left: ");
                helium_ast_pretty_println_expr(out, expr->binary.left, depth);;
                
                DO_TABS(); fprintf(out, "right: ");
                helium_ast_pretty_println_expr(out, expr->binary.right, depth);
            }
            
            DO_TABS(); fprintf(out, "}");
        } break;
        case helium_ast_type_expr_prefix: {
            fprintf(out, "prefix {\n");
            DO_MORE_DEPTH(depth) {
                DO_TABS(); fprintf(out, "operator: (%.*s)\n", expr->prefix.operator.slice.len, expr->prefix.operator.slice.buf);
                
                DO_TABS(); fprintf(out, "value: ");
                helium_ast_pretty_println_expr(out, expr->prefix.value, depth);
            }
            
            DO_TABS(); fprintf(out, "}");
        } break;
        case helium_ast_type_expr_qualifiers: {
            depth++;
            fprintf(out, "qualifiers {\n");
            struct helium_qualifiers {
                bool is_const;
                bool is_comptime;
                bool is_static;
            };
            struct helium_qualifiers qualifiers = {
                .is_const = expr->qualifiers.qualifiers.is_const,
                .is_comptime = expr->qualifiers.qualifiers.is_comptime,
                .is_static = expr->qualifiers.qualifiers.is_static
            };
            
            DO_TABS();
            fprintf(out, "mutability: %s\n", qualifiers.is_const ? "const" : "var");
            DO_TABS();
            fprintf(out, "evaluation: %s\n", qualifiers.is_comptime ? "comptime" : "runtime");
            DO_TABS();
            fprintf(out, "static: %s\n", qualifiers.is_static ? "true" : "false");

            DO_TABS();
            fprintf(out, "value: ");
            helium_ast_pretty_println_expr(out, expr->qualifiers.value, depth);
            
            depth--;
            DO_TABS();
            fprintf(out, "}");
        } break;
        case helium_ast_type_expr_tuple: {
            if (expr->tuple.first == NULL) {
                fprintf(out, "tuple []");
                break;
            }
            depth++;
            fprintf(out, "tuple [\n");
            
            struct helium_ast_expr_tuple_elem *node = expr->tuple.first;
            while (node != NULL) {
                DO_TABS();
                fprintf(out, "");
                helium_ast_pretty_print_expr(out, node->expr, depth);
                
                node = node->next;
                if (node == expr->tuple.first) {
                    fprintf(out, "\n");
                    break;
                }
                fprintf(out, ",\n");
            }
            
            depth--;
            DO_TABS();
            fprintf(out, "]");
        } break;
        case helium_ast_type_expr_array: {
            fprintf(out, "array {\n");
            
            depth++;
            if (expr->array.size) {
                DO_TABS();
                fprintf(out, "size: ");
                helium_ast_pretty_println_expr(out, expr->array.size, depth);
            }
            
            DO_TABS();
            fprintf(out, "contents: ");
            helium_ast_pretty_println_expr(out, expr->array.contents, depth);
            depth--;
            
            DO_TABS();
            fprintf(out, "}");
        } break;
        case helium_ast_type_expr_member: {
            fprintf(out, "member {\n");
            
            depth++;
            DO_TABS();
            fprintf(out, "indexe: ");
            helium_ast_pretty_println_expr(out, expr->member.indexe, depth);
            
            DO_TABS();
            fprintf(out, "index: identifier(%.*s)\n", expr->member.member.value.len, expr->member.member.value.buf);
            depth--;
            
            DO_TABS();
            fprintf(out, "}");
        } break;
        case helium_ast_type_expr_index: {
            fprintf(out, "index {\n");
            
            depth++;
            DO_TABS();
            fprintf(out, "indexe: ");
            helium_ast_pretty_println_expr(out, expr->index.indexe, depth);
            
            DO_TABS();
            fprintf(out, "index: ");
            helium_ast_pretty_println_expr(out, expr->index.index, depth);
            depth--;
            
            DO_TABS();
            fprintf(out, "}");
        } break;
        case helium_ast_type_expr_func: {
            fprintf(out, "func {\n");
            
            DO_MORE_DEPTH(depth) {
                DO_TABS();
                fprintf(out, "return_type: ");
                helium_ast_pretty_println_expr(out, expr->func.return_type, depth);
                
                DO_TABS(); fprintf(out, "params: [");
                if (expr->func.params.first != NULL) DO_MORE_DEPTH(depth) {
                    struct helium_ast_func_param *node = expr->func.params.first;
                    for (;;) {
                        fprintf(out, "\n"); DO_TABS();
                        fprintf(out, "%.*s: {\n", node->name.value.len, node->name.value.buf);
                        DO_MORE_DEPTH(depth) {
                            DO_TABS();
                            fprintf(out, "type: ");
                            helium_ast_pretty_println_expr(out, node->type, depth);
                            
                            if (node->default_value) {
                                DO_TABS();
                                fprintf(out, "default_value: ");
                                helium_ast_pretty_println_expr(out, node->default_value, depth);
                            }
                        }
                        DO_TABS(); fprintf(out, "}");
                        
                        node = node->next;
                        if (node == expr->func.params.first) {
                            fprintf(out, "\n");
                            depth--;DO_TABS();depth++;
                            break;
                        }
                    }
                }
                fprintf(out, "]\n");
                
                DO_TABS();
                fprintf(out, "body: ");
                helium_ast_pretty_println_stmt(out, expr->func.body, depth);
            }
            
            DO_TABS();
            fprintf(out, "}");
        } break;
    }
}

void helium_ast_pretty_println_stmt(FILE *out, struct helium_ast_stmt *stmt, size_t depth) {
    helium_ast_pretty_print_stmt(out, stmt, depth);
    fprintf(out, "\n");
}

void helium_ast_pretty_print_stmt(FILE *out, struct helium_ast_stmt *stmt, size_t depth) {
    struct helium_ast_stmt *first = stmt;
    
    for (;;) {
        switch(stmt->ast_type) {
            case helium_ast_type_stmt_expr: {
                helium_ast_pretty_print_expr(out, stmt->expr, depth);
            } break;
            case helium_ast_type_stmt_block: {
                fprintf(out, "block ");
                
                if (stmt->block.qualifiers.is_const || stmt->block.qualifiers.is_comptime) {
                    fprintf(out, "(");
                    if (stmt->block.qualifiers.is_comptime) {
                        fprintf(out, "comptime");
                        
                        if (stmt->block.qualifiers.is_const) {
                            fprintf(out, " ");
                        }
                    }
                    
                    if (stmt->block.qualifiers.is_const) {
                        fprintf(out, "const");
                    }
                    fprintf(out, ") ");
                }
                
                fprintf(out, "[");
                if (stmt->block.first != NULL) {
                    DO_MORE_DEPTH(depth) {
                        fprintf(out, "\n");
                        DO_TABS();
                        helium_ast_pretty_print_stmt(out, stmt->block.first, depth);
                    }
                }
                if (stmt->block.first != NULL) {
                    fprintf(out, "\n"); 
                    DO_TABS();
                }
                fprintf(out, "]");
            } break;
            case helium_ast_type_stmt_variable: break;
            case helium_ast_type_stmt_if: break;
            case helium_ast_type_stmt_for: break;
        }
        
        stmt = stmt->next;
        if (stmt == first) break;
        if (stmt == NULL) break;
        fprintf(out, ",\n");
        DO_TABS();
    }
    #undef TAB_LEN
    #undef DO_TABS
    #undef DO_INDENT
}

#endif // _INCLUDE_HELIUM_AST_C