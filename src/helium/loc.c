#ifndef _INCLUDE_HELIUM_LOC_C
#define _INCLUDE_HELIUM_LOC_C

#include <int.c>
#include <str.c>
#include <io.c>

struct helium_loc {
    struct str_slice file_path;
    size_t line_num;
    size_t line_begin;
    size_t column;
    size_t pos;
    size_t len;
};

struct helium_lexer_loc {
    struct str_slice file_path;
    size_t line_num;
    size_t line_begin;
    size_t column;
    size_t pos;
};

void helium_loc_pretty_print(
    FILE *out,
    struct str_slice source,
    struct helium_loc loc,
    struct str_slice *message,
    size_t display_post_loc,
    size_t padding,
    bool colors
) {
    const char *ANSI_RESET = (colors) ? "\e[0m"  : "";
    const char *ANSI_BOLD  = (colors) ? "\e[1m"  : "";
    const char *ANSI_FAINT = (colors) ? "\e[2m"  : "";
    const char *ANSI_RED   = (colors) ? "\e[31m" : "";
    
    fprintf(out, "% *s | \n", padding, "");
    {
        size_t line_len = 0;
        for (size_t i = loc.line_begin; i < source.len && source.buf[i] != '\n'; i++)
            line_len++;
        
        fprintf(out, "%-*i | %.*s\n", padding, loc.line_num + 1, line_len, source.buf + loc.line_begin);
    }
    if (message != NULL) {
        fprintf(out, "% *s | ", padding, "");
        for (size_t i = loc.line_begin; i < loc.pos; i++) {
            if (source.buf[i] == '\t') {
                putc('\t', out);
            } else {
                putc(' ', out);
            }
        }
        
        if (display_post_loc != 0 && display_post_loc < loc.len) for (size_t i = 0; i <= (loc.len - display_post_loc); i++) {
            fprintf(out, " ");
        }
        
        fprintf(out, "%.*s\n", message->len, message->buf);
    }
    fprintf(out, "% *s | \n", padding, "");
    fprintf(out, "% *sin: %s%.*s:%llu:%llu%s\n",
        padding,
        "",
        ANSI_BOLD,
        loc.file_path.len, loc.file_path.buf,
        loc.line_num + 1,
        loc.column + 1,
        ANSI_RESET
    );
}

#endif // _INCLUDE_HELIUM_LOC_C