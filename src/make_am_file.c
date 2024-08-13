#include <stdlib.h>
#include <string.h>

#include "../include/file_line.h"
#include "../include/macro_routine.h"
#include "../include/get_macros.h"
#include "../include/allocate_memory.h"
#include "../include/make_am_file.h"
#include "../include/util.h"

file_head* make_am_file(file_head *source_file, char* file_name, macro_list* list){
    int macro_flag = 0;
    file_head* am_file = allocate_memory(sizeof(file_head), "am_file_head");
    file_line *current_input = source_file->head;
    macro_routine *macro_pointer = NULL;
    char macro_name[MAX_LINE_LENGTH];
    while(current_input != NULL){
        if(is_macro(current_input->content) && macro_flag == 0)
            macro_flag = 1;
        else if(macro_flag == 0){
            remove_space(strcpy(macro_name, current_input->content));
            macro_pointer = find_macro(list, macro_name);
            if(macro_pointer==NULL)
                prepend_line(am_file, copy_line(current_input));
            else {
                spred_macro(source_file, am_file, macro_pointer);
                macro_pointer = NULL;
            }
        }
        else if(is_end(current_input->content))
            macro_flag = 0;
        current_input = current_input->next;
    }
    return reverse_file(am_file);
}

 void spred_macro(file_head* source_file, file_head* am_file, macro_routine* macro){
    file_line* line = find_line(source_file->head, macro->start_line);
        line = line->next;
        while(line->line_number < macro->end_line){
            prepend_line(am_file, copy_line(line));
            line = line->next;
        }
 }
