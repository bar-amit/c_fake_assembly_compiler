#include "./constants.h"

typedef struct macro_routine {
    char *name;
    int start_line;
    int end_line;
    int is_used;
    struct macro_routine *next;
} macro_routine;

typedef struct macro_list {
    struct macro_routine* head;
} macro_list;

macro_routine* find_macro(macro_list* list, char* macro_name);
void prepend_macro(macro_list* list, char* name, int start_line, int end_line);
macro_routine* get_macro(char* name, int start_line, int end_line);
