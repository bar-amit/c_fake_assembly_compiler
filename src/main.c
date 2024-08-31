#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/stream.h"
#include "../include/file_line.h"
#include "../include/macro_routine.h"
#include "../include/get_macros.h"
#include "../include/make_am_file.h"
#include "../include/entry_table.h"
#include "../include/data_table.h"
#include "../include/make_extra_files.h"
#include "../include/constants.h"
#include "../include/handle_error.h"
#include "../include/util.h"
#include "../include/parser.h"
#include "../include/make_ob_file.h"

int main(int argc, char** argv){
    unsigned short code_image[4000];
    file_head* am_file, *code_scaffold, *warnings, *errors;
    data_table* data;
    entry_table* entries;
    file_head* source_file;
    macro_list* macros;

    if(argc<1){
        printf("No arguments were given\n");
        return 1;
    }

    while(--argc>0){
        code_scaffold = (file_head*)malloc(sizeof(file_head));
        data = (data_table*)malloc(sizeof(data_table));
        entries = (entry_table*)malloc(sizeof(entry_table));
        source_file = (file_head*)malloc(sizeof(file_head));
        warnings = (file_head*)malloc(sizeof(file_head));
        errors = (file_head*)malloc(sizeof(file_head));
        macros = (macro_list*)malloc(sizeof(macro_list));
        source_file = read_file(source_file, argv[argc]);
        if(source_file==NULL)
            continue;
        macros = get_macros(source_file, errors, warnings);
        am_file = make_am_file(source_file, macros, warnings);
        code_scaffold = parse_source(am_file, code_scaffold, data, entries, errors, warnings);
        make_assembly(code_image, code_scaffold, data, entries, errors);
        if(warnings->line_count > 0){
            printf("Warnings for file \"%s\":\n", argv[argc]);
            print_warnings(warnings, source_file);
        }
        if(errors->line_count > 0){
            printf("Errors for file \"%s\":\n", argv[argc]);
            print_errors(errors, source_file);
            printf("Please fix all errors to create output files.\n\n");
        } else {
            write_file(am_file, replace_file_ending(argv[argc], ".am"));
            write_ob_file(code_image, data, code_scaffold->line_count, replace_file_ending(argv[argc], ".ob"));
            write_entry_file(entries, data, replace_file_ending(argv[argc], ".ent"));
            write_external_file(entries, replace_file_ending(argv[argc], ".ext"));
        }
    }
    return 0;
}
