# vec
an array like vec that is kind of like what we have in cpp

### API
```c
// resize the vector to specific size, low level control that would be completed by the library
T* vec_resize( T* self, size_t size, T val );

// clear the vector which may or may not reallocate
T* vec_clear( T* self );

// effectively clear the vector, and change everything to val * size
T* vec_assign( T* self, size_t size, T val );

// return the last element of the vector
T* vec_back( T* self );

// automatic resize without information lost
T* vec_shrink_to_fit( T* self );

// reserve memory that can hold 'size' elements
T* vec_reserve( T* self, size_t size );

// insert element at index 'position'
T* vec_insert( T* self, size_t position, T val );

// append 'val' to the end of the vector, resize will be automatic
T* vec_append( T* self, T val );

// append is an alias of 'vec_push_back'
T* vec_push_back( T* self, T val );

// pop the last element, return its value
T* vec_pop_back( T* self );

// get the size of the vector
size_t vec_size( T* self );

// get the capacity of the vector
size_t vec_capacity( T* self );

// free the vector
void vec_free( T* self );

// 'vec_free' is an alias of vec_destroy
void vec_destroy_( T* self );


```