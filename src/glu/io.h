/** !
 * I/O interface 
 * 
 * @file src/glu/ios.h
 * 
 * @author Jacob Smith
 */

// Header guard
#pragma once 

// Include
#include "definitions.h"
#include "teletype.h"
#include "variadic.h"

// x86
#include "../x86/VGA.h"

/** !
 * Get standard out
 * 
 * @param p_result the result
 * 
 * @return 1 on success, 0 on error
 */
int standard_out_get ( g_teletype *p_result );

/** !
 * Write a single character to standard out
 * 
 * @param c the character
 * 
 * @return 1 on success, 0 on error
 */
int putchar ( char c );

/** !
 * Write size bytes from p_str to standard out
 * 
 * @param p_str pointer to the bytes to write
 * @param size  the quantity of bytes to write
 * 
 * @return 1 on success, 0 on error
 */
int write ( const char *p_str, size_t size );

/** !
 * Formatted text output
 * 
 * @param format the format string
 * @param ...    variadic arguments
 * 
 * @return 1 on success, 0 on error
 */
int format ( const char *format, ... );