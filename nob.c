// Thank you amista azozin
#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);
    size_t index = 1;
    size_t remaining = argc - index;

    if (remaining > 0) do {
        char *arg = argv[remaining--, index++];
        if (strcmp(arg, "build") == 0) {
            Cmd cmd = {0};
            cmd_append(
                &cmd,
                "clang",
                "-std=c23",
                "-w",
                "-lm",
                "-fsanitize=address",
                "-o",
                "./bin/helium",
                "-Isrc",
                "./tests/pratt.c"
            );
            
            if (!cmd_run(&cmd)) return 1;
        } else if (strcmp(arg, "run") == 0) {
            Cmd cmd = {0};
            cmd_append(&cmd, "./bin/helium");
            
            if (!cmd_run(&cmd)) return 1;
            printf("\n");
        }
    } while (remaining-- && strcmp(argv[index++], "+") == 0);
}