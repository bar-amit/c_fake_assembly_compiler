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
#define OPERAND_CODE_OFFSET 3
#define OPERAND_CODE_LENGTH 4

typedef enum ORPAND_POSITION {
    DESTINATION = 0,
    SOURCE
} ORPAND_POSITION;

int get_operation_code(char* operation);