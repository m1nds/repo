#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

struct vector
{
    size_t size;
    size_t capacity;
    char *data;
};

/*
 * @def: creates a vector (Dynamic array)
 * @args capacity: the vector capacity. All other values
 *  are initialized to zero;
 */
struct vector *vector_create(size_t capacity);

/*
 * @def: appends a character at the end of the vector
 * @args vector: the vector where the data is appended
 *       data: the character to be appended
 */
void vector_append(struct vector *vector, char data);

/*
 * @def: destroys a vector
 * @args vector: the vector to be freed
 */
void vector_destroy(struct vector *vector);

#endif /* ! VECTOR_H */