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
T* vec_resize_##T( T* arr, size_t size, T val );                                                            \
T* vec_clear_##T( T* arr );                                                                                 \
T* vec_assign_##T( T* arr, size_t size, T val );                                                            \
T* vec_back_##T( T* arr );                                                                                  \
T* vec_shrink_to_fit_##T( T* arr );                                                                         \
T* vec_reserve_##T( T* arr, size_t size );                                                                  \
T* vec_insert_##T( T* arr, size_t position, T val );                                                        \
T* vec_insert_arr_##T( T* arr, size_t position, T* array, size_t size );                                    \
T* vec_push_back_##T( T* arr, T val );                                                                      \
T* vec_pop_back_##T( T* arr );                                                                              \
size_t vec_size_##T( T* arr );                                                                              \
size_t vec_capacity_##T( T* arr );                                                                          \
void vec_destroy_##T( T* arr );                                                                             \


// use `_Generic` keyword from C11 to wrap all of the function above
/*
#define vec_resize( arr, size, val ) _Generic( (arr),                                                       \
    int*:       vec_resize_int,                                                                             \
    char*:      vec_resize_char,                                                                            \
    float*:     vec_resize_float,                                                                           \
    double*:    vec_resize_double                                                                           \
)( arr, size, val )                                                                                         \
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
    vec_meta* vector = malloc( sizeof ( vec_meta ) + sizeof ( T ) * cap );                                  \
    vector->size = size;                                                                                    \
    vector->capacity = cap;                                                                                 \
    T* data = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                             \
    for ( size_t i = 0; i < size; i++ )                                                                     \
    {                                                                                                       \
        data[i] = val;                                                                                      \
    }                                                                                                       \
    return data;                                                                                            \
}                                                                                                           \
/* get the address for vec_meta */                                                                          \
static inline vec_meta* vec_get_meta_##T( T* arr )                                                          \
{                                                                                                           \
    return (void*) ( (char*) arr - sizeof ( vec_meta ) );                                                   \
}                                                                                                           \
/* change vector to specific size */                                                                        \
T* vec_resize_##T( T* arr, size_t size, T val )                                                             \
{                                                                                                           \
    vec_meta* vector = vec_get_meta_##T( arr );                                                             \
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
        arr = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                             \
        for ( size_t i = vector->size; i < size; i++ )                                                      \
        {                                                                                                   \
            arr[i] = val;                                                                                   \
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
        arr = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                             \
        vector->size = size;                                                                                \
    }                                                                                                       \
    return arr;                                                                                             \
}                                                                                                           \
/* clear, remove all elements in the vector, may not reallocate */                                          \
T* vec_clear_##T( T* arr )                                                                                  \
{                                                                                                           \
    vec_meta* vector = vec_get_meta_##T( arr );                                                             \
    vector->size = 0;                                                                                       \
    vector->capacity = 16;                                                                                  \
    vector = realloc( vector, sizeof ( vec_meta ) + sizeof ( T ) * vector->capacity );                      \
    arr = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                                 \
    memset( arr, 0, sizeof (T) * vector->capacity );                                                        \
    return arr;                                                                                             \
}                                                                                                           \
/* Assigns new contents to the vector and modifying its size accordingly */                                 \
T* vec_assign_##T( T* arr, size_t size, T val )                                                             \
{                                                                                                           \
    vec_meta* vector = vec_get_meta_##T( arr );                                                             \
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
        arr = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                             \
        for ( size_t i = 0; i < size; i++ )                                                                 \
        {                                                                                                   \
            arr[i] = val;                                                                                   \
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
        arr = (void*) ( (char*) vector + sizeof ( vec_meta ) );                                             \
        for ( size_t i = 0; i < size; i++ )                                                                 \
        {                                                                                                   \
            arr[i] = val;                                                                                   \
        }                                                                                                   \
        vector->size = size;                                                                                \
    }                                                                                                       \
    return arr;                                                                                             \
}                                                                                                           \
/* get the reference of the first item */                                                                   \
T* vec_front_##T( T* arr )                                                                                  \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    if ( vector->size > 0 ) return arr;                                                                     \
    else return NULL;                                                                                       \
}                                                                                                           \
/* get the reference of the last item */                                                                    \
T* vec_back_##T( T* arr )                                                                                   \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    if ( vector->size > 0 ) return &arr[ vector->size - 1 ];                                                \
    else return NULL;                                                                                       \
}                                                                                                           \
/* Returns the number of elements in the vector */                                                          \
size_t vec_size_##T( T* arr )                                                                               \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    return vector->size;                                                                                    \
}                                                                                                           \
/* get the allocated size for the vector in terms of elements */                                            \
size_t vec_capacity_##T( T* arr )                                                                           \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    return vector->capacity;                                                                                \
}                                                                                                           \
/* Requests the container to reduce its capacity to fit its size */                                         \
T* vec_shrink_to_fit_##T( T* arr )                                                                          \
{                                                                                                           \
    return vec_resize_##T( arr, vec_size_##T(arr), 0 );                                                     \
}                                                                                                           \
/* Requests that the vector capacity be at least enough to contain size elements */                         \
T* vec_reserve_##T( T* arr, size_t size )                                                                   \
{                                                                                                           \
    if ( size <= vec_size_##T( arr ) ) return arr;                                                          \
    else                                                                                                    \
    {                                                                                                       \
        return vec_resize_##T( arr, size, 0 );                                                              \
    }                                                                                                       \
}                                                                                                           \
/* extend the vector by inserting an element at the specified position */                                   \
T* vec_insert_##T( T* arr, size_t position, T val )                                                         \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    if ( position > vector->size )                                                                          \
    {                                                                                                       \
        fprintf( stderr, "[ERRO]: index out of bounds, aborted\n" );                                        \
        exit(1);                                                                                            \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        vector->size++;                                                                                     \
        arr = vec_resize_##T( arr, vector->size, 0 );                                                       \
        memmove( &arr[ position + 1 ], &arr[ position ], ( vector->size - position ) * sizeof ( T ) );      \
        arr[ position ] = val;                                                                              \
    }                                                                                                       \
    return arr;                                                                                             \
}                                                                                                           \
/* extend the vector by inserting an array of elements at the specified position */                         \
T* vec_insert_arr_##T( T* arr, size_t position, T* array, size_t size )                                     \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    if ( position > vector->size )                                                                          \
    {                                                                                                       \
        fprintf( stderr, "[ERRO]: index out of bounds, aborted\n" );                                        \
        exit(1);                                                                                            \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        vector->size += size;                                                                               \
        arr = vec_resize_##T( arr, vector->size, 0 );                                                       \
        memmove( &arr[ position + size ], &arr[ position ], ( vector->size - position ) * sizeof ( T ) );   \
        memmove( &arr[ position ], array, size * sizeof ( T ) );                                            \
    }                                                                                                       \
    return arr;                                                                                             \
}                                                                                                           \
/* Adds a new element at the end of the vector */                                                           \
T* vec_push_back_##T( T* arr, T val )                                                                       \
{                                                                                                           \
    return vec_insert_##T( arr, vec_size_##T(arr), val );                                                   \
}                                                                                                           \
/* Removes the last element in the vector, effectively reducing the container size by one */                \
T* vec_pop_back_##T( T* arr )                                                                               \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    if ( !vector->size )                                                                                    \
    {                                                                                                       \
        fprintf( stderr, "[ERRO]: empty vector, aborted\n" );                                               \
        exit(1);                                                                                            \
    }                                                                                                       \
    else                                                                                                    \
    {                                                                                                       \
        vector->size--;                                                                                     \
        vec_shrink_to_fit_##T( arr );                                                                       \
        return arr;                                                                                         \
    }                                                                                                       \
}                                                                                                           \
/* free the resources used by the container */                                                              \
void vec_destroy_##T( T* arr )                                                                              \
{                                                                                                           \
    vec_meta *vector = vec_get_meta_##T( arr );                                                             \
    free(vector);                                                                                           \
}                                                                                                           \

#endif  // VECTOR_IMPLEMENTATION