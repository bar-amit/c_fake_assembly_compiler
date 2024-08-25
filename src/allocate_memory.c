#include <stdlib.h>

#include "../include/allocate_memory.h"

void *allocate_memory(long object_size){
    void *object_pointer = malloc(object_size);
    if (object_pointer == NULL) {
        return NULL;
    }
    return object_pointer;
}
