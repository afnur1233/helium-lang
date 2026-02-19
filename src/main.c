#include <stdio.h>
#include "str.c"
#include "helium/lexer.c"

int main() {
	FILE *file = fopen("files/syntax_test.hel", "r");
	if (file == NULL) {
		perror("[ERROR(fopen, 'test.hel' in read mode)]");
		return -1;
	}
	Str contents = {0};
	StrResult res = str_read_entire_file(&contents, file);
	if (res != str_result_ok) {
		printf("[ERROR(str_read_entire_file)]: %i\n", res);
		return -1;
	}
	
	fclose(file);

	Helium_Lexer lexer = (Helium_Lexer){
		.input = str_slice(&contents, 0, contents.len),
		.file_path = (StrSlice){ .buf = "(temp)", .len = 6 },
		.line_num = 1,
		.line_begin = 0,
		.column = 1,
		.pos = 0,
	};
	
	Helium_Token tok = helium_lexer_next(&lexer);
	while (tok.type != helium_token_type_end) {
		printf("[%s] %.*s\n", helium_str_token_type(tok.type), tok.slice.len, tok.slice.buf);
		tok = helium_lexer_next(&lexer);
	}
}