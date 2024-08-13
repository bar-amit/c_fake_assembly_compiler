#include <stdio.h>
#include <stdlib.h>
#include "../include/handle_error.h"

void handle_error(char* function_name, char* error){
        fprintf(stderr, "In %s: ERROR: %s\n", function_name, error);
        exit(2);
}