#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/stream.h"
#include "../include/file_line.h"
#include "../include/macro_routine.h"
#include "../include/get_macros.h"
#include "../include/make_am_file.h"
#include "../include/constants.h"
#include "../include/handle_error.h"
#include "../include/allocate_memory.h"
#include "../include/util.h"
#include "../include/data_table.h"
#include "../include/entry_table.h"
#include "../include/parser.h"
#include "../include/assembler.h"

void write_ob_file(short* code_image, data_table* data, char* file_name);

int main(int argc, char** argv){
    short code_image[4000];
    file_head* am_file;
    file_head* ob_file = allocate_memory(sizeof(file_head));
    data_table* data = allocate_memory(sizeof(data_table));
    entry_table* entries = allocate_memory(sizeof(entry_table));
    file_head* test_file = allocate_memory(sizeof(file_head));
    file_head* warnings = allocate_memory(sizeof(file_head));
    file_head* errors = allocate_memory(sizeof(file_head));
    macro_list* macros = allocate_memory(sizeof(macro_list));
    if(argc>1)
        test_file = read_file(test_file, argv[1]);
    else
    {
        printf("No arguments were given\n");
        return 1;
    }
    macros = get_macros(test_file, errors, warnings);
    am_file = make_am_file(test_file, "test.am", macros, warnings);
    ob_file = parse_source(am_file, ob_file, data, entries, errors, warnings);
    make_assembly(code_image, ob_file, data, entries, errors);
    if(warnings->line_count > 0){
        print_warnings(warnings, test_file);
    }
    if(errors->line_count > 0){
        print_errors(errors, test_file);
    } else {
        write_file(am_file, "test.am");
        write_ob_file(code_image, data, "test.ob");
        write_file(ob_file, "test.ob");
    }
    return 0;
}

void write_ob_file(short* code_image, data_table* data, char* file_name){
    FILE* file_pointer = NULL;
    data_unit* current_data = data->head;
    int code_index = -1, data_index = 0, data_counter = MEMORY_ADDRESS_START;
    file_pointer = write_steam(file_pointer, file_name);
    while (code_image[++code_index]!=0)
        {
            fprintf(file_pointer, "%04d %05o\n", code_index+MEMORY_ADDRESS_START, code_image[code_index]);
        }
    data_counter += code_index;
    while (current_data!=NULL){
        switch (current_data->type_code)
        {
        case STRING:
            while(*(current_data->string_data+data_index)!='\0')
                fprintf(file_pointer, "%04d %05o\n", data_counter++, (short)*(current_data->string_data+(data_index++)));
            fprintf(file_pointer, "%04d %05o\n", data_counter++, (short)0);
            break;
        case NUMERIC:
            while(data_index < current_data->num_data->length)
                fprintf(file_pointer, "%04d %05o\n", data_counter++, current_data->num_data->values[data_index++]);
            break;
        }
        data_index = 0;
        current_data = current_data->next;
    }
}