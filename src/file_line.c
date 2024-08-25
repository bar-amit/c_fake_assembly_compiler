#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "../include/file_line.h"
#include "../include/allocate_memory.h"
#include "../include/constants.h"
#include "../include/stream.h"
#include "../include/util.h"

void* allocate_memory_file_line(){
    return allocate_memory(sizeof(file_line));
}

file_line* copy_line(file_line* source){
    file_line* destination = allocate_memory_file_line();
    destination->content = source->content;
    destination->line_number = source->line_number;
    destination->next = NULL;
    return destination;
}

file_line* find_line(file_line* line_list, int line_number){
    while(line_list!=NULL){
        if(line_list->line_number==line_number)
            return line_list;
        line_list = line_list->next;
    }
    return NULL;
}

file_line* create_line(char* content, int line_number){
    file_line* line = allocate_memory_file_line();
    line->content = allocate_memory(strlen(content) + 1);
    strcpy(line->content, content);
    line->line_number = line_number;
    line->next = NULL;
    return line;
}

void prepend_line(file_head* file, file_line* line){
    if(file->head == NULL){
        file->head = line;
        file->line_count = 1;
        return;
    }
    line->next = file->head;
    file->head = line;
    file->line_count++;
}

file_head* reverse_file(file_head* file){
    file_line *next = NULL, *prev = NULL, *head = file->head;
    while(head != NULL){
        next = head->next;
        head->next = prev;
        prev = head;
        head = next;
    }
    file->head = prev;
    return file;
}

file_head* read_file(file_head *file, char* file_name){
    int line_index = 0;
    FILE *file_pointer = NULL;
    char *line = allocate_memory(MAX_LINE_LENGTH);
    file_pointer = read_stream(file_pointer, file_name);
    if(file_pointer==NULL)
        return NULL;
    while((line = fgets(line, MAX_LINE_LENGTH, file_pointer))){
        line_index++;
        clean_whitespace(line);
        if(check_empty(line) || check_comment(line))
            continue;
        prepend_line(file, create_line(line, line_index));
    }
    free(line);
    return reverse_file(file);
}

void write_file(file_head *file, char* file_name){
    FILE* file_pointer = NULL;
    file_line* current = file->head;
    file_pointer = write_steam(file_pointer, file_name);
    while(current != NULL){
        fputs(current->content, file_pointer);
        current = current->next;
    }
}

int check_comment(char *line){
    if(line[0] == COMMENT_MARKER)
        return 1;
    return 0;
}

int check_empty(char *line){
    if(line==NULL || line[0] == '\n')
        return 1;
    return 0;
}
