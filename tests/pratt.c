#include <stdio.h>
#include <helium/lexer.c>
#include <helium/parser.c>

// Currently 'finished' ast nodes
// [+] helium_ast_type_expr_ch
// [+] helium_ast_type_expr_str
// [+] helium_ast_type_expr_ident
// [+] helium_ast_type_expr_num
// [+] helium_ast_type_expr_bool
// [+] helium_ast_type_expr_nothing
// [+] helium_ast_type_expr_binary
// [+] helium_ast_type_expr_prefix
// [+] helium_ast_type_expr_qualifiers
// [+] helium_ast_type_expr_func
// [+] helium_ast_type_expr_tuple
// [+] helium_ast_type_expr_array
// [ ] helium_ast_type_expr_call
// [+] helium_ast_type_expr_member
// [+] helium_ast_type_expr_index
// [ ] helium_ast_type_expr_struct
// [ ] helium_ast_type_expr_union
// [ ] helium_ast_type_expr_enum
// [ ] helium_ast_type_expr_typeof
// [ ] helium_ast_type_expr_oper
// [ ] helium_ast_type_expr_block
// [+] helium_ast_type_stmt_expr
// [+] helium_ast_type_stmt_block
// [ ] helium_ast_type_stmt_variable
// [ ] helium_ast_type_stmt_if
// [ ] helium_ast_type_stmt_for
// [ ] helium_ast_type_stmt_return
// [ ] helium_ast_type_stmt_break
// [ ] helium_ast_type_stmt_continue

bool read_entire_file(struct allocator *allocator, struct str *str, FILE *file) {
    for (;;) {
        char buffer[256];
        size_t count = fread(buffer, 1, sizeof(buffer), file);
        if (ferror(file)) {
            return false;
        }
        
        if (!str_append(allocator, str, buffer, count)) {
            return false;
        }
        
        if (feof(file)) {
            break;
        }
    }
    
    return true;
}

int main(void) {
    struct allocator_tracking tracking = { .allocator = &allocator_libc };
    struct allocator allocator = allocator_from_tracking(&tracking);
    const char path[] = "./files/binary.he";
    struct str source = {0};
    {
        FILE *file = fopen(path, "rb");
        if (!file) return perror("Failed to read file"), 1;
        
        bool success = read_entire_file(&allocator, &source, file);
        fclose(file);
        
        if (!success) {
            return 1;
        }
    }
    
    struct helium_lexer lexer = {
        .iterator = (struct iterator_utf8){
            .slice = (struct str_slice){
                .buf = source.buf,
                .len = source.len,
            },
        },
        .loc = (struct helium_lexer_loc){
            .file_path = STR_SLICE_LIT(path),
        }
    };
    
    struct helium_tokens tokens = {0};
    if (!helium_lex_all(&allocator, &lexer, &tokens)) return false;
    
    struct helium_syntax_err err = {0};
    struct helium_parser parser = {
        .tokens = tokens,
        .source = (struct str_slice){
            .buf = source.buf,
            .len = source.len,
        },
    };
    helium_parser_lookups_set_default(&parser);
    
    FILE *out = stdout;
    if (!helium_parser_check_braces(&allocator, parser, &err)) {
        helium_syntax_err_pretty_print(out, parser, err, true);
        return 0;
    }
    
    struct helium_ast_stmt *stmt = helium_pratt_stmt(&allocator, &parser, &err);
    
    if (stmt) {
        helium_ast_pretty_print_stmt(out, stmt, 0);
        fprintf(out, "\n");
        helium_ast_stmt_free(&allocator, stmt);
    } else {
        helium_syntax_err_pretty_print(out, parser, err, true);
    }
    
    str_free(&allocator, &source);
    da_free(&allocator, &tokens);
    
    return 0;
}