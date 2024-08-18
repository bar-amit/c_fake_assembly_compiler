#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#include "../include/macro_routine.h"
#include "../include/file_line.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/parser.h"
#include "../include/get_macros.h"
#include "../include/allocate_memory.h"
#include "../include/handle_error.h"
#include "../include/constants.h"
#include "../include/util.h"

macro_list* get_macros(file_head* source_file, file_head* errors, file_head* warnings){
    macro_list* list = allocate_memory(sizeof(macro_list), "get_macros<list>");
    file_line *current_line = source_file->head;
    int start_line, end_line;
    char *name;
    while (current_line != NULL)
    {
        if(is_macro(current_line->content)){
            start_line = current_line->line_number;
            get_name(&name, current_line->content);
            if(is_preserved(name, list, NULL)){
                handle_message(errors, "macro name is already taken", start_line);
            } else if (strchr(name, ' ')!=NULL){
                handle_message(errors, "macro name can't have a space", start_line);
            }
            while (!is_end((current_line = current_line->next)->content))
            {
                if(current_line == NULL){
                    handle_message(errors, "macro without ending", start_line);
                }
            }
            end_line = current_line->line_number;
            if(start_line-end_line == 1){
                handle_message(warnings, "empty macro", start_line);
            }
            prepend_macro(list, name, start_line, end_line);
        }
        current_line = current_line->next;
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
