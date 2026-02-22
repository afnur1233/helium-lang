#include <stdio.h>
#include "str.c"
#include "helium/lexer.c"
#include "helium/parser.c"

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

	StrSlice file_path = str_slice_from_lit("files/syntax_test.hel");
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
	for (Helium_Token lexing; true;) {
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
	};
	
	Helium_Parser parser = {
		.source = str_slice(&contents, 0, contents.len),
		.file_path = file_path,
		.pos = 0,
		.tokens = tokens,
		.token_count = token_count,
	};
	
	Helium_ParseExprResult parse_res = helium_parse_expr(&parser);
	Str str = parse_res.as.expr.str.value;
	printf("\"%.*s\"\n", str.len, str.buf);
}