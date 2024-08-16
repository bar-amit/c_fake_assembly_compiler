#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/macro_routine.h"
#include "../include/label_table.h"
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

int is_preserved(char *name, macro_list *macros, label_table* labels){
    return !(is_register(name)
        || is_operation(name)
        || is_instraction(name)
        || find_macro(macros, name)!=NULL
        || find_label(name, labels));
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

int* parse_numaric_data(char *str, int *values, char *error){
    int index = 0;
    char* temp = strtok(str, ",");
    while(temp != NULL){
        if(!validate_numaric(temp)){
            strcpy(error, "Bad data");
            return values;
        }
        values[index++] = atoi(temp);
        temp = strtok(NULL, ",");
    }
    return values;
}

int validate_numaric(char* str){
    while(*str != '\0'){
        if(!isdigit(*str))
            return 0;
        str++;
    }
    return 1;
}
