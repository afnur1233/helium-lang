#include <stdio.h>
#include <helium/lexer.c>

bool read_entire_file(struct allocator *allocator, struct str *str, FILE *file) {
    for (;;) {
        char buffer[256];
        size_t count = fread(buffer, 1, arr_sizeof(buffer), file);
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
    const char path[] = "./files/basic_main.he";
    struct str source = {0};
    {
        FILE *file = fopen(path, "rb");
        if (!file) return perror("Failed to read file"), 1;
        
        bool success = read_entire_file(&allocator_libc, &source, file);
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
    if (!helium_lex_all(&allocator_libc, &lexer, &tokens)) return false;
    
    for (size_t i = 0; i < tokens.len; i++) {
        struct helium_token token = tokens.items[i];
        
        printf(
            "%26s \e[2m:\e[0m %.*s\n",
            helium_str_token_type(token.type),
            token.slice.len,
            token.slice.buf
        );
    }
}