#ifndef __SET_H__
#define __SET_H__

#define UNUSED(x) (void)(x)

#include <stddef.h>
struct set;

/**
Return a struct set representing the empty set
*/
struct set *set__empty(int (*cmp)(const void *, const void *), void *(*copy)(const void *), void (*del)(void *));
/**
Returns non-zero if se is empty, 0 otherwise
*/
int set__is_empty(const struct set *se);

/**
Returns non-zero if c belongs to se, 0 otherwise
*/
int set__find(const struct set *se, void *);

/**
Returns an element if condition is true, 0 otherwise
*/
void *set__search_by(const struct set *se, int(cond)(void *, void *), void *args);

/**
Returns the number of elements in se
*/
size_t set__size(const struct set *se);

/** Adds c to se
Returns 0 if c has been added to se, a negative value otherwise
NB: se should not be modified if c cannot be added to se
*/
int set__add(struct set *se, void *);

/**
Removes c from se
Returns 0 if c has been removed from se, a negative value otherwise
NB: se should not be modified if c cannot be removed from se
*/
int set__remove(struct set *se, void *);

/**
Free a struct
*/
void set__free(struct set *se);

/**
 * Apply a filter function (take a int and return a boolean) on a given set
 *
 */
struct set *set__filter(int (*filter)(void *, void *), const struct set *se, void *args);

/**
 * Apply a filter function (take a int and return a boolean) on a given set
 *
 */
void set__for_each(void (*function)(void *, void *), struct set *se, void *args);

#endif // __SET_H__
