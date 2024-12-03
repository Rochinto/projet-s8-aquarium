#ifndef __FD_EL_H__
#define __FD_EL_H__

#define UNUSED(x) (void)(x)

#include <stddef.h>

struct fd_el
{
    int fd;
    int view;
    int authenticated;
    char *buffer;
};

int fd_el__cmp(const void *, const void *);
void *fd_el__copy(const void *);
void fd_el__del(void *);

#endif // __FD_EL_H__
