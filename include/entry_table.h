typedef struct entry_label {
    int is_declared;
    int source_line;
    int instraction_line;
    int* used_external;
    int external_count;
    char* name;
    int type_code;
    struct entry_label* next; 
} entry_label;

typedef struct entry_table {
    entry_label* head;
} entry_table;

typedef enum ENTRY_CODES {
    ENTRY = 0,
    EXTERN,
    DATA
} ENTRY_CODES;

/**
 * Prepends an entry to an entry table.
 * 
 * @param table The table to prepend to.
 * @param entry An entry item to prepend to table.
 */
void prepend_entry(entry_table* table, entry_label* entry);

/**
 * Searches an entry by label name.
 * 
 * @param label_name A label name to search for.
 * @param table The table to search in. 
 */
entry_label* find_entry(char* label_name, entry_table* table);

/**
 * Creates a new entry item out of struct entry_label
 * 
 * @param name Label name.
 * @param type Either ENTRY or EXTERN or DATA.
 * @param source_line Number of line in source file that created this label.
 * @param instraction_line Line number of instraction which the label points to (if exists).
 * @param is_declared Is declared by ".entry" instraction.
 */
entry_label* create_entry(char* name, int type, int source_line, int instraction_line, int is_declared);

/**
 * Adds a line number where an external label is used to entry table.
 * 
 * @param entry Entry being used.
 * @param line_number Line number where entry is used. 
 */
void add_external_use(entry_label* entry, int line_number);
