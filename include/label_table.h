typedef struct label {
    int data_count;
    char* name;
    int type_code;
} label;

typedef struct label_table {
    label* head;
    int data_count;
} label_table;

typedef enum TYPE_CODES {
    DATA = 0,
    STRING,
}
