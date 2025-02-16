/** !
 * Type definitions for glu
 * 
 * @file src/glu/definitions.h
 * 
 * @author Jacob Smith
 */

// Integral
typedef unsigned size_t;

// Integer
typedef unsigned  char      u8;
typedef signed    char      s8;
typedef           char      i8;
typedef unsigned  short     u16;
typedef signed    short     s16;
typedef           short     i16;
typedef unsigned  long      u32;
typedef signed    long      s32;
typedef           long      i32;
typedef unsigned  long long u64;
typedef signed    long long s64;
typedef           long long i64;

// Floating point 
typedef float  f32;
typedef double f64;

// Hash
typedef unsigned long long hash64;

// Opaque handles
typedef void *g_bump;
typedef void *g_teletype;

// Teletype
typedef struct g_teletype_create_info_s g_teletype_create_info;
typedef int (fn_teletype_putchar) (g_teletype teletype, char c);
typedef int (fn_teletype_write)   (g_teletype teletype, const char *p_string, size_t len);
