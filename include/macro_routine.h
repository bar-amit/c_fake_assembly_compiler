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

/**
 * Finds a macro in macro table by name.
 * 
 * @param list List of macros.
 * @param macro_name Macro name to find.
 */
macro_routine* find_macro(macro_list* list, char* macro_name);

/**
 * Prepends a macro to list.
 * 
 * @param list Macro list to prepend to.
 * @param name A macro name to prepend.
 * @param start_line The line on which the macro starts.
 * @param end_line The line on which the macro ends.
 */
void prepend_macro(macro_list* list, char* name, int start_line, int end_line);

/**
 * Creates a new macro item of struct macro_routine.
 * 
 * @param name Name of macro.
 * @param start_line Macro start line.
 * @param end_line Macro ending line.
*/
macro_routine* get_macro(char* name, int start_line, int end_line);
