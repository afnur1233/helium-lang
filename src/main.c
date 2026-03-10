#include <stdio.h>
#include "str.c"
#include "helium/lexer.c"

int main() {
    FILE *file = fopen("files/syntax_test.hel", "r");
    if (file == NULL) {
        perror("[ERROR(fopen 'test.hel' in read mode)]");
        return -1;
    }
    struct str contents = {0};
    StrResult res = str_read_entire_file(&contents, file);
    if (res != str_result_ok) {
        printf("[ERROR(str_read_entire_file)]: %i\n", res);
        return -1;
    }
    
    fclose(file);

    struct str_slice file_path = str_slice_from_lit("files/syntax_test.hel");
    Helium_Lexer lexer = (Helium_Lexer){
        .input = str_slice(&contents, 0, contents.len),
        .file_path = file_path,
        .line_num = 1,
        .line_begin = 0,
        .column = 0,
        .pos = 0,
    };
    u64 token_count = 0;
    Helium_Token *tokens = NULL;
    for (Helium_Token lexing;;) {
        lexing = helium_lexer_next(&lexer);
        if (lexing.type == helium_token_type_end) {
            break;
        }
        
        if (tokens == NULL) {
            tokens = malloc(sizeof(*tokens) * (token_count + 1));
        } else {
            tokens = realloc(tokens, sizeof(*tokens) * (token_count + 1));
        }
        if (tokens == NULL) {
            printf("Failed allocation for token\n");
            return -1;
        }
        tokens[token_count++] = lexing;
        printf("%.*s\n", lexing.slice.len, lexing.slice.buf);
    };
}