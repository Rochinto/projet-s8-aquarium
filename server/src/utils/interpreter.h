#ifndef GENERAL_PARSER_H
#define GENERAL_PARSER_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Single-chained list to represent a command line
 *
 * @param element : name of the command or argument
 * @param next : next element of the command line
 */
struct line
{
    char *element;
    struct line *next;
    int nbelement;
};

/**
 * Represents a command and its corresponding execution
 *
 * @param name : command name
 * @param func : function to execute. Gets a command line (list of arguments) and returns non-zero if error
 */
struct command
{
    char *name;
    struct line *args;
    int (*func)(struct line *arg_list, int fd);
};

/**
 * Create command line from buffer
 *
 * @param buf : buffer to convert
 * @param sep : separation character
 *
 * @result line ready for interpretation
 */
struct line *create_line(char *buf, char *sep);

/**
 * Pop first element and return rest of the line
 *
 * @param l: line to shrink
 *
 * @result reduced line
 */
struct line *pop_arg(struct line *l);

/**
 * Free a command line
 *
 * @param l : Command line to free
 */
void free_line(struct line *l);

/**
 * Buffer-like command line interpreter
 *
 * @param buf : Buffer to read
 * @param sep : Commande separator
 * @param command_list : list of executable commands
 * @param nb_commands : number of commands in command_list
 *
 * @result interpreted command if interpretation is correct, NULL if error
 */
struct command *interpreter(char *buf, char *sep, struct command command_list[], size_t nb_commands);

#endif /* GENERAL_PARSER_H */