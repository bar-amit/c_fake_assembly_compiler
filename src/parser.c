#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include <stdio.h>

#include "../include/macro_routine.h"
#include "../include/file_line.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/parser.h"
#include "../include/constants.h"
#include "../include/allocate_memory.h"
#include "../include/util.h"
#include "../include/handle_error.h"

#define MAX_LABEL_NAME_LENGTH 31

char *registers[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8"};
char *instractions[] = {".data", ".string", ".entry", ".extern"};
char *operations[] = 
    {
        "mov",
        "cmp",
        "add",
        "sub",
        "lea",
        "clr",
        "not",
        "inc",
        "dec",
        "jmp",
        "bne",
        "red",
        "prn",
        "jsr",
        "rts",
        "stop"
    };

void parse_line(
    file_head* ob_output, data_table* data, entry_table* entries,
    file_line* line, file_head* errors, file_head* warnings)
{
    char *temp = allocate_memory(strlen(line->content) + 1);
    char *word, *label_name;
    strcpy(temp, line->content);
    word = get_next_token(temp, ":");
    if(is_label(word)){
        label_name = allocate_memory(strlen(word) + 1);
        strcpy(label_name, word);
        word = get_next_token(NULL, ": ");
        if(is_data(word)){
            prepend_data(data, parse_data(label_name, word, get_next_token(NULL, ""), line->line_number, errors));
        } else if(is_operation(word)){
            prepend_line(ob_output, parse_operation(word, get_next_token(NULL, ""), line->line_number, errors));
            parse_entry(entries, label_name, ENTRY, line->line_number, ob_output->line_count, errors);
        } else if(is_entry(word)){
            handle_message(warnings, "label for a declaration line", line->line_number);
            parse_entry(entries, get_next_token(NULL, ""), get_entry_code(word), line->line_number, -1, errors);
        } else {
            handle_message(errors, "label of an empty line", line->line_number);
        }
    } else {
        word = get_next_token(word, " ");
        if(is_operation(word)){
            prepend_line(ob_output, parse_operation(word, get_next_token(NULL, ""), line->line_number, errors));
        } else if(is_data(word)){
            prepend_data(data, parse_data(NULL, word, get_next_token(NULL, ""), line->line_number, errors));
            handle_message(warnings, "data instraction without a label", line->line_number);
        } else if(is_entry(word)){
            parse_entry(entries, get_next_token(NULL, ""), get_entry_code(word), line->line_number, -1, errors);
        } else {
            handle_message(errors, "can't parse code", line->line_number);
        }
    }
}

file_head* parse_source(file_head* am_file, file_head* ob_output, data_table* data, entry_table* entries,
    file_head* errors, file_head* warnings)
{
    file_line* current_input = am_file->head;
    while(current_input != NULL){
        parse_line(ob_output, data, entries, current_input, errors, warnings);
        current_input = current_input->next;
    }
    reverse_data(data);
    return reverse_file(ob_output);
}

file_line* parse_operation(char* operation, char* orpands, int source_line, file_head* errors){
    char* temp = allocate_memory(MAX_LINE_LENGTH), *str;
    strcpy(temp, operation);
    str = strtok(orpands, ",");
    if(str!=NULL){
        strcat(temp, " ");
        strcat(temp, str);
    }
    str = strtok(NULL, ",");
    if(str!=NULL){
        strcat(temp, " ");
        strcat(temp, str);
    }
    if(strtok(NULL, ",")!=NULL)
        handle_message(errors, "too many orpands", source_line);
    return create_line(temp, source_line);
}

data_unit* parse_data(char* label_name, char* instraction, char* orpand, int source_line, file_head* errors){
    int type_code = get_data_code(instraction);
    data_unit* new_data = create_data(label_name, type_code);
    switch (type_code){
    case STRING:
        if(!validate_string_data(orpand)){
            handle_message(errors, "bad string data", source_line);
            return NULL;
        }
        new_data->string_data = parse_string_data(orpand);
        break;
    case NUMERIC:
        if(!validate_numeric_data(orpand)){
            handle_message(errors, "bad numeric data", source_line);
            return NULL;
        }
        new_data->num_data = parse_numeric_data(orpand);
        break;
    }
    return new_data;
}

int get_data_code(char *instraction){
    return strcmp(instraction, ".string")==0 ?
        STRING : NUMERIC;
}

void parse_entry(
entry_table* entries, char* name, int type_code, int source_line, int instraction_line, file_head* errors)
{
    entry_label* entry = find_entry(name, entries);
    if(entry==NULL){
        entry = create_entry(name, type_code, source_line, instraction_line, instraction_line == -1);
        prepend_entry(entries, entry);
        return;
    }
    else if((entry->instraction_line>=0 && instraction_line>=0) || (instraction_line<0 && entry->is_declared)){
        handle_message(errors, "redefenition/redeclaration of label", source_line);
    }
    if(instraction_line>=0)
        entry->instraction_line = instraction_line;
    else
        entry->is_declared = 1;
    return;
}

int get_entry_code(char *instraction){
    return strcmp(instraction, ".entry")==0 ?
        ENTRY : EXTERN;
}

int is_preserved(char *name, macro_list *macros, data_table* labels){
    return is_register(name)
        || is_operation(name)
        || is_instraction(name)
        || find_macro(macros, name)!=NULL
        || find_data(name, labels)!=NULL;
}

int is_in_list(char *name, char *list[]){
    int i = -1;
    while(list[++i] != NULL)
        if(strcmp(name, list[i])==0) return 1;
    return 0;
}

int is_operation(char* str){
    if(str==NULL)
        return 0;
    return is_in_list(str, operations);
}

int is_instraction(char* str){
    if(str==NULL)
        return 0;
    return is_in_list(str, instractions);
}

int is_register(char* str){
    if(str==NULL)
        return 0;
    return is_in_list(str, registers);
}

int is_indirect_register(char* str){
    if(str==NULL)
        return 0;
    return *str=='*' && is_register(str+1);
}

int are_registers(char* source_operand, char* destination_operand){
    return (source_operand!=NULL && destination_operand!=NULL &&
        (is_register(source_operand) ||
        is_indirect_register(source_operand)) &&
        (is_register(destination_operand) ||
        is_indirect_register(destination_operand))
    );
}

int is_label(char* str){
    if(str==NULL)
        return 0;
    if(isalpha(*str) && is_alnum(str))
        return 1;
    return 0;
}

int is_alnum(char *str){
    while(*(++str) != '\0'){
        if(isalnum(*str)==0)
            return 0;
    }
    return 1;
}

int is_entry(char* str){
    if(str==NULL)
        return 0;
    if(strcmp(str, ".entry")==0 || strcmp(str, ".extern")==0)
        return 1;
    return 0;
}

int is_data(char* str){
    if(str==NULL)
        return 0;
    return strcmp(str, ".string")==0 || strcmp(str, ".data")==0;
}

int validate_string_data(char* str){
    char *data_start, *data_end;
    data_start = strchr(str, '\"');
    data_end = strrchr(str, '\"');
    if(data_start==NULL || data_start==data_end || *(data_end+1)!='\0')
        return 0;
    return 1;
}

char* parse_string_data(char* str){
    char *data = allocate_memory(sizeof(char)*MAX_LINE_LENGTH);
    char *data_start, *data_end;
    data_start = strchr(str, '\"');
    data_end = strrchr(str, '\"');
    strncpy(data, data_start + 1, (int)(data_end - data_start));
    return data;
}

numeric_data* parse_numeric_data(char *str){
    numeric_data *data = allocate_memory(sizeof(numeric_data));
    char* temp = strtok(str, ",");
    data->length = 0;
    while(temp != NULL){
        data->values[data->length++] = atoi(temp);
        temp = strtok(NULL, ",");
    }
    return data;
}

int validate_numeric_data(char* str){
    int space_flag = 0, num_flag = 0, comma_flag = 0;
    while(*str!='\0'){
        if(*str==','){
            if(comma_flag)
                return 0;
            comma_flag = 1;
            num_flag = 0;
            space_flag = 0;
        } else if(*str==' '){
            if(num_flag)
                space_flag = 1;
        } else if(isdigit(*str)){
            comma_flag = 0;
            if(space_flag)
                return 0;
            num_flag = 1;
        } else if(*str=='-' || *str=='+'){
            if(num_flag || !isdigit(*(str+1)))
                return 0;
            num_flag = 1;
        } else{
            return 0;
        }
        str++;
    }
    return 1;
}

int validate_label_name(char* label_name, data_table* labels, macro_list* macros){
    if(is_preserved(label_name, macros, labels) || \
        !isalpha(*label_name) || strlen(label_name)>MAX_LABEL_NAME_LENGTH){
        return 0;
    }
    while (*(++label_name) != '\0')
    {
        if(!isalnum(*label_name))
            return 0;
    }
    return 1;
}
