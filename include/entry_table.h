typedef struct entry_label {
    int is_declared;
    int source_line;
    int instraction_line;
    char* name;
    int type_code;
    struct entry_label* next; 
} entry_label;

typedef struct entry_table {
    entry_label* head;
} entry_table;

typedef enum ENTRY_CODES {
    ENTRY = 0,
    EXTERN
} ENTRY_CODES;

void prepend_entry(entry_table* table, entry_label* entry);
entry_label* find_entry(char* label_name, entry_table* table);
entry_label* create_entry(char* name, int type, int source_line, int instraction_line, int is_declared);
