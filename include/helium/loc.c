#pragma once
#include "int.c"
#include "str.c"

typedef struct Helium_Loc {
	StrSlice file_path;
	u64 line_num;
	u64 line_begin;
	u64 column;
	u64 pos;
	u64 len;
} Helium_Loc;