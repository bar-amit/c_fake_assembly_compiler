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

/**
 * Alias for "malloc(sizeof(file_line))".
 */
void* allocate_memory_file_line();

/**
 * Creates a new line wich is a copy of an existing line.
 * 
 * @param source Existing line to copy.
 */
file_line* copy_line(file_line* source);

/**
 * Searches for a line by line number.
 * 
 * @param line_list Head of file.
 * @param line_number Line number of line to get.
 */
file_line* find_line(file_line* line_list, int line_number);

/**
 * Read a file from system and mirror it in memory.
 * 
 * @param file File head (destination).
 * @param file_name A name of file on system (source).
 */
file_head* read_file(file_head *file, char* file_name);

/**
 * Write a file to system.
 * 
 * @param file File to write.
 * @param file_name Name of output file.
 */
void write_file(file_head *file, char* file_name);

/**
 * Checks if a line is a comment line.
 * 
 * @param line Content of line to check.
 */
int check_comment(char *line);

/**
 * Checks if a line is empty (whitespace).
 * 
 * @param line Line to check.
 */
int check_empty(char *line);

/**
 * Reverses a file to perserve the original line order.
 * 
 * @param file File to reverse.
 */
file_head* reverse_file(file_head* file);

/**
 * Prepends a line to file.
 * 
 * @param file The file to prepend to.
 * @param line A line to prepend to file.
 */
void prepend_line(file_head* file, file_line* line);

/**
 * Creates a new line item of struct file_line.
 * 
 * @param content Content of new line.
 * @param line_number Line number in the source file.
 */
file_line* create_line(char* content, int line_number);
