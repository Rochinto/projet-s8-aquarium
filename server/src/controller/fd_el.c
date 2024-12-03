#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "set.h"
#include "fd_el.h"

int fd_el__cmp(const void *fd1, const void *fd2)
{
    return ((const struct fd_el *)fd1)->fd - ((const struct fd_el *)fd2)->fd;
}

void *fd_el__copy(const void *fd)
{
    struct fd_el *new_fd_el = malloc(sizeof(struct fd_el));
    new_fd_el->fd = ((const struct fd_el *)fd)->fd;
    new_fd_el->view = ((const struct fd_el *)fd)->view;
    new_fd_el->authenticated = ((const struct fd_el *)fd)->authenticated;
    new_fd_el->buffer = malloc(sizeof(char) * (strlen(((const struct fd_el *)fd)->buffer) + 1));
    strcpy(new_fd_el->buffer, ((const struct fd_el *)fd)->buffer);
    return new_fd_el;
}

void fd_el__del(void *fd)
{
    free(((struct fd_el *)fd)->buffer);
    free(fd);
}