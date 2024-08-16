#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include "../include/macro_routine.h"
#include "../include/file_line.h"
#include "../include/label_table.h"
#include "../include/parser.h"
#include "../include/get_macros.h"
#include "../include/allocate_memory.h"
#include "../include/handle_error.h"
#include "../include/constants.h"
#include "../include/util.h"

macro_list* get_macros(file_line* file){
    macro_list* list = allocate_memory(sizeof(macro_list), "get_macros<list>");
    int start_line, end_line;
    char *name, error[100];
    while (file != NULL)
    {
        if(is_macro(file->content)){
            start_line = file->line_number;
            get_name(&name, file->content);
            if(!is_preserved(name, list, NULL)){
                printf("name error at %d\n", file->line_number);
                /*
                get_name_error(error, file->line_number);
                handle_error("get_macros<validate_name>", error); */
            }
            while (!is_end((file = file->next)->content))
            {
                if(file == NULL){
                    handle_error("get_macros<no_end>", get_end_error(error, start_line));
                }
            }
            end_line = file->line_number;
            prepend_macro(list, name, start_line, end_line);
        }
        file = file->next;
    }
    return list;
}

void get_name(char **target, char *line){
    *target = (char*)allocate_memory(sizeof(char)*MAX_LINE_LENGTH, "get_name");
    remove_last_space(line);
    strcpy(*target, (line + strlen(MACRO_START) + 1));
}

int is_macro(char *line){
    return starts_with(MACRO_START, line);
}

int is_end(char *line){
    return starts_with(MACRO_END, line);
}

int starts_with(char *start, char *str){
    return strncmp(start, str, strlen(start))==0 ? 1 : 0;
}

char *int_to_string(int n, char *str){
    *str = (char)(n + '0');
    *(str+1) = '\n';
    *(str+2) = '\0';
    return str;
}

char *get_name_error(char *error, int line_number){
    char *line = allocate_memory(sizeof(char)*3, "get_name_error");
    error = "Bad macro name at line ";
    error = strcat(error, int_to_string(line_number, line));
    return error;
}

char *get_end_error(char *error, int line_number){
    char *line = allocate_memory(sizeof(char)*3, "get_name_error");
    error = "Missing end of macro starting from line ";
    error = strcat(error, int_to_string(line_number, line));
    return error;
}
