#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct vec_meta
{
    size_t size;            // number of elements in the vector
    size_t capacity;        // the total capacity of how many elements the vector can hold
} vec_meta;


#ifndef VECTOR_IMPLEMENTATION
#define using_vector( T )                                                                                   \
T* vec_new_##T( size_t size, T val );                                                                       \
T* vec_resize_##T( T* self, size_t size, T val );                                                           \
T* vec_clear_##T( T* self );                                                                                \
T* vec_assign_##T( T* self, size_t size, T val );                                                           \
T* vec_back_##T( T* self );                                                                                 \
T* vec_shrink_to_fit_##T( T* self );                                                                        \
T* vec_reserve_##T( T* self, size_t size );                                                                 \
T* vec_insert_##T( T* self, size_t position, T val );                                                       \
T* vec_insert_arr_##T( T* self, size_t position, T* arr, size_t size );                                     \
T* vec_push_back_##T( T* self, T val );                                                                     \
T* vec_pop_back_##T( T* self );                                                                             \
size_t vec_size_##T( T* self );                                                                             \
size_t vec_capacity_##T( T* self );                                                                         \
void vec_destroy_##T( T* self );                                                                            \


// use `_Generic` keyword from C11 to wrap all of the function above
// the following example shows how to use this library with { int, char, double } vectors
// copy, paste, and modify them for easy use. 
/*

using_vector(int)
using_vector(char)
using_vector(double)

#define vec_resize( self, size, val ) _Generic( (self),                                                     \
    int*:       vec_resize_int,                                                                             \
    char*:      vec_resize_char,                                                                            \
    double*:    vec_resize_double                                                                           \
) ( self, size, val )                                                                                       \

#define vec_clear( self ) _Generic( (self),                                                                 \
    int*:       vec_clear_int,                                                                              \
    char*:      vec_clear_char,                                                                             \
    double*:    vec_clear_double                                                                            \
) ( self )                                                                                                  \

#define vec_assign( self, size, val ) _Generic( (self),                                                     \
    int*:       vec_assign_int,                                                                             \
    char*:      vec_assign_char,                                                                            \
    double*:    vec_assign_double                                                                           \
) ( self, size, val )                                                                                       \

#define vec_back( self ) _Generic( (self),                                                                  \
    int*:       vec_back_int,                                                                               \
    char*:      vec_back_char,                                                                              \
    double*:    vec_back_double                                                                             \
) ( self, size, val )                                                                                       \

#define vec_shrink_to_fit( self ) _Generic( (self),                                                         \
    int*:       vec_shrink_to_fit_int,                                                                      \
    char*:      vec_shrink_to_fit_char,                                                                     \
    double*:    vec_shrink_to_fit_double                                                                    \
) ( self )                                                                                                  \

#define vec_reserve( self, size ) _Generic( (self),                                                         \
    int*:       vec_reserve_int,                                                                            \
    char*:      vec_reserve_char,                                                                           \
    double*:    vec_reserve_double                                                                          \
) ( self, size )                                                                                            \

#define vec_insert( self, position, val ) _Generic( (self),                                                 \
    int*:       vec_insert_int,                                                                             \
    char*:      vec_insert_char,                                                                            \
    double*:    vec_insert_double                                                                           \
) ( self, position, val )                                                                                   \

#define vec_insert_arr( self, position, arr, size ) _Generic( (self),                                       \
    int*:       vec_insert_arr_int,                                                                         \
    char*:      vec_insert_arr_char,                                                                        \
    double*:    vec_insert_arr_double                                                                       \
) ( self, position, arr, size )                                                                             \

#define vec_push_back( self, val ) _Generic( (self),                                                        \
    int*:       vec_push_back_int,                                                                          \
    char*:      vec_push_back_char,                                                                         \
    double*:    vec_push_back_double                                                                        \
) ( self, val )                                                                                             \

#define vec_pop_back( self ) _Generic( (self),                                                              \
    int*:       vec_pop_back_int,                                                                           \
    char*:      vec_pop_back_char,                                                                          \
    double*:    vec_pop_back_double                                                                         \
) ( self )                                                                                                  \

#define vec_size( self ) _Generic( (self),                                                                  \
    int*:       vec_size_int,                                                                               \
    char*:      vec_size_char,                                                                              \
    double*:    vec_size_double                                                                             \
) ( self )                                                                                                  \

#define vec_capacity( self ) _Generic( (self),                                                              \
    int*:       vec_capacity_int,                                                                           \
    char*:      vec_capacity_char,                                                                          \
    double*:    vec_capacity_double                                                                         \
) ( self )                                                                                                  \

#define vec_destroy( self ) _Generic( (self),                                                               \
    int*:       vec_destroy_int,                                                                            \
    char*:      vec_destroy_char,                                                                           \
    double*:    vec_destroy_double                                                                          \
) ( self )                                                                                                  \

*/



#endif  // VECTOR_IMPLEMENTATION
#endif  // __VECTOR_H__


// use this macro to create all the function for that specific type
// T is the type of needed vector
#ifdef VECTOR_IMPLEMENTATION
#define using_vector( T )                                                                                   \
/* create a vector with size size, all initialize to val */                                                 \
T* vec_new_##T( size_t size, T val )                                                                        \
{                                                                                                           \
    size_t cap;                                                                                             \
    if ( size < 16 ) cap = 16;                                                                              \
    else cap = size;                                                                                        \
    vec_meta* vector = malloc( sizeof ( vec_meta ) );                                                       \
    vector->size = size;                                                                                    \
    vector->capacity = 16;                                                                                  \
    if ( cap > vector->capacity )                                                                           \
    {                                                                                                       \
        while ( vector->capacity < cap )                                                                    \
        {                                                                                                   \
            if ( vector->capacity <= 1024 )                                                                 \
            {                                                                                               \
                vector->capacity *= 2;                                                                      \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                vector->capacity += 512;                                                                    \
            }                                                                                               \
        }                                                                                                   \
    }                                                                                                       \
    vector = realloc( vector, sizeof ( vec_meta ) + sizeof (T) * vector->capacity );                        \
    T* data = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                             \
    for ( size_t i = 0; i < size; i++ )                                                                     \
    {                                                                                                       \
        data[i] = val;                                                                                      \
    }                                                                                                       \
    return data;                                                                                            \
}                                                                                                           \
/* get the address for vec_meta */                                                                          \
static inline vec_meta* vec_get_meta_##T( T* self )                                                         \
{                                                                                                           \
    return (vec_meta*) ( (char*) self - sizeof ( vec_meta ) );                                              \
}                                                                                                           \
/* change vector to specific size */                                                                        \
T* vec_resize_##T( T* self, size_t size, T val )                                                            \
{                                                                                                           \
    vec_meta* vector = vec_get_meta_##T( self );                                                            \
    size_t cap;                                                                                             \
    if ( size < 16 ) cap = 16;                                                                              \
    else cap = size;                                                                                        \
    if ( cap > vector->capacity )                                                                           \
    {                                                                                                       \
        while ( vector->capacity < cap )                                                                    \
        {                                                                                                   \
            if ( vector->capacity <= 1024 )                                                                 \
            {                                                                                               \
                vector->capacity *= 2;                                                                      \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                vector->capacity += 512;                                                                    \
            }                                                                                               \
        }                                                                                                   \
        vector = realloc( vector, sizeof ( vec_meta ) + sizeof ( T ) * vector->capacity );                  \
        self = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                            \
        for ( size_t i = vector->size; i < size; i++ )                                                      \
        {                                                                                                   \
            self[i] = val;                                                                                  \
        }                                                                                                   \
        vector->size = size;                                                                                \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        while ( cap > 1024 )                                                                                \
        {                                                                                                   \
            if ( cap + 512 < vector->capacity )                                                             \
            {                                                                                               \
                vector->capacity -= 512;                                                                    \
            }                                                                                               \
            else break;                                                                                     \
        }                                                                                                   \
        while ( cap < 1024 )                                                                                \
        {                                                                                                   \
            if ( cap * 2 < vector->capacity )                                                               \
            {                                                                                               \
                vector->capacity /= 2;                                                                      \
            }                                                                                               \
            else break;                                                                                     \
        }                                                                                                   \
        vector = realloc( vector, sizeof ( vec_meta ) + sizeof ( T ) * vector->capacity );                  \
        self = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                            \
        vector->size = size;                                                                                \
    }                                                                                                       \
    return self;                                                                                            \
}                                                                                                           \
/* clear, remove all elements in the vector, may not reallocate */                                          \
T* vec_clear_##T( T* self )                                                                                 \
{                                                                                                           \
    vec_meta* vector = vec_get_meta_##T( self );                                                            \
    vector->size = 0;                                                                                       \
    vector->capacity = 16;                                                                                  \
    vector = realloc( vector, sizeof ( vec_meta ) + sizeof ( T ) * vector->capacity );                      \
    self = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                                \
    memset( self, 0, sizeof (T) * vector->capacity );                                                       \
    return self;                                                                                            \
}                                                                                                           \
/* Assigns new contents to the vector and modifying its size accordingly */                                 \
T* vec_assign_##T( T* self, size_t size, T val )                                                            \
{                                                                                                           \
    vec_meta* vector = vec_get_meta_##T( self );                                                            \
    size_t cap;                                                                                             \
    if ( size < 16 ) cap = 16;                                                                              \
    else cap = size;                                                                                        \
    if ( cap > vector->capacity )                                                                           \
    {                                                                                                       \
        while ( vector->capacity < cap )                                                                    \
        {                                                                                                   \
            if ( vector->capacity <= 1024 )                                                                 \
            {                                                                                               \
                vector->capacity *= 2;                                                                      \
            }                                                                                               \
            else                                                                                            \
            {                                                                                               \
                vector->capacity += 512;                                                                    \
            }                                                                                               \
        }                                                                                                   \
        vector = realloc( vector, sizeof ( vec_meta ) + sizeof ( T ) * vector->capacity );                  \
        self = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                            \
        for ( size_t i = 0; i < size; i++ )                                                                 \
        {                                                                                                   \
            self[i] = val;                                                                                  \
        }                                                                                                   \
        vector->size = size;                                                                                \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        while ( cap > 1024 )                                                                                \
        {                                                                                                   \
            if ( cap + 512 < vector->capacity )                                                             \
            {                                                                                               \
                vector->capacity -= 512;                                                                    \
            }                                                                                               \
            else break;                                                                                     \
        }                                                                                                   \
        while ( cap < 1024 )                                                                                \
        {                                                                                                   \
            if ( cap * 2 < vector->capacity )                                                               \
            {                                                                                               \
                vector->capacity /= 2;                                                                      \
            }                                                                                               \
            else break;                                                                                     \
        }                                                                                                   \
        vector = realloc( vector, sizeof ( vec_meta ) + sizeof ( T ) * vector->capacity );                  \
        self = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                            \
        for ( size_t i = 0; i < size; i++ )                                                                 \
        {                                                                                                   \
            self[i] = val;                                                                                  \
        }                                                                                                   \
        vector->size = size;                                                                                \
    }                                                                                                       \
    return self;                                                                                            \
}                                                                                                           \
/* get the reference of the first item */                                                                   \
T* vec_front_##T( T* self )                                                                                 \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    if ( vector->size > 0 ) return self;                                                                    \
    else return NULL;                                                                                       \
}                                                                                                           \
/* get the reference of the last item */                                                                    \
T* vec_back_##T( T* self )                                                                                  \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    if ( vector->size > 0 ) return &self[ vector->size - 1 ];                                               \
    else return NULL;                                                                                       \
}                                                                                                           \
/* Returns the number of elements in the vector */                                                          \
size_t vec_size_##T( T* self )                                                                              \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    return vector->size;                                                                                    \
}                                                                                                           \
/* get the allocated size for the vector in terms of elements */                                            \
size_t vec_capacity_##T( T* self )                                                                          \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    return vector->capacity;                                                                                \
}                                                                                                           \
/* Requests the container to reduce its capacity to fit its size */                                         \
T* vec_shrink_to_fit_##T( T* self )                                                                         \
{                                                                                                           \
    return vec_resize_##T( self, vec_size_##T(self), 0 );                                                   \
}                                                                                                           \
/* Requests that the vector capacity be at least enough to contain size elements */                         \
T* vec_reserve_##T( T* self, size_t size )                                                                  \
{                                                                                                           \
    if ( size <= vec_size_##T( self ) ) return self;                                                        \
    else                                                                                                    \
    {                                                                                                       \
        return vec_resize_##T( self, size, 0 );                                                             \
    }                                                                                                       \
}                                                                                                           \
/* extend the vector by inserting an element at the specified position */                                   \
T* vec_insert_##T( T* self, size_t position, T val )                                                        \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    if ( position > vector->size )                                                                          \
    {                                                                                                       \
        fprintf( stderr, "[ERRO]: index out of bounds, aborted\n" );                                        \
        exit(1);                                                                                            \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        vector->size++;                                                                                     \
        self = vec_resize_##T( self, vector->size, 0 );                                                     \
        memmove( &self[ position + 1 ], &self[ position ], ( vector->size - position ) * sizeof ( T ) );    \
        self[ position ] = val;                                                                             \
    }                                                                                                       \
    return self;                                                                                            \
}                                                                                                           \
/* extend the vector by inserting an array of elements at the specified position */                         \
T* vec_insert_arr_##T( T* self, size_t position, T* arr, size_t size )                                      \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    if ( position > vector->size )                                                                          \
    {                                                                                                       \
        fprintf( stderr, "[ERRO]: index out of bounds, aborted\n" );                                        \
        exit(1);                                                                                            \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        vector->size += size;                                                                               \
        self = vec_resize_##T( self, vector->size, 0 );                                                     \
        memmove( &self[ position + size ], &self[ position ], ( vector->size - position ) * sizeof ( T ) ); \
        memmove( &self[ position ], arr, size * sizeof ( T ) );                                             \
    }                                                                                                       \
    return self;                                                                                            \
}                                                                                                           \
/* Adds a new element at the end of the vector */                                                           \
T* vec_push_back_##T( T* self, T val )                                                                      \
{                                                                                                           \
    return vec_insert_##T( self, vec_size_##T(self), val );                                                 \
}                                                                                                           \
/* Removes the last element in the vector, effectively reducing the container size by one */                \
T* vec_pop_back_##T( T* self )                                                                              \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    if ( !vector->size )                                                                                    \
    {                                                                                                       \
        fprintf( stderr, "[ERRO]: empty vector, aborted\n" );                                               \
        exit(1);                                                                                            \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        vector->size--;                                                                                     \
        vec_shrink_to_fit_##T( self );                                                                      \
        return self;                                                                                        \
    }                                                                                                       \
}                                                                                                           \
/* free the resources used by the container */                                                              \
void vec_destroy_##T( T* self )                                                                             \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( self );                                                            \
    free(vector);                                                                                           \
}                                                                                                           \

#endif  // VECTOR_IMPLEMENTATION