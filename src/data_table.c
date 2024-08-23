#include <ctype.h>
#include <string.h>

#include "../include/macro_routine.h"
#include "../include/data_table.h"
#include "../include/allocate_memory.h"
#include "../include/constants.h"

data_unit* create_data(char* name, int type){
    data_unit* new_label = allocate_memory(sizeof(data_unit), "get_label<new_label>");
    if(name != NULL){
        new_label->name = allocate_memory(strlen(name) + 1, "get_label<name>");
        strcpy(new_label->name, name);
        new_label->type_code = type;
    }
    return new_label;
}

void prepend_data(data_table* table, data_unit* data){
    if(data==NULL)
        return;
    if(table->head == NULL){
        table->head = data;
        table->data_count = 0;
        data->data_count = table->data_count;
        return;
    }
    data->data_count = table->data_count;
    data->next = table->head;
    table->head = data;
    if(data->type_code == NUMERIC)
        table->data_count += data->num_data->length;
    else
        table->data_count += strlen(data->string_data)+1;
}

data_unit* find_data(char* label_name, data_table* table){
    data_unit* temp = table ? table->head : NULL;
    while(temp!=NULL && strcmp(temp->name, label_name)!=0)
        temp = temp->next;
    return temp;
}

data_table* reverse_data(data_table* file){
    data_unit *next = NULL, *prev = NULL, *head = file->head;
    while(head != NULL){
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    file->head = prev;
    return file;
}
