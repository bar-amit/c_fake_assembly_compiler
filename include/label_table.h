typedef struct numeric_data {
    int values[24];
    int lenght = 0;
} numeric_data;

typedef struct label {
    char* name;
    int type_code;
    char* string_data;
    numeric_data* num_data:
    label* next;
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
}
