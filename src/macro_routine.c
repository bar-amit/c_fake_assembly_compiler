#include <string.h>
#include <stdlib.h>

#include "../include/macro_routine.h"

macro_routine* find_macro(macro_list* list, char* macro_name){
    macro_routine* current = list->head;
    while(current != NULL){
        if(strcmp(macro_name, current->name)==0)
            return current;
        current = current->next;
    }
    return NULL;
}

macro_routine* get_macro(char* name, int start_line, int end_line){
    macro_routine* macro = malloc(sizeof(macro_routine));
    macro->name = malloc(strlen(name) + 1);
    strcpy(macro->name, name);
    macro->start_line = start_line;
    macro->end_line = end_line;
    macro->is_used = 0;
    macro->next = NULL;
    return macro;
}

void prepend_macro(macro_list* list, char* name, int start_line, int end_line){
    macro_routine* temp = get_macro(name, start_line, end_line);
    if(list->head == NULL){
        list->head = temp;
        return;
    }
    temp->next = list->head;
    list->head = temp;
}
