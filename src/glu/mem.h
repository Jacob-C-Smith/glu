/** !
 * Allocators
 * 
 * @file src/glu/mem.h
 * 
 * @author Jacob Smith
 */

// Header guard
#pragma once

// Include 
#include "definitions.h"
#include "io.h"

/** !
 * Construct a bump allocator
 * 
 * @param p_bump pointer to result
 * @param base   the base address of the bump allocator
 * @param size   the size of the pool
 * 
 * @return 1 on success, 0 on error 
 */
int bump_construct ( g_bump *p_bump, void *base, size_t size );

int memory_map_from_multiboot_header ( void *p_multiboot_info );

/** !
 * Allocate memory from a bump allocator
 * 
 * @param bump      the bump allocator
 * @param pp_result pointer to result
 * @param size      the quantity of bytes to be allocated
 * 
 * @return 1 on success, 0 on error 
 */
int bump_alloc ( g_bump bump, void **pp_result, size_t size );

/** !
 * Allocate memory
 * 
 * @param pp_result pointer to result
 * @param size      the quantity of bytes to be allocated
 * 
 * @return 1 on success, 0 on error 
 */
int alloc ( void **pp_result, size_t size );

/** !
 * Destroy a bump allocator
 * 
 * @param p_bump pointer to the bump allocator
 * 
 * @return 1 on success, 0 on error 
 */
int bump_destroy ( g_bump *p_bump );
