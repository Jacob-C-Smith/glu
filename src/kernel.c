#include "glu/definitions.h"
#include "glu/mem.h"
#include "glu/io.h"

#include "x86/VGA.h"
#include "x86/GDT.h"

void kernel_early ( void )
{

	// Initialize the allocator
	alloc(0, 0);

	// Initialize standard out
	write(0, 0);

	// Initialize VGA teletype
	terminal_initialize();

	// Initialize the global descriptor table
	{
		gdt_segment_descriptor *p_gdt = (void *) 0;

		alloc((void **)&p_gdt, 5 * sizeof(gdt_segment_descriptor));

		gdt_segment_descriptor_construct(
			&p_gdt[0], 
			(void *) 0x0, 
			(void *) 0x0,
			0,
			0
		);

		gdt_segment_descriptor_construct(
			&p_gdt[1], 
			(void *) 0x00000000, 
			(void *) 0xFFFFF,
			SEGMENT_ACCESS_KERNEL_CODE,
			GDT_FLAG_MODE_32_BIT | GDT_FLAG_GRANULARITY_PAGE
		);

		gdt_segment_descriptor_construct(
			&p_gdt[2], 
			(void *) 0x00000000, 
			(void *) 0xFFFFF,
			SEGMENT_ACCESS_KERNEL_DATA,
			GDT_FLAG_MODE_32_BIT | GDT_FLAG_GRANULARITY_PAGE
		);

		gdt_segment_descriptor_construct(
			&p_gdt[3], 
			(void *) 0x00000000, 
			(void *) 0xFFFFF,
			SEGMENT_ACCESS_USER_CODE,
			GDT_FLAG_MODE_32_BIT | GDT_FLAG_GRANULARITY_PAGE
		);

		gdt_segment_descriptor_construct(
			&p_gdt[4], 
			(void *) 0x00000000, 
			(void *) 0xFFFFF,
			SEGMENT_ACCESS_USER_DATA,
			GDT_FLAG_MODE_32_BIT | GDT_FLAG_GRANULARITY_PAGE
		);

		for (size_t i = 0; i < 5; i++)
			gdt_segment_descriptor_print(&p_gdt[i]), putchar('\n');
	}
}

void kernel_main ( void *a, void *b, void *c, void *d )  
{

	// Initialize the kernel
	kernel_early();

	// Initialized data
	format("Hello, %s!\n", "World");

	// Error
	return;
}
