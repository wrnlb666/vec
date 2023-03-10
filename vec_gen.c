#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define VECTOR_IMPLEMENTATION
#include "vector.h"


typedef struct cstr
{
    char* cstr;
    size_t length;
} cstr;


using_vector( cstr )



int main( int argc, char** argv )
{
    if ( argc == 1 )
    {
        fprintf( stderr, "[usage]: %s <types> [optional... output name]\n", argv[0] );
        exit(1);
    }

    cstr* types = vec_new_cstr( 0, (cstr) { 0 } );
    cstr target = { .length = 3, .cstr = "vec" };
    
    for ( int i = 1; i < argc; i++ )
    {
        char* curr = argv[i];
        if ( curr[0] != '-' )
        {
            vec_push_back_cstr( types, (cstr) { .length = strlen(curr), .cstr = curr } );
            continue;
        }
        if ( curr[1] == 'o' )
        {
            curr = argv[++i];
            if ( curr != NULL )
            {
                target.length = strlen( curr );
                target.cstr = curr;
                continue;
            }
            fprintf( stderr, "fatel error: output file flag specified but no file name given\n" );
            vec_destroy_cstr( types );
            exit(1);
        }
        if ( curr[1] == 'h' )
        {
            curr++;
            // print man page
            fprintf( stderr, "[usage]: ./%s <type>\n", argv[0] );
            fprintf( stderr, "    optional arguments:\n" );
            fprintf( stderr, "          -h          print this manual\n" );
            fprintf( stderr, "          -o          specify output file name, will generage \"*.c\" and \"*.h\" file. \n" );
            vec_destroy_cstr( types );
            exit(0);
        }
    }
    char* file_c = malloc( target.length + 3 );
    char* file_h = malloc( target.length + 3 );
    strcpy( file_c, target.cstr );
    strcat( file_c, ".c" );
    strcpy( file_h, target.cstr );
    strcat( file_h, ".h" );
    
    FILE* fpc = fopen( file_c, "w+" );
    FILE* fph = fopen( file_h, "w+" );

    free( file_c );
    free( file_h );

    size_t size = vec_size_cstr( types );

    // include header .h
    fprintf( fph, "#include \"vector.h\"\n\n" );

    // include header .c
    fprintf( fpc, "#define VECTOR_IMPLEMENTATION\n" );
    fprintf( fpc, "#include \"vector.h\"\n\n" );


    // add using types
    for ( size_t i = 0; i < size; i++ )
    {
        fprintf( fph, "using_vector(%s)\n", types[i].cstr );
        fprintf( fpc, "using_vector(%s)\n", types[i].cstr );
        // printf( "%s\n", types[i].cstr );
    }
    fprintf( fph, "\n" ); 


    // add macros for .h file

    // T* vec_resize( T* self, size_t size, T val );
    fprintf( fph, "// T* vec_resize( T* self, size_t size, T val );\n" );
    fprintf( fph, "#define vec_resize( self, size, val ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_resize_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_resize_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self, size, val ) \\\n\n" );

    // T* vec_clear( T* self );
    fprintf( fph, "// T* vec_clear( T* self );\n" );
    fprintf( fph, "#define vec_clear( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_clear_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_clear_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );

    // T* vec_assign( T* self, size_t size, T val );
    fprintf( fph, "// T* vec_assign( T* self, size_t size, T val );\n" );
    fprintf( fph, "#define vec_assign( self, size, val ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_assign_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_assign_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self, size, val ) \\\n\n" );

    // T* vec_back( T* self );
    fprintf( fph, "// T* vec_back( T* self );\n" );
    fprintf( fph, "#define vec_back( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_back_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_back_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );

    // T* vec_shrink_to_fit( T* self );
    fprintf( fph, "// T* vec_shrink_to_fit( T* self );\n" );
    fprintf( fph, "#define vec_shrink_to_fit( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_shrink_to_fit_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_shrink_to_fit_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );

    // T* vec_reserve( T* self, size_t size );
    fprintf( fph, "// T* vec_reserve( T* self );\n" );
    fprintf( fph, "#define vec_reserve( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_reserve_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_reserve_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self, size ) \\\n\n" );

    // T* vec_insert( T* self, size_t position, T val );
    fprintf( fph, "// T* vec_insert( T* self, size_t position, T val );\n" );
    fprintf( fph, "#define vec_insert( self, position, val ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_insert_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_insert_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self, position, val ) \\\n\n" );

    // T* vec_append( T* self, T val );
    fprintf( fph, "// T* vec_append( T* self, T val );\n" );
    fprintf( fph, "#define vec_append( self, val ) vec_push_back( self, val )\n" );

    // T* vec_push_back( T* self, T val );
    fprintf( fph, "// T* vec_push_back( T* self, T val );\n" );
    fprintf( fph, "#define vec_push_back( self, val ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_push_back_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_push_back_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self, val ) \\\n\n" );

    // T* vec_pop_back( T* self );
    fprintf( fph, "// T* vec_pop_back( T* self );\n" );
    fprintf( fph, "#define vec_pop_back( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_pop_back_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_pop_back_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );

    // size_t vec_size( T* self );
    fprintf( fph, "// size_t vec_size( T* self );\n" );
    fprintf( fph, "#define vec_size( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_size_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_size_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );

    // size_t vec_capacity( T* self );
    fprintf( fph, "// size_t vec_capacity( T* self );\n" );
    fprintf( fph, "#define vec_capacity( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_capacity_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_capacity_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );


    // void vec_free( T* self );
    fprintf( fph, "// void vec_free( T* self )\n" );
    fprintf( fph, "#define vec_free( self ) vec_destroy( self )\n" );
    // void vec_destroy( T* self );
    fprintf( fph, "// void vec_destroy_( T* self );\n" );
    fprintf( fph, "#define vec_destroy( self ) _Generic( (self), \\\n" );
    for ( size_t i = 0; i < size - 1; i++ )
    {
        fprintf( fph, "    %s*: vec_destroy_%s, \\\n", types[i].cstr, types[i].cstr );
    }
    fprintf( fph, "    %s*: vec_destroy_%s \\\n", types[size-1].cstr, types[size-1].cstr );
    fprintf( fph, ")( self ) \\\n\n" );


    fclose( fpc );
    fclose( fph );
    vec_destroy_cstr( types );

    return 0;
}


#if 0   // example output file

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

#define vec_append( self, val ) vec_push_back( self, val )
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

#endif  // example
