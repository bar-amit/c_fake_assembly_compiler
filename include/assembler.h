typedef enum OPERATION_CODES {
    MOV = 0,
    CMP,
    ADD,
    SUB,
    LEA,
    CLR,
    NOT,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP
} OPERATION_CODES;

typedef enum ADDRESSING_CODE {
    NONE_ADDRESS_METHOD = 0,
    IMMEDIATE_ADDRESSING = 1,
    DIRECT_ADDRESSING = 2,
    INDIRECT_REGISTER = 4,
    DIRECT_REGISTER = 8
} ADDRESSING_CODE;

typedef enum ENCODING_TYPE {
    EXTERNAL = 1,
    RELOCATABLE = 2,
    ABSOLUTE = 4
} ENCODING_TYPE;

typedef enum OPERAND_POSITION {
    NONE_OPERAND = -1,
    DESTINATION_OPERAND,
    SOURCE_OPERAND
} OPERAND_POSITION;

/* OFFSET OF BITS IN A WORD, RIGHT TO LEFT */
#define OPERATION_CODE_OFFSET 11
#define DESTINATION_OPERAND_CODE_OFFSET 3
#define SOURCE_OPERAND_CODE_OFFSET 7
#define SOURCE_REGISTER_CODE_OFFSET 6
#define DESTINATION_REGISTER_CODE_OFFSET 3

void make_assembly(short* code_image,
    file_head* ob_file, data_table* data,
    entry_table* entries, file_head* errors);
short get_instraction_encoding(char* operation,
    char* first_operand, char* second_operand,
    file_head* errors, int source_line);
short get_registers_encoding(char* source_operand,
    char* destination_operand);
short get_register_encoding(char* register_name, int position);
short get_register_number(char* register_name);
short get_label_encoding(char* operand,
    data_table* data, entry_table* entries,
    int data_start_address);
short get_data_encoding(data_unit* data_label,
    int data_start_address);
short get_entry_encoding(entry_label* entry);
short get_immediate_encoding(char* operand);
short get_operand_encoding(char* operand,
    int position, data_table* data,
    entry_table* entries, int data_start_address);
short get_operand_address_method(char* operand);
short get_operation_code(char* operation);
int get_orpands_amount(int operation_code);
void is_address_method_allowed(int operation_code,
    int operand_position, int address_method,
    file_head* errors, int source_line);
