#include <stdlib.h>

#include "memory.h"

void* _zsvmReallocate(void* pointer, size_t newSize) {
	if (newSize == 0) {
		free(pointer);
		return NULL;
	}

	void* result = realloc(pointer, newSize);
	if (result == NULL) exit(1);
	return result;
}