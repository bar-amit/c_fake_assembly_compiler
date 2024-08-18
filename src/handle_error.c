#include <stdio.h>
#include <stdlib.h>

#include "../include/file_line.h"
#include "../include/handle_error.h"

void handle_message(file_head* messages, char* message, int source_line){
    if(find_line(messages->head, source_line)==NULL)
        prepend_line(messages, create_line(message, source_line));
}

void print_warnings(file_head* warnings, file_head* source_file){
    print_messages(warnings, source_file, "WARNING");
}
void print_errors(file_head* errors, file_head* source_file){
    print_messages(errors, source_file, "ERROR");
}

void print_messages(file_head* lines, file_head* source_file, char* message){
    file_line* current, *source;
    reverse_file(lines);
    current = lines->head;
    while(current!=NULL){
        source = find_line(source_file->head, current->line_number);
        printf("%s: %s at line %d:\n", message, current->content, current->line_number);
        printf("\t%s\n", source->content);
        current = current->next;
    }
}
