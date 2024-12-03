#ifndef PROMPT_H
#define PROMPT_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#include "interpreter.h"
#include "aquarium_parser.h"
#include "Aquarium.h"

/**
 * Load the aquarium
 *
 * @param l : parsed line used by load, expecting one argument being the name of the aquarium we have to load
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int load(struct line *l, int fd);

/**
 * Show the aquarium
 *
 * @param l : parsed line used by show, exepecting one argument being aquarium
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int show(struct line *l, int fd);

/**
 * Add a view to the current aquarium
 *
 * @param l : parsed line used by add, expecting three arguments being view, the name of the view and its dimensions
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int add(struct line *l, int fd);

/**
 * Delete a view from the current aquarium
 *
 * @param l : parsed line used by del, expecting two arguments being view and the name of the view
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int del(struct line *l, int fd);

/**
 * Save the current aquarium
 *
 * @param l : parsed line used by save, expecting one argument being the name given to the saved aquarium
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int save(struct line *l, int fd);

/**
 * Exit the prompt and stop the program
 *
 * @result return 1
 */
int end();

/**
 * Kill all the fish in the aquarium
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int lessive();

/**
 * Wait the user to enter a command
 *
 * @result 0 if interpretation is correct, non-zero if error
 */
int prompt();

#endif /*PROMPT_H*/
