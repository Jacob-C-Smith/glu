/** !
 * 
 * 
 * 
 * 
 */

#include "VGA.h"

#define vga_entry_color(fg, bg) ((u8)(fg | bg << 4))
#define TERMINAL_TAB_WIDTH 4

static inline u16 vga_entry ( u8 c, u8 color ) 
{
	return (u16) c | (u16) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static size_t terminal_row = 0;
static size_t terminal_column = 0;
static u8 terminal_color = vga_entry_color(LIGHT_GREY, BLACK);;
static u16* terminal_buffer = (u16*) 0xB8000;

void terminal_initialize ( void ) 
{

	// Initialize the teletype
	for (size_t y = 0; y < VGA_HEIGHT; y++) 
		for (size_t x = 0; x < VGA_WIDTH; x++) 
			terminal_buffer[( y * VGA_WIDTH + x)] = vga_entry(' ', terminal_color);

	// Done
	return;
}

void terminal_setcolor ( u8 color )  { terminal_color = color; }

void terminal_putentryat ( char c, u8 color, size_t x, size_t y ) 
{
	
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = vga_entry(c, color);
}

int terminal_putchar ( void *, char c ) 
{
	switch (c)
	{
		case '\n':
			terminal_column = 0,
			terminal_row++;
			return 1;
		
		case '\t':
			terminal_column += TERMINAL_TAB_WIDTH;
			return 1;
		
		case '\r':
			terminal_column = 0;
			return 1;
		
		default:
			break;
	}
	

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}

	return 1;
}

int terminal_write ( void*, const char* data, size_t size ) 
{
	size_t i = 0;

	for (; i < size; i++) 
		validate(terminal_putchar(0, data[i]), failed_to_print_char);

	// Printed i characters
	return i;
	
	// Error handling
	failed_to_print_char: return 0;
}
