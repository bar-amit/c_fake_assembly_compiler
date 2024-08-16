#include <ctype.h>
#include <string.h>

#include "../include/macro_routine.h"
#include "../include/label_table.h"
#include "../include/parser.h"
#include "../include/allocate_memory.h"

#define MAX_LABEL_NAME_LENGTH 31

int validate_label_name(char* label_name, label_table* labels, macro_list* macros){
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

label* get_label(char* name, int type){
    label* new_label = allocate_memory(sizeof(label), "get_label<new_label>");
    new_label->name = allocate_memory(sizeof(char) * strlen(name), "get_label<name>");
    strcpy(new_label->name, name);
    new_label->type_code = type;
    return new_label;
}

void prepend_label(label_table* table, label* l){
    if(table->head == NULL){
        table->head = l;
        return;
    }
    l->next = table->head;
    table->head = l;
}

label* find_label(char* label_name, label_table* table){
    label* temp = table ? table->head : NULL;
    while(temp!=NULL && strcmp(temp->name, label_name)!=0)
        temp = temp->next;
    return temp;
}
