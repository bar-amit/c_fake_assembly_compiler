typedef struct numeric_data {
    int values[24];
    int lenght;
} numeric_data;

typedef struct label {
    char* name;
    int type_code;
    char* string_data;
    struct numeric_data* num_data;
    struct label* next;
} label;

typedef struct label_table {
    label* head;
    int data_count;
} label_table;

typedef enum TYPE_CODES {
    DATA = 0,
    STRING,
    ENTRY,
    EXTERN
} TYPE_CODES;

label* find_label(char* label_name, label_table* table);
