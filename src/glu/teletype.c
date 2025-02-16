#include "teletype.h"


// Structure definitions
struct g_teletype_s
{
    fn_teletype_putchar *pfn_tty_putchar;
    fn_teletype_write   *pfn_tty_write;
};

// Type definitions
typedef struct g_teletype_s g_teletype_t;

int teletype_construct ( g_teletype *p_teletype, g_teletype_create_info _g_teletype_create_info )
{

    // Argument check
    if ( p_teletype == (void *) 0 ) goto no_teletype;

    // Initialized data
    g_teletype_t *p_result = (void *) 0;

    // Allocate a teletype 
    alloc((void *)&p_result, sizeof(g_teletype_t));

    // Construct the allocator
    *p_result = (g_teletype_t)
    {
        .pfn_tty_putchar = _g_teletype_create_info.pfn_tty_putchar,
        .pfn_tty_write   = _g_teletype_create_info.pfn_tty_write,
    };

    // Return the result to the caller
    *p_teletype = p_result;

    // Success
    return 1;

    // Error handling
    no_teletype:
        return 0;
}

int teletype_putchar ( g_teletype teletype, char c )
{

    // Argument check
    if ( teletype == (void *) 0 ) goto no_teletype;

    // Initialized data
    g_teletype_t *p_teletype = (g_teletype_t *) teletype;

    // Pass
    return p_teletype->pfn_tty_putchar(p_teletype, c);

    // Error handling
    no_teletype: return 0;
}

int teletype_write ( g_teletype teletype, const char *p_string, size_t len )
{

    // Argument check
    if ( teletype == (void *) 0 ) goto no_teletype;

    // Initialized data
    g_teletype_t *p_teletype = (g_teletype_t *) teletype;

    // Pass
    return p_teletype->pfn_tty_write(p_teletype, p_string, len);

    // Error handling
    no_teletype: return 0;
}

// int teletype_destroy ( g_bump *p_bump );
