#pragma once
#include "helium/lexer.c"
#include "helium/ast.c"
#include "str.c"

typedef struct Helium_ParserError Helium_ParserError;
struct Helium_ParserError {
	StrSlice message;
	Helium_Loc loc;
};

typedef struct Helium_ParseExprResult Helium_ParseExprResult;
struct Helium_ParseExprResult {
	enum : i32 {
		helium_parseexpr_success,
		helium_parseexpr_parse_error,
		helium_parseexpr_str_error,
	} status;
	union {
		Helium_Ast_NodeExpr expr;
		Helium_ParserError parse_error;
		StrResult str_error;
	} as;
};
#define Helium_ParseExprResult_Ok(...) \
	(Helium_ParseExprResult){ \
		.status = helium_parseexpr_success, \
		.as = { \
			.expr = (Helium_Ast_NodeExpr)(__VA_ARGS__), \
		}, \
	}
#define Helium_ParseExprResult_ErrParse(...) \
	(Helium_ParseExprResult){ \
		.status = helium_parseexpr_parse_error, \
		.as = { \
			.parse_error = ((Helium_ParserError)__VA_ARGS__), \
		}, \
	}
#define Helium_ParseExprResult_ErrStr(...) \
	(Helium_ParseExprResult){ \
		.status = helium_parseexpr_str_error, \
		.as = { \
			.str_error = ((StrResult)__VA_ARGS__), \
		}, \
	}

typedef struct Helium_Parser Helium_Parser;
struct Helium_Parser {
	u64 pos;
	u64 token_count;
	Helium_Token *tokens;
	StrSlice source;
	StrSlice file_path;
};
Helium_ParseExprResult helium_parse_expr(Helium_Parser *parser);
Helium_ParseExprResult helium_parse_expr_primary(Helium_Parser *parser);

Helium_ParseExprResult helium_parse_expr(Helium_Parser *parser) {
	return helium_parse_expr_primary(parser);
}

Helium_ParseExprResult helium_parse_expr_primary(Helium_Parser *parser) {
	Helium_Token *token = &parser->tokens[parser->pos++];
	StrSlice token_slice = str_slice_slice(&parser->source, token->loc.pos, token->loc.len);
	
	switch (token->type) {
		// TODO: Add exponential support: 1e+5, 57.5e-7
		case helium_token_type_num: {
			bool is_float = false;
			Helium_Ast_NumVal value = {
				.integral = 0,
			};
			u64 floating_size = 0;
			
			for (u64 i = 0; i < token_slice.len; i++) {
				u8 ch = token_slice.buf[i];
				if (ch == '_') {
					continue;
				}
				if (ch == '.') {
					is_float = true;
					value.floating = (f64)value.integral;
					continue;
				}
				
				if (isdigit(ch)) {
					if (is_float) {
						value.floating += (ch - '0') / pow(10, ++floating_size);
					} else {
						value.integral *= 10;
						value.integral += ch - '0';
					}
				}
			}
			
			return Helium_ParseExprResult_Ok((Helium_Ast_NodeExprNum){
				.type = helium_ast_nodetype_expr_num,
				.loc = token->loc,
				.is_float = is_float,
				.as = value,
			});
		} break;
		
		case helium_token_type_ident: {
			return Helium_ParseExprResult_Ok((Helium_Ast_NodeExprIdent){
				.type = helium_ast_nodetype_expr_ident,
				.loc = token->loc,
				.value = token_slice,
			});
		} break;
		
		// TODO: Implement full C escape sequence support
		case helium_token_type_str: {
			Str str = {0};
			StrResult str_error = str_alloc(&str, token_slice.len);
			if (str_error != str_result_ok) {
				return Helium_ParseExprResult_ErrStr(str_error);
			}
			
			for (u64 i = 1; i < token_slice.len - 2; i++) {
				u8 ch = token_slice.buf[i];
				if (ch == '\\') {
					ch = token_slice.buf[++i];
					switch (ch) {
						case 'a': str_append(&str, "\a", 1); break;
						case 'b': str_append(&str, "\b", 1); break;
						case 'e': str_append(&str, "\e", 1); break;
						case 'f': str_append(&str, "\f", 1); break;
						case 'n': str_append(&str, "\n", 1); break;
						case 'r': str_append(&str, "\r", 1); break;
						case 't': str_append(&str, "\t", 1); break;
						case 'v': str_append(&str, "\v", 1); break;
						case '\\': str_append(&str, "\\", 1); break;
						case '\'': str_append(&str, "\'", 1); break;
						case '\"': str_append(&str, "\"", 1); break;
						default: str_append(&str, &ch, 1); break;
					}
					continue;
				}
				str_append(&str, &ch, 1);
			}
			
			return Helium_ParseExprResult_Ok((Helium_Ast_NodeExprStr){
				.type = helium_ast_nodetype_expr_ident,
				.loc = token->loc,
				.value = str,
			});
		} break;
	}
	
	return Helium_ParseExprResult_ErrParse({
		.message = str_slice_from_lit("Unexpected token"),
		.loc = token->loc,
	});
}