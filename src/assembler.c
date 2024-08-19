#include <string.h>

#include "../include/assembler.h"
#include "../include/file_line.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/handle_error.h"

void make_assembly(
short* code_image, file_head* ob_file, data_table* data, entry_table* entries, file_head* errors)
{
    int word_counter = 0;
    short word_code;
    file_line* current = ob_file->head;
    char* cursor, source_operand[MAX_LABEL_NAME_LENGTH];
    while(current!=NULL){
        word_code = ABSOLUTE;
        cursor = strtok(current->content, " ");
        word_code += get_operation_code(cursor, errors, current->line_number);
        cursor = strtok(NULL, " ");
        if(cursor==NULL){
            code_image[word_counter++] = word_code;
            continue;
        }
        word_code += get_operand_code(cursor, SOURCE);
        strcpy(source_operand, cursor);
        cursor = strtok(NULL, " ");
        if(cursor==NULL){
            code_image[word_counter++] = word_code;
            code_image[word_counter++] = get_operand_encoding(source_operand, data, entries);
            continue;
        }
        word_code += get_operand_code(cursor, DESTINATION);
        code_image[word_counter++] = word_code;
        code_image[word_counter++] = get_operand_encoding(source_operand, data, entries);
        code_image[word_counter++] = get_operand_encoding(cursor, data, entries);
        current = current->next;
    }
}

short get_operand_encoding(char* source_operand, data_table* data, entry_table* entries){
    short word;
    if(is_label(source_operand))
        return get_label_encoding(source_operand, data, entries);
    if(is_register(source_operand))
        word = DIRECT_REGISTER;
    if()
}

short get_operand_code(char* operand, int operand_position){
    short operand_code;
    if(is_register()){
        operand_code = DIRECT_REGISTER;
    } else if(is_indirect_register()){
        operand_code = INDIRECT_REGISTER;
    } else if(is_label()){
        operand_code = DIRECT_ADDRESSING;
    } else {
        operand_code = IMMEDIATE_ADDRESSING;
    }
    return operand_code << OPERAND_CODE_OFFSET << operand_position * OPERAND_CODE_LENGTH;
}

short get_operation_code(char* operation, file_head* errors, int source_line){
    if(strcmp(operation, "mov")){
        return MOV << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "cmp")){
        return CMP << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "add")){
        return ADD << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "sub")){
        return SUB << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "lea")){
        return LEA << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "clr")){
        return CLR << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "not")){
        return NOT << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "inc")){
        return INC << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "dec")){
        return DEC << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "jmp")){
        return JMP << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "bne")){
        return BNE << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "red")){
        return RED << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "prn")){
        return PRN << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "jsr")){
        return JSR << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "rts")){
        return RTS << OPERATION_CODE_OFFSET;
    } else if(strcmp(operation, "stop")){
        return STOP << OPERATION_CODE_OFFSET;
    }
    handle_message(errors, "bad operation name", source_line)
    return 0;
}

int get_orpands_amount(int operation_code){
    switch(operation_code){
    case MOV:
    case CMP:
    case ADD:
    case SUB:
    case LEA:
        return 2;
        break;
    case CLR:
    case NOT:
    case INC:
    case DEC:
    case JMP:
    case BNE:
    case RED:
    case PRN:
    case JSR:
        return 1;
        break;
    }
    return 0;
}

int is_address_method_allowed(int operation_code, int orpand_code, int address_method){
    int orpand_amount = get_orpands_amount(operation_code);
    if((orpand_code==SOURCE && orpand_amount!=2) || orpand_amount==0){
        return 0;
    }
    if(orpand_code==SOURCE){
        if(address_method!=1 && operation_code==LEA)
            return 0;
    } else{
        if(address_method==3 && (operation_code==JMP || operation_code==BNE || operation_code==JSR))
            return 0;
        if(address_method==0 && !(operation_code==CMP || operation_code==PRN))
            return 0;
    }
    return 1;
}
