/** !
 * Video Graphics Array text driver
 * 
 * @file src/x86/VGA.h
 * 
 * @author Jacob Smith
 */

// Header guard
#pragma once

// Includes
#include "../glu/definitions.h"
#include "../glu/validation.h"

// VGA text colors
enum color_e
{
	BLACK         = 0,
	BLUE          = 1,
	GREEN         = 2,
	CYAN          = 3,
	RED           = 4,
	MAGENTA       = 5,
	BROWN         = 6,
	LIGHT_GREY    = 7,
	DARK_GREY     = 8,
	LIGHT_BLUE    = 9,
	LIGHT_GREEN   = 10,
	LIGHT_CYAN    = 11,
	LIGHT_RED     = 12,
	LIGHT_MAGENTA = 13,
	LIGHT_BROWN   = 14,
	WHITE         = 15,
};


void terminal_initialize ( void );

void terminal_setcolor ( u8 color );

void terminal_putentryat ( char c, u8 color, size_t x, size_t y );

int terminal_putchar ( void *, char c );

int terminal_write ( void*, const char* data, size_t size );

void terminal_writestring ( const char* data );
