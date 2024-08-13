#include <stdlib.h>
#include "../include/allocate_memory.h"
#include "../include/handle_error.h"

void *allocate_memory(long object_size, char *debug_id) {
    void *object_pointer = malloc(object_size);
    if (object_pointer == NULL) {
        return NULL;
        handle_error(debug_id, "memory allocation fail");
    }
    return object_pointer;
}
