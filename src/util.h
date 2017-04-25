#ifndef UTIL_H
#define UTIL_H
extern void warn(const char *message, ...);
extern void *mmalloc(size_t size);
extern void *rrealloc(void *ptr, size_t size);
#endif
