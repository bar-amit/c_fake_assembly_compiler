#include "../include/preserved_words.h"

const char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8"};
const char *instractions[] = {".data", ".string", ".entry", ".extern"};
const char *operations[] = 
    {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "red",
        "prn",
        "jsr",
        "rts",
        "stop"
    };
