#include "middleware.h"
#include "interpreter.h"
#include "controller.h"
#include "logger.h"
#include "Aquarium.h"

#define MIDDLEWARE_BUF_SIZE 256

static int e = EMPTY_FD;
const char *ng = "no greetings";
const char *greetings = "greetings ";

int findEmptyDisplay(int fd)
{
    const Display *d = Aquarium_setFdOnDisplay(e, fd);
    if (d == NULL)
    {
        write(fd, ng, strlen(ng));
        return 1;
    }
    else
    {
        char buf[MIDDLEWARE_BUF_SIZE];
        strcpy(buf, greetings);
        strcat(buf, d->name);
        strcat(buf, "\n");
        write(fd, buf, strlen(buf));
    }
    log_message(INFO, "Fd %d is logged to display %s", d->client_fd, d->name);
    return 0;
}
int middleware_hello(struct line *l, int fd)
{
    Display *d = NULL;
    if (l == NULL)
    {
        return findEmptyDisplay(fd);
    }
    else
    {
        if (strcmp(l->element, "in") == 0 && strcmp(l->next->element, "as") == 0)
        {
            l = l->next->next;
            d = Aquarium_findDisplayByName(l->element);
            if (d != NULL)
            {
                char buf[MIDDLEWARE_BUF_SIZE];
                strcpy(buf, greetings);
                strcat(buf, d->name);
                d->client_fd = fd;
                Aquarium_updatePing(d);
                write(fd, buf, strlen(buf));
                log_message(INFO, "Fd %d is logged to display %s", d->client_fd, d->name);
            }
            else
            {
                return findEmptyDisplay(fd);
            }
        }
        else
        {
            log_message(WARN, "Hello arguments interpretation failed");
            return 1;
        }
    }
    return 0;
}

struct command middleware_cl[2] = {
    {"hello", NULL, middleware_hello},
};

int authenticate(int fd)
{
    char buffer[MIDDLEWARE_BUF_SIZE];
    Display *d = Aquarium_findDisplayByFd(fd);
    if (d != NULL)
    {
        if (Aquarium_displayIsTimeout(d))
        {
            log_message(INFO, "Display %s with fd %d has expired.", d->name, d->client_fd);
            Aquarium_setFdOnDisplay(d->client_fd, e);
            write(fd, "NOK", strlen("NOK"));
            return 0;
        }
        int answer_err = answer(fd);
        if (answer_err == 0)
        {
            Aquarium_updatePing(d);
        }
        return answer_err;
    }
    else
    {
        exit_if(read(fd, buffer, MIDDLEWARE_BUF_SIZE) < 0, "Client reading failed");
        struct command *my_command;
        my_command = interpreter(buffer, " ", middleware_cl, 1);
        if (my_command == NULL)
        {
            write(fd, "NOK", strlen("NOK"));
            return 0;
        }
        if (my_command->func(my_command->args, fd) != 0)
        {
            write(fd, "NOK", strlen("NOK"));
        }
        free_line(my_command->args);
        return 0;
    }
}