#include <ctype.h>
#include <string.h>

#include "../include/macro_routine.h"
#include "../include/label_table.h"
#include "../include/parser.h"

int validate_label_name(char* label_name, label_table* labels, macro_list* macros){
    if(is_preserved(label_name, macros) || \
        !isalpha(*label_name) || strlen(label_name>MAX_LABEL_NAME_LENGTH)){
        return 0;
    }
    while (*(++label_name) != '\0')
    {
        if(!isalnum(*label_name))
            return 0;
    }
    return 1;
}
