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
    IMMEDIATE_ADDRESSING = 0,
    DIRECT_ADDRESSING,
    INDIRECT_REGISTER,
    DIRECT_REGISTER
} ADDRESSING_CODE;

typedef enum ENCODING_TYPE {
    NONE = 0,
    EXTERNAL,
    RELOCATABLE,
    ABSOLUTE
}

/* OFFSET OF BITS IN A WORD, RIGHT TO LEFT */
#define OPERATION_CODE_OFFSET 11
#define DESTINATION_OPERAND_CODE_OFFSET 3
#define SOURCE_OPERAND_CODE_OFFSET 7
#define SOURCE_REGISTER_CODE_OFFSET 6
#define DESTINATION_REGISTER_CODE_OFFSET 3

typedef enum ORPAND_POSITION {
    DESTINATION = 0,
    SOURCE
} ORPAND_POSITION;

int get_operation_code(char* operation);
void make_assembly(
    short* code_image, file_head* ob_file,
    data_table* data, entry_table* entries,
    file_head* errors);
short get_registers_encoding(char* source_operand, char* destination_operand);
short get_register_encoding(char* register, int position);
short get_register_number(char* register);
short get_label_encoding(operand);
short get_immediate_encoding(char* operand);
short get_operand_encoding(char* operand, int position, data_table* data, entry_table* entries);
short get_operand_code(char* operand, int operand_position);
short get_operation_code(char* operation, file_head* errors, int source_line);
int get_orpands_amount(int operation_code);
int is_address_method_allowed(int operation_code, int orpand_code, int address_method);
