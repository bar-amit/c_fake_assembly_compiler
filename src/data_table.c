#include <ctype.h>
#include <string.h>

#include "../include/macro_routine.h"
#include "../include/data_table.h"
#include "../include/allocate_memory.h"
#include "../include/constants.h"

data_unit* create_data(char* name, int type){
    data_unit* new_label = allocate_memory(sizeof(data_unit));
    if(name != NULL){
        new_label->name = allocate_memory(strlen(name) + 1);
        strcpy(new_label->name, name);
    }
    else {
        new_label->name = allocate_memory(1);
        strcpy(new_label->name, "");
    }
    new_label->type_code = type;
    return new_label;
}

void prepend_data(data_table* table, data_unit* data){
    if(data==NULL)
        return;
    if(table->head == NULL){
        table->head = data;
        data->data_count = table->data_count = 0;
    } else {
        data->data_count = table->data_count;
        data->next = table->head;
        table->head = data;
    }
    table->data_count += get_data_length(data);
}

int get_data_length(data_unit* data){
    switch (data->type_code){
        case NUMERIC:
            return data->num_data->length;
        case STRING:
            return strlen(data->string_data)+1;
    }
    return 0;
}

data_unit* find_data(char* label_name, data_table* table){
    data_unit* temp = table ? table->head : NULL;
    while(temp!=NULL && strcmp(temp->name, label_name)!=0)
        temp = temp->next;
    return temp;
}

data_table* reverse_data(data_table* table){
    data_unit *next = NULL, *prev = NULL, *head = table->head;
    while(head != NULL){
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    table->head = prev;
    return table;
}
