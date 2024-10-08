#include <string.h>
#include <stdlib.h>

#include "../include/constants.h"
#include "../include/entry_table.h"
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
    entry_label* new_entry = malloc(sizeof(entry_label));
    new_entry->name = malloc(strlen(name) + 1);
    strcpy(new_entry->name, name);
    if(instraction_line!=-1)
        new_entry->instraction_line = instraction_line;
    else
        new_entry->instraction_line = -1;
    new_entry->type_code = type;
    new_entry->external_count = 0;
    return new_entry;
}

void add_external_use(entry_label* entry, int position, int operand_amount, int line_number){
    int address = line_number;
    if(position==SOURCE_OPERAND || operand_amount==1)
        address += 1;
    else
        address += 2;
    entry->external_count++;
    entry->used_external = realloc(entry->used_external, sizeof(int) * (entry->external_count));
    entry->used_external[entry->external_count-1] = address;
}
