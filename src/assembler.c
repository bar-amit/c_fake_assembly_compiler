#include <string.h>

#include "../include/assembler.h"
#include "../include/file_line.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/handle_error.h"
#include "../include/parser.h"

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
        if(are_registers(source_operand, cursor))
            code_image[word_counter++] = get_registers_encoding(source_operand, cursor);
        else {    
            code_image[word_counter++] = get_operand_encoding(source_operand, SOURCE,data, entries);
            code_image[word_counter++] = get_operand_encoding(cursor, DESTINATION, data, entries);
        }
        current = current->next;
    }
}

short get_registers_encoding(char* source_operand, char* destination_operand){
    short word = ABSOLUTE;
    word += get_register_number(source_operand) << SOURCE_REGISTER_CODE_OFFSET;
    word += get_register_number(destination_operand) << DESTINATION_REGISTER_CODE_OFFSET;
    return word;
}

short get_register_encoding(char* register, int position){
    short word = ABSOLUTE;
    word += get_register_number(register) << position ?
        SOURCE_REGISTER_CODE_OFFSET : DESTINATION_REGISTER_CODE_OFFSET;
    return word;
}

short get_register_number(char* register){
    if(*register=='*')
        return (short)(*(register+2)-'0');
    return (short)(*(register+1)-'0')
}

short get_label_encoding(operand){
    short word = RELOCATABLE;
    //todo
    return word;
}

short get_immediate_encoding(char* operand){
    short word = ABSOLUTE;
    if(*(operand+1)=='-')
        return word+= (short)(-(*(operand+2) - '0')) << DESTINATION_OPERAND_CODE_OFFSET;
    else if(*(operand+1)=='+')
        return word+= (short)(*(operand+2) - '0') << DESTINATION_OPERAND_CODE_OFFSET;
    return word+= (short)(*(operand+1) - '0') << DESTINATION_OPERAND_CODE_OFFSET;
}

short get_operand_encoding(char* operand, int position, data_table* data, entry_table* entries){
    short word;
    if(is_label(operand))
        return get_label_encoding(operand, position, data, entries);
    if(is_register(operand) || is_inddirect_register(operand))
        return get_register_encoding(operand, position);
    return get_immediate_encoding(operand, position);
}

short get_operand_code(char* operand, int operand_position){
    short operand_code = 1;
    if(is_register(operand))
        operand_code << DIRECT_REGISTER;
    else if(is_indirect_register(operand))
        operand_code << INDIRECT_REGISTER;
    else if(is_label(operand))
        operand_code << DIRECT_ADDRESSING;
    return operand_code << operand_position ?
        SOURCE_OPERAND_CODE_OFFSET : DESTINATION_OPERAND_CODE_OFFSET;
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
