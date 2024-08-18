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

int main(int argc, char** argv){
    file_head* am_file, *ob_file;
    file_head* test_file = allocate_memory(sizeof(file_head), "main<test_file>");
    file_head* warnings = allocate_memory(sizeof(file_head), "main<test_file>");
    file_head* errors = allocate_memory(sizeof(file_head), "main<test_file>");
    macro_list* macros = allocate_memory(sizeof(macro_list), "main<macros>");
    if(argc>1)
        test_file = read_file(test_file, argv[1]);
    else
    {
        printf("No arguments were given\n");
        return 1;
    }
    macros = get_macros(test_file, errors, warnings);
    am_file = make_am_file(test_file, "test.am", macros, warnings);
    ob_file = parse_source(am_file, errors, warnings);
    if(warnings->line_count > 0){
        print_warnings(warnings, test_file);
    }
    if(errors->line_count > 0){
        print_errors(errors, test_file);
    } else {
        write_file(am_file, "test.am");
        write_file(ob_file, "test.ob");
    }
    return 0;
}
