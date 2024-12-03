#ifndef __MIDDLEWARE_H__
#define __MIDDLEWARE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "logger.h"
#include "controller.h"
#include "fd_el.h"

int authenticate(int fd);

#endif /// __MIDDLEWARE_H__