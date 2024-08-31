#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../include/constants.h"
#include "../include/file_line.h"
#include "../include/macro_routine.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/make_ob_file.h"
#include "../include/stream.h"
#include "../include/handle_error.h"
#include "../include/parser.h"
#include "../include/util.h"

void write_ob_file(unsigned short* code_image, data_table* data, int instraction_count, char* file_name){
    FILE* file_pointer = NULL;
    data_unit* current_data = data->head;
    int code_index = -1, data_index = 0, data_counter = data->data_address_start;
    file_pointer = write_steam(file_pointer, file_name);
    fprintf(file_pointer, "%4d %d\n", instraction_count, data->data_count);
    while (code_image[++code_index]!=0)
        {
            fprintf(file_pointer, "%04d %05o\n", code_index+MEMORY_ADDRESS_START, code_image[code_index]);
        }
    while (current_data!=NULL){
        switch (current_data->type_code)
        {
        case STRING:
            while(*(current_data->string_data+data_index)!='\0')
                fprintf(file_pointer, "%04d %05o\n", data_counter++, (unsigned short)*(current_data->string_data+(data_index++)));
            fprintf(file_pointer, "%04d %05o\n", data_counter++, (unsigned short)0);
            break;
        case NUMERIC:
            while(data_index < current_data->num_data->length)
                fprintf(file_pointer, "%04d %05o\n", data_counter++, current_data->num_data->values[data_index++]);
            break;
        }
        data_index = 0;
        current_data = current_data->next;
    }
}

void make_assembly(
unsigned short* code_image, file_head* ob_file, data_table* data, entry_table* entries, file_head* errors)
{
    int word_counter = 0, current_address;
    file_line* current = ob_file->head;
    char* cursor, first_operand[MAX_LABEL_NAME_LENGTH], operation_name[4];
    while(current!=NULL){
        current_address = MEMORY_ADDRESS_START + word_counter;
        cursor = strtok(remove_last_space(current->content), " ");
        strcpy(operation_name, cursor);
        cursor = strtok(NULL, " ");
        if(cursor!=NULL)
            strcpy(first_operand, cursor);
        else
            first_operand[0] = '\0';
        cursor = strtok(NULL, " ");
        code_image[word_counter++] = get_instraction_encoding(operation_name, first_operand, cursor, errors, current->line_number);
        if(are_registers(first_operand, cursor))
            code_image[word_counter++] = get_registers_encoding(first_operand, cursor);
        else if(first_operand!=NULL && cursor!=NULL){
            code_image[word_counter++] =
                get_operand_encoding(first_operand, SOURCE_OPERAND, 2, data, entries, current_address, errors, current->line_number);
            code_image[word_counter++] =
                get_operand_encoding(cursor, DESTINATION_OPERAND, 2, data, entries, current_address, errors, current->line_number);
        }
        else if(*first_operand!='\0'){
            code_image[word_counter++] =
                get_operand_encoding(first_operand, DESTINATION_OPERAND, 1, data, entries, current_address, errors, current->line_number);
        }
        current = current->next;
    }
}

unsigned short get_instraction_encoding(
    char* operation, char* first_operand, char* second_operand, file_head* errors, int source_line)
{
    unsigned short word = get_operation_code(operation), source_address_method, destination_address_method;
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

unsigned short get_registers_encoding(char* source_operand, char* destination_operand){
    unsigned short word = ABSOLUTE;
    word += get_register_number(source_operand) << SOURCE_REGISTER_CODE_OFFSET;
    word += get_register_number(destination_operand) << DESTINATION_REGISTER_CODE_OFFSET;
    return word;
}

unsigned short get_register_encoding(char* register_name, int position){
    unsigned short word = ABSOLUTE;
    word += get_register_number(register_name) << (position ?
        SOURCE_REGISTER_CODE_OFFSET : DESTINATION_REGISTER_CODE_OFFSET);
    return word;
}

unsigned short get_register_number(char* register_name){
    if(*register_name=='*')
        return (unsigned short)(*(register_name+2)-'0');
    return (unsigned short)(*(register_name+1)-'0');
}

unsigned short get_label_encoding(char* operand, int position, int operand_amount, data_table* data,
    entry_table* entries, int instraction_line, file_head* errors, int source_line)
{
    data_unit* data_label;
    entry_label* entry;
    data_label = find_data(operand, data);
    if(data_label!=NULL)
        return get_data_encoding(data_label, data->data_address_start);
    entry = find_entry(operand, entries);
    if(entry!=NULL)
        return get_entry_encoding(entry, position, operand_amount, instraction_line);
    handle_message(errors, "label is used but not defined", source_line);
    return 0;
}

unsigned short get_data_encoding(data_unit* data_label, int data_start_address){
    unsigned short word = RELOCATABLE;
    word +=
        (unsigned short)(data_start_address+data_label->data_count) << DESTINATION_OPERAND_CODE_OFFSET;
    return word;
}

unsigned short get_entry_encoding(entry_label* entry, int position, int operand_amount, int instraction_line){
    unsigned short word = RELOCATABLE;
    if (entry->type_code == EXTERN){
        add_external_use(entry, position, operand_amount, instraction_line);
        return (unsigned short)EXTERNAL;
    }
    word += (unsigned short)entry->instraction_line << DESTINATION_OPERAND_CODE_OFFSET;
    return word;
}

unsigned short get_immediate_encoding(char* operand){
    unsigned short word = ABSOLUTE;
    if(*(operand+1)=='-')
        return word+= ((unsigned short)(-atoi(operand+2)) << DESTINATION_OPERAND_CODE_OFFSET) ^ BIT_MASK;
    else if(*(operand+1)=='+')
        return word+= ((unsigned short)atoi(operand+2)) << DESTINATION_OPERAND_CODE_OFFSET;
    return word+= ((unsigned short)atoi(operand+1)) << DESTINATION_OPERAND_CODE_OFFSET;
}

unsigned short get_operand_encoding(char* operand, int position, int operand_amount,
    data_table* data, entry_table* entries, int instraction_line, file_head* errors,
    int source_line)
{
    if(is_register(operand) || is_indirect_register(operand))
        return get_register_encoding(operand, position);
    if(is_label(operand))
        return get_label_encoding(operand, position, operand_amount, data, entries, instraction_line, errors, source_line);
    return get_immediate_encoding(operand);
}

unsigned short get_operand_address_method(char* operand){
    if(is_register(operand))
        return DIRECT_REGISTER;
    else if(is_indirect_register(operand))
        return INDIRECT_REGISTER;
    else if(is_label(operand))
        return DIRECT_ADDRESSING;
    else if(is_immediate(operand))
        return IMMEDIATE_ADDRESSING;
    return NONE_ADDRESS_METHOD;
}

unsigned short get_operation_code(char* operation){
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

int get_operand_amount(int operation_code){
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
    int operand_amount = get_operand_amount(operation_code);
    if(operand_amount==0 && operand_position!=NONE_OPERAND)
        handle_message(errors, "too many operands (expected 0)", source_line);
    if(address_method==NONE_ADDRESS_METHOD && operand_position!=NONE_OPERAND)
        handle_message(errors, "bad operand (can't find address method)", source_line);
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
