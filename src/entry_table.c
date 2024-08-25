#include <string.h>

#include "../include/entry_table.h"
#include "../include/allocate_memory.h"

void prepend_entry(entry_table* table, entry_label* entry){
    if(table->head == NULL){
        table->head = entry;
        return;
    }
    entry->next = table->head;
    table->head = entry;
}

entry_label* find_entry(char* label_name, entry_table* table){
    entry_label* temp = table ? table->head : NULL;
    while(temp!=NULL && strcmp(temp->name, label_name)!=0)
        temp = temp->next;
    return temp;
}

entry_label* create_entry(char* name, int type, int source_line, int instraction_line, int is_declared){
    entry_label* new_entry = allocate_memory(sizeof(entry_label));
    new_entry->name = allocate_memory(strlen(name) + 1);
    strcpy(new_entry->name, name);
    new_entry->type_code = type;
    return new_entry;
}
