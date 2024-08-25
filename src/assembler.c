#include <string.h>

#include "../include/file_line.h"
#include "../include/macro_routine.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/assembler.h"
#include "../include/handle_error.h"
#include "../include/parser.h"
#include "../include/util.h"

void make_assembly(
short* code_image, file_head* ob_file, data_table* data, entry_table* entries, file_head* errors)
{
    int word_counter = 0, data_start_address = ob_file->line_count - 1 + MEMORY_ADDRESS_START;
    file_line* current = ob_file->head;
    char* cursor, first_operand[MAX_LABEL_NAME_LENGTH], operation_name[4];
    while(current!=NULL){
        cursor = strtok(remove_last_space(current->content), " ");
        strcpy(operation_name, cursor);
        cursor = strtok(NULL, " ");
        if(cursor!=NULL)
            strcpy(first_operand, cursor);
        cursor = strtok(NULL, " ");
        code_image[word_counter++] = get_instraction_encoding(operation_name, first_operand, cursor, errors, current->line_number);
        if(are_registers(first_operand, cursor))
            code_image[word_counter++] = get_registers_encoding(first_operand, cursor);
        else if(first_operand!=NULL && cursor!=NULL){
            code_image[word_counter++] =
                get_operand_encoding(first_operand, SOURCE_OPERAND, data, entries, data_start_address);
            code_image[word_counter++] =
                get_operand_encoding(cursor, DESTINATION_OPERAND, data, entries, data_start_address);
        }
        else if(first_operand!=NULL){
            code_image[word_counter++] =
                get_operand_encoding(first_operand, DESTINATION_OPERAND, data, entries, data_start_address);
        }
        current = current->next;
    }
}

short get_instraction_encoding(
    char* operation, char* first_operand, char* second_operand, file_head* errors, int source_line)
{
    short word = get_operation_code(operation), source_address_method, destination_address_method;
    source_address_method = destination_address_method = NONE_ADDRESS_METHOD;
    if(first_operand!=NULL && second_operand!=NULL){
        source_address_method = get_operand_address_method(first_operand);
        destination_address_method = get_operand_address_method(second_operand);
        is_address_method_allowed(word, SOURCE_OPERAND, source_address_method, errors, source_line);
        is_address_method_allowed(word, DESTINATION_OPERAND, destination_address_method, errors, source_line);
    }
    else if(first_operand!=NULL && *first_operand!='\0'){
        destination_address_method = get_operand_address_method(first_operand);
        is_address_method_allowed(word, DESTINATION_OPERAND, destination_address_method, errors, source_line);
    }
    else
        is_address_method_allowed(word, NONE_OPERAND, NONE_ADDRESS_METHOD, errors, source_line);
    word = word << OPERATION_CODE_OFFSET;
    word += ABSOLUTE;
    if(destination_address_method!=NONE_ADDRESS_METHOD)
        word += destination_address_method << DESTINATION_OPERAND_CODE_OFFSET;
    if(source_address_method!=NONE_ADDRESS_METHOD)
        word += source_address_method << SOURCE_OPERAND_CODE_OFFSET;
    return word;
}

short get_registers_encoding(char* source_operand, char* destination_operand){
    short word = ABSOLUTE;
    word += get_register_number(source_operand) << SOURCE_REGISTER_CODE_OFFSET;
    word += get_register_number(destination_operand) << DESTINATION_REGISTER_CODE_OFFSET;
    return word;
}

short get_register_encoding(char* register_name, int position){
    short word = ABSOLUTE;
    word += get_register_number(register_name) << (position ?
        SOURCE_REGISTER_CODE_OFFSET : DESTINATION_REGISTER_CODE_OFFSET);
    return word;
}

short get_register_number(char* register_name){
    if(*register_name=='*')
        return (short)(*(register_name+2)-'0');
    return (short)(*(register_name+1)-'0');
}

short get_label_encoding(char* operand, data_table* data, entry_table* entries, int data_start_address){
    data_unit* data_label;
    data_label = find_data(operand, data);
    if(data_label!=NULL)
        return get_data_encoding(data_label, data_start_address);
    return get_entry_encoding(find_entry(operand ,entries));
}

short get_data_encoding(data_unit* data_label, int data_start_address){
    short word = RELOCATABLE;
    word +=
        (short)(data_start_address+data_label->data_count) << DESTINATION_OPERAND_CODE_OFFSET;
    return word;
}

short get_entry_encoding(entry_label* entry){
    short word = RELOCATABLE;
    if(entry==NULL)
        return 0;
    if (entry->type_code == EXTERN)
        return (short)EXTERNAL;
    word += (short)(entry->instraction_line + MEMORY_ADDRESS_START) >> DESTINATION_OPERAND_CODE_OFFSET;
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

short get_operand_encoding(
    char* operand, int position, data_table* data, entry_table* entries, int data_start_address)
{
    if(is_register(operand) || is_indirect_register(operand))
        return get_register_encoding(operand, position);
    if(is_label(operand))
        return get_label_encoding(operand, data, entries, data_start_address);
    return get_immediate_encoding(operand);
}

short get_operand_address_method(char* operand){
    if(is_register(operand))
        return DIRECT_REGISTER;
    else if(is_indirect_register(operand))
        return INDIRECT_REGISTER;
    else if(is_label(operand))
        return DIRECT_ADDRESSING;
    return IMMEDIATE_ADDRESSING;
}

short get_operation_code(char* operation){
    if(strcmp(operation, "mov")==0){
        return MOV;
    } else if(strcmp(operation, "cmp")==0){
        return CMP;
    } else if(strcmp(operation, "add")==0){
        return ADD;
    } else if(strcmp(operation, "sub")==0){
        return SUB;
    } else if(strcmp(operation, "lea")==0){
        return LEA;
    } else if(strcmp(operation, "clr")==0){
        return CLR;
    } else if(strcmp(operation, "not")==0){
        return NOT;
    } else if(strcmp(operation, "inc")==0){
        return INC;
    } else if(strcmp(operation, "dec")==0){
        return DEC;
    } else if(strcmp(operation, "jmp")==0){
        return JMP;
    } else if(strcmp(operation, "bne")==0){
        return BNE;
    } else if(strcmp(operation, "red")==0){
        return RED;
    } else if(strcmp(operation, "prn")==0){
        return PRN;
    } else if(strcmp(operation, "jsr")==0){
        return JSR;
    } else if(strcmp(operation, "rts")==0){
        return RTS;
    } else if(strcmp(operation, "stop")==0){
        return STOP;
    }
    return -1;
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

void is_address_method_allowed(
    int operation_code, int operand_position, int address_method, file_head* errors, int source_line)
{
    int operand_amount = get_orpands_amount(operation_code);
    if(operand_amount==0 && operand_position!=NONE_OPERAND)
        handle_message(errors, "too many operands (expected 0)", source_line);
    if(operand_position==SOURCE_OPERAND){
        if(operand_amount!=2)
            handle_message(errors, "too many operands (expected 1)", source_line);
        if(address_method!=DIRECT_ADDRESSING && operation_code==LEA)
            handle_message(errors, "illegal addressing method (expected direct address)", source_line);
    } else {
        if(address_method==DIRECT_REGISTER && (operation_code==JMP || operation_code==BNE || operation_code==JSR))
            handle_message(errors, "illegal addressing method (direct register)", source_line);
        if(address_method==IMMEDIATE_ADDRESSING && !(operation_code==CMP || operation_code==PRN))
            handle_message(errors, "illegal addressing method (immediate address)", source_line);
    }
}
