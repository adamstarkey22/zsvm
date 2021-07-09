#ifndef zsvm_memory_h
#define zsvm_memory_h

#define _ZSVM_GROW_CAPACITY(capacity)             ((capacity) < 8 ? 8 : (capacity) * 2)
#define _ZSVM_GROW_ARRAY(type, pointer, newCount) (type*)_zsvmReallocate(pointer, sizeof(type) * (newCount))
#define _ZSVM_FREE_ARRAY(pointer)                       _zsvmReallocate(pointer, 0)

void* _zsvmReallocate(void* pointer, size_t newSize);

#endif