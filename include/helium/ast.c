#pragma once
#include "int.c"
#include "helium/loc.c"

typedef union Helium_Ast_NodeExpr Helium_Ast_NodeExpr;
typedef union Helium_Ast_NodeStmt Helium_Ast_NodeStmt;
enum : i32 {
	helium_ast_nodetype_expr_num,
	helium_ast_nodetype_expr_ident,
	helium_ast_nodetype_expr_str,
	helium_ast_nodetype_expr_boolean,
	helium_ast_nodetype_expr_operator,
	helium_ast_nodetype_expr_tuple,
	helium_ast_nodetype_expr_call,
	helium_ast_nodetype_expr_init,
	helium_ast_nodetype_expr_operation,
	helium_ast_nodetype_expr_index,
	helium_ast_nodetype_expr_member,
	helium_ast_nodetype_expr_array,
	helium_ast_nodetype_expr_qualifiers,
	helium_ast_nodetype_stmt_expr,
};
char const *helium_str_ast_nodetype(i32 type) {
	switch (type) {
		case helium_ast_nodetype_expr_num: return "expr_num";
		case helium_ast_nodetype_expr_ident: return "expr_ident";
		case helium_ast_nodetype_expr_str: return "expr_str";
		case helium_ast_nodetype_expr_boolean: return "expr_boolean";
		case helium_ast_nodetype_expr_operator: return "expr_operator";
		case helium_ast_nodetype_expr_tuple: return "expr_tuple";
		case helium_ast_nodetype_expr_call: return "expr_call";
		case helium_ast_nodetype_expr_init: return "expr_init";
		case helium_ast_nodetype_expr_operation: return "expr_operation";
		case helium_ast_nodetype_expr_index: return "expr_index";
		case helium_ast_nodetype_expr_member: return "expr_member";
		case helium_ast_nodetype_expr_array: return "expr_array";
		case helium_ast_nodetype_expr_qualifiers: return "expr_qualifiers";
		case helium_ast_nodetype_stmt_expr: return "stmt_expr";
	}
	return "(invalid-type)";
}

typedef union Helium_Ast_NumVal Helium_Ast_NumVal;
union Helium_Ast_NumVal {
	double floating;
	u64 integral;
};

typedef struct Helium_Ast_NodeExprNum Helium_Ast_NodeExprNum;
struct Helium_Ast_NodeExprNum {
	i32 type;
	Helium_Loc loc;
	bool is_float;
	Helium_Ast_NumVal as;
};

typedef struct Helium_Ast_NodeExprIdent Helium_Ast_NodeExprIdent;
struct Helium_Ast_NodeExprIdent {
	i32 type;
	Helium_Loc loc;
	StrSlice value;
};

typedef struct Helium_Ast_NodeExprStr Helium_Ast_NodeExprStr;
struct Helium_Ast_NodeExprStr {
	i32 type;
	Helium_Loc loc;
	Str value;
};

typedef struct Helium_Ast_NodeExprBool Helium_Ast_NodeExprBool;
struct Helium_Ast_NodeExprBool {
	i32 type;
	Helium_Loc loc;
	bool value;
};

typedef struct Helium_Ast_NodeExprOperator Helium_Ast_NodeExprOperator;
struct Helium_Ast_NodeExprOperator {
	i32 type;
	Helium_Loc loc;
	i32 operator_kind;
};

typedef struct Helium_Ast_NodeExprTuple Helium_Ast_NodeExprTuple;
struct Helium_Ast_NodeExprTuple {
	i32 type;
	u64 len;
	Helium_Ast_NodeExpr *values;
};

typedef struct Helium_Ast_Node_CallArg Helium_Ast_Node_CallArg;
struct Helium_Ast_Node_CallArg {
	Helium_Ast_NodeExprIdent *arg_name;
	Helium_Ast_NodeExpr *value;
};

typedef struct Helium_Ast_Node_CallArgs Helium_Ast_Node_CallArgs;
struct Helium_Ast_Node_CallArgs {
	Helium_Ast_NodeExpr *args;
	u64 len;
};

typedef struct Helium_Ast_NodeExprCall Helium_Ast_NodeExprCall;
struct Helium_Ast_NodeExprCall {
	i32 type;
	Helium_Ast_NodeExpr *calle;
	Helium_Ast_Node_CallArgs *args;
};

typedef struct Helium_Ast_NodeExprInit Helium_Ast_NodeExprInit;
struct Helium_Ast_NodeExprInit {
	i32 type;
	Helium_Ast_NodeExpr *init_type;
	Helium_Ast_Node_CallArgs *init_args;
};

typedef struct Helium_Ast_NodeExprOperation Helium_Ast_NodeExprOperation;
struct Helium_Ast_NodeExprOperation {
	i32 type;
	Helium_Ast_NodeExprOperator operator;
	Helium_Ast_NodeExpr *operands;
	u64 operands_len;
};

typedef struct Helium_Ast_NodeExprIndex Helium_Ast_NodeExprIndex;
struct Helium_Ast_NodeExprIndex {
	i32 type;
	Helium_Ast_NodeExpr *value;
	Helium_Ast_NodeExpr *indexer;
};

typedef struct Helium_Ast_NodeExprMember Helium_Ast_NodeExprMember;
struct Helium_Ast_NodeExprMember {
	i32 type;
	Helium_Ast_NodeExpr *value;
	Helium_Ast_NodeExprIdent *indexer;
};

typedef struct Helium_Ast_NodeExprArray Helium_Ast_NodeExprArray;
struct Helium_Ast_NodeExprArray {
	i32 type;
	Helium_Ast_NodeExpr *inner_type;
	Helium_Ast_NodeExpr *size;
};

typedef i32 Helium_Ast_Qualifiers;
enum : Helium_Ast_Qualifiers {
	helium_ast_qualifier_const,
	helium_ast_qualifier_comptime,
};

typedef struct Helium_Ast_NodeExprQualifiers Helium_Ast_NodeExprQualifiers;
struct Helium_Ast_NodeExprQualifiers {
	i32 type;
	Helium_Ast_Qualifiers qualifiers;
	Helium_Ast_NodeExpr *value;
};

union Helium_Ast_NodeExpr {
	i32 type;
	Helium_Ast_NodeExprNum num;
	Helium_Ast_NodeExprIdent ident;
	Helium_Ast_NodeExprStr str;
	Helium_Ast_NodeExprBool boolean;
	Helium_Ast_NodeExprOperator operator;
	Helium_Ast_NodeExprTuple tuple;
	Helium_Ast_NodeExprCall call;
	Helium_Ast_NodeExprInit init;
	Helium_Ast_NodeExprOperation operation;
	Helium_Ast_NodeExprIndex index;
	Helium_Ast_NodeExprMember member;
	Helium_Ast_NodeExprArray array;
	Helium_Ast_NodeExprQualifiers qualifiers;
};

typedef struct Helium_Ast_NodeStmtExpr Helium_Ast_NodeStmtExpr;

union Helium_Ast_NodeStmt {
	i32 type;
	Helium_Ast_NodeExpr expr;
};