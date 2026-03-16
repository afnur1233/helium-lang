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
    
    while (helium_can_lex(lexer)) {
        struct helium_token token = helium_lex(&lexer);
        
        printf(
            "%26s \e[2m:\e[0m %.*s\n",
            helium_str_token_type(token.type),
            token.data.len,
            token.data.buf
        );
    }
}