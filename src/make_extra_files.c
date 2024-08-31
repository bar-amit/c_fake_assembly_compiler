#include <stdio.h>
#include <stdlib.h>

#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/make_extra_files.h"
#include "../include/stream.h"

void write_entry_file(entry_table* entries, int data_address_start, char* file_name){
    entry_label* current = entries->head;
    FILE* file_pointer = NULL;
    while(current!=NULL){
        if(current->is_declared && current->type_code==ENTRY){
            if(file_pointer==NULL)
                file_pointer = write_steam(file_pointer, file_name);
            fprintf(file_pointer, "%s %04d\n", current->name, current->instraction_line);
        }
        if(current->is_declared && current->type_code==DATA)
            fprintf(file_pointer, "%s %04d\n", current->name, current->instraction_line + data_address_start);
        current = current->next;
    }
}

void write_external_file(entry_table* entries, char* file_name){
    int index;
    entry_label* current = entries->head;
    FILE* file_pointer = NULL;
    while(current!=NULL){
        if(current->type_code==EXTERN){
            for(index=0; index<current->external_count; index++){
                if(file_pointer==NULL)
                    file_pointer = write_steam(file_pointer, file_name);
                fprintf(file_pointer, "%s %04d\n", current->name, current->used_external[index]);
            }
        }
        current = current->next;
    }
}