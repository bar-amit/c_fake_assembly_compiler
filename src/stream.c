#include <stdio.h>
#include <stdlib.h>

#include "../include/stream.h"

FILE *write_steam(FILE *file_pointer, char *file_name){
    return open_stream(file_pointer, file_name, "w");
}

FILE *read_stream(FILE *file_pointer, char *file_name){
    return open_stream(file_pointer, file_name, "r");
}

FILE *open_stream(FILE *file_pointer, char *file_name, char *mode){
    file_pointer = fopen(file_name, mode);
    if(file_pointer==NULL)
        printf("can't open file %s\n", file_name);
    return file_pointer;
}
