typedef struct numeric_data {
    short values[24];
    int length;
} numeric_data;

typedef struct data_unit {
    char* name;
    int type_code;
    int source_line;
    int data_count;
    char* string_data;
    struct numeric_data* num_data;
    struct data_unit* next;
} data_unit;

typedef struct data_table {
    data_unit* head;
    int data_count;
} data_table;

typedef enum TYPE_CODES {
    NUMERIC = 0,
    STRING
} TYPE_CODES;

data_unit* find_data(char* label_name, data_table* table);
data_unit* create_data(char* name, int type);
void prepend_data(data_table* table, data_unit* data);
data_table* reverse_data(data_table* file);
