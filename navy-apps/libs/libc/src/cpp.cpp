#include <stdlib.h>

<<<<<<< HEAD
void* operator new(size_t size) { return malloc(size); }

void* operator new[](size_t size) { return malloc(size); }

void operator delete(void* p) { free(p); }

void operator delete[](void* p) { free(p); }
=======
void *operator new(size_t size) { return malloc(size); }

void *operator new[](size_t size) { return malloc(size); }

void operator delete(void *p) { free(p); }

void operator delete[](void *p) { free(p); }
>>>>>>> master
