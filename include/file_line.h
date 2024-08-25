#include "./constants.h"

typedef struct file_line {
    char *content;
    int line_number;
    struct file_line *next;
} file_line;

typedef struct file_head {
    struct file_line *head;
    int line_count;
} file_head;

void* allocate_memory_file_line();
file_line* copy_line(file_line* source);
file_line* find_line(file_line* line_list, int line_number);
file_head* read_file(file_head *file, char* file_name);
void write_file(file_head *file, char* file_name);
int check_comment(char *line);
int check_empty(char *line);
file_head* reverse_file(file_head* file);
void prepend_line(file_head* file, file_line* line);
file_line* create_line(char* content, int line_number);
