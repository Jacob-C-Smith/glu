#include "io.h"

static char _digit_lut[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
};
static g_teletype standard_out = (void *) 0;
static const g_teletype_create_info _defaults = (g_teletype_create_info)
{
    .pfn_tty_putchar = terminal_putchar,
    .pfn_tty_write   = terminal_write 
};

size_t strlen(const char* str) 
{

    // Initialized data
	size_t len = 0;

    // Count to len
	while ( str[len++] );

    // Success
	return len;
}

static void singleton_check ( void ) 
{
    
    // Singleton
    if ( standard_out == (void *) 0 )
        teletype_construct(&standard_out, _defaults);
    
    // Done
    return;
}

int standard_out_get ( g_teletype *p_result )
{

    // Return a pointer to the caller
    *p_result = standard_out;

    // Success
    return 1;
}

int putchar ( char c )
{

    // State check
    singleton_check();
    
    // Pass
    return teletype_putchar(standard_out, c);
}

int write ( const char *p_str, size_t size )
{

    // State check
    singleton_check();
    
    // Pass
    return teletype_write(standard_out, p_str, size);
}

int format ( const char *format, ... )
{
    
    // State check
    singleton_check();

    // Initialized data
    va_list list;
    size_t read = 0;

    // Construct the variadic list
    va_start(list, format);
    
    // Print the format string
    while ( format[read] )
    {

        // Parse format characters
        if ( format[read] != '%' )
        {
            putchar(format[read++]);

            continue;
        }

        // Increment the index
        read++;

        // Strategy
        switch ( format[read] )
        {
            case 'c':
            {

                // Initialized data
                char c = va_arg(list, int);
                
                putchar(c), read++;
                
                break;
            }

            case 's':
            {

                // Initialized data
                const char *s = va_arg(list, const char *);
                
                write(s, strlen(s)-1), read++;
                
                break;
            }
            
            case 'x':
            {

                // Initialized data
                unsigned int x = va_arg(list, unsigned int);
                
                for (int i = 7; i > -1; i--)
                    putchar(_digit_lut[(x & (0xf << (i << 2))) >> (i << 2)]);
                
                read++;
                
                break;
            }
        }
	}

    // Done with variadic list
    va_end(list);

    // Success
    return 1;
}
