#include <stdio.h>
#include <stdlib.h>

#include "internal.h"

void *_z_memory_realloc(void *pointer, size_t old_size, size_t new_size)
{
    if (new_size == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, new_size);
    if (result == NULL) {
        fprintf(stderr, "[zsvm] FATAL: Call to 'realloc' failed.");
        exit(EXIT_FAILURE);
    }

    return result;
}
