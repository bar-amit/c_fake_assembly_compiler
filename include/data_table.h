typedef struct numeric_data {
    unsigned short values[24];
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

/**
 * Search data by label name.
 * 
 * @param label_name The label to seach for.
 * @param table The data tabel head.
 */
data_unit* find_data(char* label_name, data_table* table);

/**
 * Creates new data item of data_unit struct.
 * 
 * @param name Label name.
 * @param type Either NUMERIC or STRING type. 
 */
data_unit* create_data(char* name, int type);

/**
 * Prepends data to table.
 * 
 * @param table The data table.
 * @param data The data to prepend.
 */
void prepend_data(data_table* table, data_unit* data);

/**
 * Reverse data table. Done at the end of the pass to preserve the original order
 * of labels in the assembly code.
 * 
 * @param table The data table to reverse.
 */
data_table* reverse_data(data_table* table);
