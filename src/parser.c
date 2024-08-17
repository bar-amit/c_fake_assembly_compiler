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

void parse_line(file_head* ob_output, data_table* labels, entry_table* entries, file_line* line){
    char *temp = allocate_memory(sizeof(char) * strlen(line->content), "parse_line");
    char *word, *label_name;
    strcpy(temp, line->content);
    word = strtok(temp, " ");
    if(is_operation(word)){
        prepend_line(ob_output, parse_operation(word, strtok(NULL, "\n"), line->line_number));
    } else if(is_data(word)){
        prepend_data(labels, parse_data(NULL, word, strtok(NULL, "\n")));
        /*WARNING: data without label*/
    } else if(is_entry(word)){
        parse_entry(entries, strtok(NULL, "\n"), word, line->line_number, -1);
    } else if(is_label(word)){
        label_name = allocate_memory(strlen(word), "parse_line<label_name>");
        strcpy(label_name, word);
        word = strtok(NULL, " ");
        if(is_data(word)){
            prepend_data(labels, parse_data(label_name, word, strtok(NULL, "\n")));
        } else if(is_operation(word)){
            prepend_line(ob_output, parse_operation(word, strtok(NULL, "\n"), line->line_number));
            parse_entry(entries, label_name, ENTRY, line->line_number, ob_output->line_count);
        } else {
            /*error*/
        }
    } else {
        /*error*/
    }
}

file_head* parse_source(file_head* am_file){
    file_line* current_input = am_file->head;
    file_head* ob_output = allocate_memory(sizeof(file_head), "parse_source<ob_output>");
    data_table* labels = allocate_memory(sizeof(data_table), "parse_source<data_table>");
    entry_table* entries = allocate_memory(sizeof(entry_table), "parse_source<entries>");
    while(current_input != NULL){
        parse_line(ob_output, labels, entries, current_input);
        current_input = current_input->next;
    }
    return reverse_file(ob_output);
}

file_line* parse_operation(char* operation, char* orpands, int source_line){
    char* temp = allocate_memory(MAX_LINE_LENGTH, "parse_operation");
    strcpy(temp, operation);
    strcat(temp, " ");
    strcat(temp, strtok(orpands, ","));
    strcat(temp, strtok(NULL, ","));
    if(strtok(NULL, ",")!=NULL)
        printf("ERROR: too many orpands at line %d\n", source_line);
    return create_line(temp, source_line);
}

data_unit* parse_data(char* label_name, char* instraction, char* orpand){
    char* error = allocate_memory(MAX_LINE_LENGTH, "parse_data");
    int type_code = get_data_code(instraction);
    data_unit* new_data = create_data(label_name, type_code);
    switch (type_code){
    case STRING:
        new_data->string_data = parse_string_data(orpand);
        break;
    case NUMERIC:
        new_data->num_data = parse_numeric_data(orpand, error);
        break;
    }
    return new_data;
}

int get_data_code(char *instraction){
    return strcmp(instraction, ".string")==0 ?
        STRING : NUMERIC;
}

void parse_entry(entry_table* entries, char* name, char* type, int source_line, int instraction_line){
    int type_code = get_entry_code(type);
    entry_label* entry = find_entry(name, entries);
    if(entry==NULL){
        entry = create_entry(name, type_code, source_line, instraction_line, instraction_line == -1);
        prepend_entry(entries, entry);
        return;
    }
    else if((entry->instraction_line>=0 && instraction_line>=0) || (instraction_line<0 && entry->is_declared))
        return; /*ERROR: Declared twice || Used twice*/
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

/* file_line* parse_entry_operation
(entry_table* entries, char* label_name, char* operation, char* orpands, int source_line, int instraction_line)
{
    parse_entry(entries, name);
} */

int is_preserved(char *name, macro_list *macros, data_table* labels){
    return !(is_register(name)
        || is_operation(name)
        || is_instraction(name)
        || find_macro(macros, name)!=NULL
        || find_data(name, labels));
}

int is_in_list(char *name, char *list[]){
    int i = -1;
    while(list[++i] != NULL)
        if(strcmp(name, list[i])==0) return 1;
    return 0;
}

int is_operation(char* str){
    return is_in_list(str, operations);
}

int is_instraction(char* str){
    return is_in_list(str, instractions);
}

int is_register(char* str){
    return is_in_list(str, registers);
}

int is_label(char* str){
    if(*(str+strlen(str)) == ':')
        return 1;
    return 0;
}

int is_entry(char* str){
    if(strcmp(str, ".entry")==0 || strcmp(str, ".extern")==0)
        return 1;
    return 0;
}

int is_data(char* str){
    return strcmp(str, ".string")==0 || strcmp(str, ".data")==0;
}

int validate_string_data(char* str){
    char *data_start, *data_end;
    data_start = strchr(str, '\"');
    data_end = strrchr(str, '\"');
    if(data_start==NULL || data_start==data_end || *(data_end+1)!='\n')
        return 0;
    return 1;
}

char* parse_string_data(char* str){
    char *data = allocate_memory(sizeof(char)*MAX_LINE_LENGTH, "parse_string_data");
    char *data_start, *data_end;
    data_start = strchr(str, '\"');
    data_end = strrchr(str, '\"');
    strncpy(data, data_start, (int)(data_end - data_start));
    return data;
}

numeric_data* parse_numeric_data(char *str, char *error){
    numeric_data *data = allocate_memory(sizeof(numeric_data), "parse_numeric_data");
    char* temp = strtok(str, ",");
    data->lenght = 0;
    while(temp != NULL){
        if(!validate_numaric_data(temp)){
            strcpy(error, "Bad data");
            return data;
        }
        data->values[data->lenght++] = atoi(temp);
        temp = strtok(NULL, ",");
    }
    return data;
}

int validate_numaric_data(char* str){
    while(*str != '\0'){
        if(!isdigit(*str))
            return 0;
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
