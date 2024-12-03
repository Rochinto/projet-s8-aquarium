#include "interpreter.h"
#include "logger.h"

struct line *create_line(char *buf, char *sep)
{
    char *rest = NULL;
    char *token;
    token = strtok_r(buf, "\n", &rest);
    token = strtok_r(token, sep, &rest);
    if (token == NULL)
    {
        return NULL;
    }
    struct line *l = malloc(sizeof(struct line));
    struct line *tmp = l;
    struct line *tmp2;
    l->nbelement = 0;
    while (token != NULL)
    {
        tmp->element = token;
        token = strtok_r(NULL, sep, &rest);
        l->nbelement++;
        if (token != NULL)
        {
            tmp2 = malloc(sizeof(struct line));
            tmp->next = tmp2;
        }
        else
        {
            tmp->next = NULL;
        }
        tmp = tmp2;
    }
    return l;
}

struct line *pop_arg(struct line *l)
{
    struct line *next = l->next;
    l->nbelement--;
    if (next)
    {
        next->nbelement = l->nbelement;
    }
    free(l);
    return next;
}

void free_line(struct line *l)
{
    if (l == NULL)
    {
        return;
    }
    struct line *tmp;
    while (l->next != NULL)
    {
        tmp = l->next;
        free(l);
        l = tmp;
    }
    free(l);
}

struct command *interpreter(char *buf, char *sep, struct command command_list[], size_t nb_commands)
{
    struct line *l = create_line(buf, sep);
    int output = -127;
    if (l == NULL)
    {
        log_message(WARN, "Empty command line !");
        return NULL;
    }
    for (size_t i = 0; i < nb_commands; i++)
    {
        if (strcmp(command_list[i].name, l->element) == 0)
        {
            command_list[i].args = l->next;
            l->nbelement--;
            if (command_list[i].args)
            {
                command_list[i].args->nbelement = l->nbelement;
            }
            free(l);
            return &command_list[i];
        }
    }
    if (output == -127)
    {
        log_message(WARN, "Command interpretation failed ! : %s not a valid command", l->element);
        free_line(l);
    }
    return NULL;
}