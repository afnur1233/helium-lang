#pragma once
#include <ctype.h>
#include "int.c"
#include "str.c"
#include "loc.c"

enum : i32 {
	helium_token_type_end,
	helium_token_type_shebang,
	helium_token_type_comment,
	helium_token_type_doc,
	helium_token_type_unkown,
	helium_token_type_str,
	helium_token_type_str_unfinished,
	helium_token_type_str_multiline,
	helium_token_type_ident,
	helium_token_type_num_decimal,
	helium_token_type_num_float,
	helium_token_type_num_decimal_malformed,
	helium_token_type_num_float_malformed,
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
	helium_token_type_keyword_and,
	helium_token_type_keyword_not,
	helium_token_type_keyword_or,
	helium_token_type_keyword_if,
	helium_token_type_keyword_else,
	helium_token_type_keyword_nothing,
	helium_token_type_keyword_import,
	helium_token_type_keyword_struct,
	helium_token_type_keyword_type,
	helium_token_type_keyword_typeof,
	helium_token_type_keyword_cast,
	helium_token_type_keyword_const,
	helium_token_type_keyword_comptime,
	helium_token_type_keyword_func,
	helium_token_type_keyword_for,
	helium_token_type_keyword_return,
	helium_token_type_keyword_let,
	helium_token_type_keyword_continue,
	helium_token_type_keyword_break,
	helium_token_type_keyword_union,
	helium_token_type_keyword_enum,
	helium_token_type_keyword_pub,
};

typedef struct Helium_Token {
	i32 type;
	StrSlice slice;
	Helium_Loc loc;
} Helium_Token;

typedef struct Helium_Lexer {
	StrSlice input;
	StrSlice file_path;
	u64 line_num;
	u64 line_begin;
	u64 column;
	u64 pos;
} Helium_Lexer;

typedef struct Helium_Keyword {
	StrSlice val;
	i32 type;
} Helium_Keyword;

#define X(str, T)((Helium_Keyword){\
	.val = (StrSlice){ .buf = str, .len = sizeof(str) - 1  },\
	.type = T,\
})
const Helium_Keyword helium_keywords[] = {
	X("and", helium_token_type_keyword_and),
	X("not", helium_token_type_keyword_not),
	X("or", helium_token_type_keyword_or),
	X("if", helium_token_type_keyword_if),
	X("else", helium_token_type_keyword_else),
	X("nothing", helium_token_type_keyword_nothing),
	X("import", helium_token_type_keyword_import),
	X("struct", helium_token_type_keyword_struct),
	X("type", helium_token_type_keyword_type),
	X("typeof", helium_token_type_keyword_typeof),
	X("cast", helium_token_type_keyword_cast),
	X("const", helium_token_type_keyword_const),
	X("comptime", helium_token_type_keyword_comptime),
	X("func", helium_token_type_keyword_func),
	X("for", helium_token_type_keyword_for),
	X("return", helium_token_type_keyword_return),
	X("let", helium_token_type_keyword_let),
	X("continue", helium_token_type_keyword_continue),
	X("break", helium_token_type_keyword_break),
	X("union", helium_token_type_keyword_union),
	X("enum", helium_token_type_keyword_enum),
	X("pub", helium_token_type_keyword_pub),
};
#undef X

const char *helium_str_token_type(i32 type) {
	switch (type) {
		case helium_token_type_end: return "end";
		case helium_token_type_shebang: return "shebang";
		case helium_token_type_comment: return "comment";
		case helium_token_type_doc: return "doc";
		case helium_token_type_unkown: return "unkown";
		case helium_token_type_str: return "str";
		case helium_token_type_str_unfinished: return "str(unfinished)";
		case helium_token_type_str_multiline: return "str(multiline)";
		case helium_token_type_ident: return "ident";
		case helium_token_type_num_decimal: return "num_decimal";
		case helium_token_type_num_float: return "num_float";
		case helium_token_type_num_decimal_malformed: return "num_decimal(malformed)";
		case helium_token_type_num_float_malformed: return "num_float(malformed)";
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
		case helium_token_type_keyword_and: return "keyword(and)";
		case helium_token_type_keyword_not: return "keyword(not)";
		case helium_token_type_keyword_or: return "keyword(or)";
		case helium_token_type_keyword_if: return "keyword(if)";
		case helium_token_type_keyword_else: return "keyword(else)";
		case helium_token_type_keyword_nothing: return "keyword(nothing)";
		case helium_token_type_keyword_import: return "keyword(import)";
		case helium_token_type_keyword_struct: return "keyword(struct)";
		case helium_token_type_keyword_type: return "keyword(type)";
		case helium_token_type_keyword_typeof: return "keyword(typeof)";
		case helium_token_type_keyword_cast: return "keyword(cast)";
		case helium_token_type_keyword_const: return "keyword(const)";
		case helium_token_type_keyword_comptime: return "keyword(comptime)";
		case helium_token_type_keyword_func: return "keyword(func)";
		case helium_token_type_keyword_for: return "keyword(for)";
		case helium_token_type_keyword_return: return "keyword(return)";
		case helium_token_type_keyword_let: return "keyword(let)";
		case helium_token_type_keyword_continue: return "keyword(continue)";
		case helium_token_type_keyword_break: return "keyword(break)";
		case helium_token_type_keyword_union: return "keyword(union)";
		case helium_token_type_keyword_enum: return "keyword(enum)";
		case helium_token_type_keyword_pub: return "keyword(pub)";
	}
	return "#unknown";
}

u8 helium_lexer_eat(Helium_Lexer *lexer) {
	u8 ch = str_slice_at(&lexer->input, lexer->pos);
	if (ch == '\0') {
		return '\0';
	}
	
	lexer->pos++;
	lexer->column++;
	if (ch == '\n') {
		lexer->line_num++;
		lexer->line_begin = lexer->pos;
	}
	
	return ch;
}

StrSlice helium_lexer_eat_whitespace(Helium_Lexer *lexer) {
	u64 begin = lexer->pos;
	u64 len = 0;
	while (isspace(str_slice_at(&lexer->input, lexer->pos))) {
		helium_lexer_eat(lexer);
		len++;
	}
	return (StrSlice){ .buf = lexer->input.buf + begin, .len = len };
}

Helium_Token helium_lexer_next(Helium_Lexer *lexer) {
	helium_lexer_eat_whitespace(lexer);
	
	u64 line_num = lexer->line_num;
	u64 line_begin = lexer->line_begin;
	u64 column = lexer->column;
	u64 begin = lexer->pos;
	StrSlice file_path = lexer->file_path;
	#define MAKE_LOC(_len)((Helium_Loc){\
		.line_num = line_num,\
		.line_begin = line_begin,\
		.column = column,\
		.pos = begin,\
		.file_path = file_path,\
		.len = _len,\
	})
	
	if (lexer->pos >= lexer->input.len) {
		return (Helium_Token){
			.type = helium_token_type_end,
			.slice = (StrSlice){ .buf = lexer->input.buf + lexer->input.len, .len = 0 },
			.loc = MAKE_LOC(0),
		};
	}
	u8 ch = helium_lexer_eat(lexer);
	
	switch (ch) {
		case '"': {
			u64 len = 1;
			u8 prev = ch;
			ch = str_slice_at(&lexer->input, lexer->pos);
			while (ch != '\n' && ch != '\0' && !(prev != '\\' && ch == '"')) {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				len++;
			}
			
			if (ch == '"') {
				helium_lexer_eat(lexer);
				len++;
				return (Helium_Token){
					.type = helium_token_type_str,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len },
					.loc = MAKE_LOC(len),
				};
			}
			return (Helium_Token){
				.type = helium_token_type_str_unfinished,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len },
				.loc = MAKE_LOC(len),
			};
		}
		
		case '\\': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch != '\\') {
				break;
			}
			int len = 2;
			
			helium_lexer_eat(lexer);
			ch = str_slice_at(&lexer->input, lexer->pos);
			
			while (ch != '\n' && ch != '\0') {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				len++;
			}
			
			return (Helium_Token){
				.type = helium_token_type_str_multiline,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len },
				.loc = MAKE_LOC(len),
			};
		}
		
		case 'a'...'z':
		case 'A'...'Z':
		case '_': {
			u64 len = 1;
			ch = str_slice_at(&lexer->input, lexer->pos);
			while (isalnum(ch) || ch == '_') {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				len++;
			}
			
			i32 type = helium_token_type_ident;
			StrSlice slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len };
			for (u64 i = 0; i < sizeof(helium_keywords)/sizeof(helium_keywords[0]); i++) {
				if (str_slice_eq(&slice, &helium_keywords[i].val)) {
					type = helium_keywords[i].type;
					break;
				}
			}
			
			return (Helium_Token){
				.type = type,
				.slice = slice,
				.loc = MAKE_LOC(len),
			};
		};
		
		case '.':
		case '0'...'9': {
			u8 prev = ch;
			ch = str_slice_at(&lexer->input, lexer->pos);
			
			if (prev == '.' && !isdigit(ch) && ch != 'e' && ch != 'E') {
				if (ch == '.') {
					helium_lexer_eat(lexer);
					ch = str_slice_at(&lexer->input, lexer->pos);
					if (ch == '.') {
						helium_lexer_eat(lexer);
						return (Helium_Token){
							.type = helium_token_type_dot_dot_dot,
							.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 3 },
							.loc = MAKE_LOC(3),
						};
					} else if (ch == '=') {
						helium_lexer_eat(lexer);
						return (Helium_Token){
							.type = helium_token_type_dot_dot_equals,
							.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 3 },
							.loc = MAKE_LOC(3),
						};
					}
					
					return (Helium_Token){
						.type = helium_token_type_dot_dot,
						.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
						.loc = MAKE_LOC(2),
					};
				}
				
				return (Helium_Token){
					.type = helium_token_type_dot,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
					.loc = MAKE_LOC(1),
				};
			}
			
			i32 type =
			(ch == '.')
			? (helium_token_type_num_float)
			: (helium_token_type_num_decimal);
			u64 len = 1;
			ch = str_slice_at(&lexer->input, lexer->pos);
			
			while (isdigit(ch) || ch == '_') {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				len++;
			}
			
			return (Helium_Token){
				.type = type,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len },
				.loc = MAKE_LOC(len),
			};
		}
		
		case '#': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch != '!') {
				break;
			}
			u64 len = 1;
			
			while (ch != '\0' && ch != '\n') {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				len++;
			}
				
			return (Helium_Token){
				.type = helium_token_type_shebang,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len },
				.loc = MAKE_LOC(len),
			};
		}
		
		
		case '(': return (Helium_Token){
			.type = helium_token_type_paren_round_open,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case ')': return (Helium_Token){
			.type = helium_token_type_paren_round_close,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case '[': return (Helium_Token){
			.type = helium_token_type_paren_block_open,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case ']': return (Helium_Token){
			.type = helium_token_type_paren_block_close,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case '{': return (Helium_Token){
			.type = helium_token_type_paren_curly_open,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case '}': return (Helium_Token){
			.type = helium_token_type_paren_curly_close,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case '+': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_plus_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_plus,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '-': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_minus_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_minus,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '%': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_percent_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_percent,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '/': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_slash_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			} else if (ch == '/') {
				u64 len = 1;
				u64 slash_count = 1;
				while (ch == '/') {
					helium_lexer_eat(lexer);
					ch = str_slice_at(&lexer->input, lexer->pos);
					len++;
					slash_count++;
				}
				
				i32 type = (slash_count == 3)
					? helium_token_type_doc
					: helium_token_type_comment;
				
				while (ch != '\n' && ch != '\0') {
					helium_lexer_eat(lexer);
					ch = str_slice_at(&lexer->input, lexer->pos);
					len++;
				}
				
				return (Helium_Token){
					.type = type,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = len },
					.loc = MAKE_LOC(len),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_slash,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '*': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_star_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_star,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '~': return (Helium_Token){
			.type = helium_token_type_bitwise_not,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case '^': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_bitwise_xor_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_bitwise_xor,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '&': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_bitwise_and_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_bitwise_and,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '|': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_bitwise_or_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_bitwise_or,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '<': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_less_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			} else if (ch == '<') {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				if (ch == '=') {
					helium_lexer_eat(lexer);
					return (Helium_Token){
						.type = helium_token_type_bitwise_shift_left_equals,
						.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 3 },
						.loc = MAKE_LOC(3),
					};
				}
				
				return (Helium_Token){
					.type = helium_token_type_bitwise_shift_left,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_less,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '>': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_greater_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			} else if (ch == '>') {
				helium_lexer_eat(lexer);
				ch = str_slice_at(&lexer->input, lexer->pos);
				if (ch == '=') {
					helium_lexer_eat(lexer);
					return (Helium_Token){
						.type = helium_token_type_bitwise_shift_right_equals,
						.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 3 },
						.loc = MAKE_LOC(3),
					};
				}
				
				return (Helium_Token){
					.type = helium_token_type_bitwise_shift_right,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_greater,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case ':': return (Helium_Token){
			.type = helium_token_type_colon,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case '=': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_equals_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_equals,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '!': {
			ch = str_slice_at(&lexer->input, lexer->pos);
			if (ch == '=') {
				helium_lexer_eat(lexer);
				return (Helium_Token){
					.type = helium_token_type_not_equals,
					.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 2 },
					.loc = MAKE_LOC(2),
				};
			}
			
			return (Helium_Token){
				.type = helium_token_type_exclamation_mark,
				.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
				.loc = MAKE_LOC(1),
			};
		}
		
		case '?': return (Helium_Token){
			.type = helium_token_type_question_mark,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case ',': return (Helium_Token){
			.type = helium_token_type_comma,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
		
		case ';': return (Helium_Token){
			.type = helium_token_type_semi_colon,
			.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
			.loc = MAKE_LOC(1),
		};
	}
	
	return (Helium_Token){
		.type = helium_token_type_unkown,
		.slice = (StrSlice){ .buf = lexer->input.buf + begin, .len = 1 },
		.loc = MAKE_LOC(1),
	};
	#undef MAKE_LOC
}