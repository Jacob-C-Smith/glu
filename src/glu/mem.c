#include "mem.h"

// Structure definitions
struct g_bump_s
{
    size_t size, used;
    u8     _pool[];
};

// Type definitions
typedef struct g_bump_s g_bump_t;

static int initialized = 0;
static g_bump allocator = (void *) 0;

static void singleton_check ( void ) 
{
    
    // Singleton
    if ( initialized == 0 )
        bump_construct((g_bump *)&allocator, (void *)0x2FFFF0, 4*1024*1024);
    
    initialized = 1;

    // Done
    return;
}

// Function definitions
int bump_construct ( g_bump *p_bump, void *base, size_t size )
{

    // Argument check
    if ( p_bump == (void *) 0 ) goto no_bump;

    // Initialized data
    g_bump_t *p_result = (g_bump_t *) base;

    // Construct the allocator
    *p_result = (g_bump_t)
    {
        .size = size,
        .used = 0,
    };

    // Return the result to the caller
    *p_bump = p_result;

    // Success
    return 1;

    // Error handling
    no_bump:
        return 0;
}

int memory_map_from_multiboot_header ( void *p_multiboot_info )
{

    // Initialized data
    u32   mmap_size = *(u32*)    p_multiboot_info + 44;
    void *mmap_addr = *(void **) p_multiboot_info + 48;

    // Success
    return 1;
}

int bump_alloc ( g_bump bump, void **pp_result, size_t size )
{
    
    // Argument check
    if ( bump      == (void *) 0 ) goto no_bump;
    if ( pp_result == (void *) 0 ) goto no_result;

    // Initialized data
    g_bump_t *p_bump   = (g_bump_t *) bump;
    void     *p_result = (void *) 0;

    // Free?
    if ( size == 0 ) goto release_all;

    // State check
    if ( size > (p_bump->size - p_bump->used) ) goto no_mem;

    // Compute the result
    p_result = &p_bump->_pool[p_bump->used];

    // Update the quantity of allocated memory
    p_bump->used += size;

    // Return a pointer to the caller
    *pp_result = p_result;

    done:

    // Success
    return 1;

    // This branch free()'s all previous allocations
    release_all: 

        // Reset the bump
        p_bump->used = 0;

        // Done
        goto done;

    // Error handling
    no_bump:   return 0;
    no_result: return 0;
    no_mem:    return 0;
}

int alloc ( void **pp_result, size_t size )
{

    // State check
    singleton_check();

    // Allocate memory
    bump_alloc(allocator, pp_result, size);
}

int bump_destroy ( g_bump *p_bump )
{

    // Argument check
    if ( p_bump == (void *) 0 ) goto no_bump;

    // Initialized data
    g_bump_t *bump = *(g_bump_t **) p_bump;

    // No more pointer for caller
    *p_bump = (void *) 0;

    // Success
    return 1;

    // Error handling
    no_bump:
        return 0;
}