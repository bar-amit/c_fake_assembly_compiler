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

int main(int argc, char** argv){
    file_head* am_file;
    file_head* test_file = allocate_memory(sizeof(file_head), "main<test_file>");
    macro_list* macros = allocate_memory(sizeof(macro_list), "main<macros>");
    macros->head = NULL;
    if(argc>1)
        test_file = read_file(test_file, argv[1]);
    else
    {
        printf("No arguments were given\n");
        return 1;
    }
    am_file = make_am_file(test_file, "test.am", get_macros(test_file->head));
    write_file(am_file, "test.am");
    return 0;
}
