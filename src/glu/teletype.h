/** !
 * Teletype interface
 * 
 * @file src/glu/mem.h
 * 
 * @author Jacob Smith
 */

// Header guard
#pragma once

// Include 
#include "definitions.h"
#include "mem.h"

// Structure definitions
struct g_teletype_create_info_s
{
    fn_teletype_putchar *pfn_tty_putchar;
    fn_teletype_write   *pfn_tty_write;
};

/** !
 * Construct a 
 * 
 * @param p_teletype
 * @param _g_teletype_create_info
 * 
 * @return 1 on success, 0 on error 
 */
int teletype_construct ( g_teletype *p_teletype, g_teletype_create_info _g_teletype_create_info );

/** !
 * 
 * @return 1 on success, 0 on error 
 */
int teletype_putchar ( g_teletype _teletype, char c );

/** !
 * 
 * @return 1 on success, 0 on error 
 */
int teletype_write ( g_teletype teletype, const char *p_string, size_t len );

/** !
 * Destroy a 
 * 
 * @return 1 on success, 0 on error 
 */
int teletype_destroy ( g_teletype *p_teletype );
