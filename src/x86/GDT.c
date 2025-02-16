#include "GDT.h"

#include "../glu/io.h"

static const char *gdt_flag_granularity_strings [] = 
{
    [GDT_FLAG_GRANULARITY_BYTE] = "byte",
    [GDT_FLAG_GRANULARITY_PAGE] = "page"
};

static const char *gdt_flag_mode_strings [] =
{
    [GDT_FLAG_MODE_16_BIT]  = "16-bit",
    [GDT_FLAG_MODE_32_BIT]  = "32-bit",
    [GDT_FLAG_MODE_64_BIT]  = "64-bit",
    [GDT_FLAG_MODE_INVALID] = "invalid"
};

static const char *gdt_access_present_strings [] = 
{
    [SEGMENT_ACCESS_ABSENT]  = "absent",
    [SEGMENT_ACCESS_PRESENT] = "present"
};

static const char *gdt_access_privilege_strings [] = 
{
    [SEGMENT_ACCESS_PRIVILEGE_KERNEL] = "kernel",
    [SEGMENT_ACCESS_PRIVILEGE_USER]   = "user"
};

static const char *gdt_access_type_strings [] = 
{
    [SEGMENT_ACCESS_TYPE_TASK_STATE] = "task state",
    [SEGMENT_ACCESS_TYPE_CODE_DATA]  = "code/data"
};


static const char *gdt_access_executable_strings [] = 
{
    [SEGMENT_ACCESS_CODE_DATA_DATA] = "data",
    [SEGMENT_ACCESS_CODE_DATA_CODE] = "code"
};

static const char *gdt_access_direction_strings [] = 
{
    [SEGMENT_ACCESS_DATA_GROWS_UP]   = "grows up",
    [SEGMENT_ACCESS_DATA_GROWS_DOWN] = "grows down"
};

static const char *gdt_access_conforming_strings [] = 
{
    [SEGMENT_ACCESS_CODE_EQUAL_PRIVILEGE] = "equal privilege",
    [SEGMENT_ACCESS_CODE_LOWER_PRIVILEGE] = "lower privilege"
};

static const char *gdt_access_code_read_write_strings [] = 
{
    [SEGMENT_ACCESS_CODE_READ_LOCK]  = "read lock",
    [SEGMENT_ACCESS_CODE_READ_ALLOW] = "readable"
};

static const char *gdt_access_data_read_write_strings [] = 
{
    [SEGMENT_ACCESS_DATA_WRITE_LOCK]  = "read only",
    [SEGMENT_ACCESS_DATA_WRITE_ALLOW] = "read/write"
};

static const char *gdt_access_accessed_strings [] = 
{
    [SEGMENT_ACCESS_ACCESSED]   = "accessed",
    [SEGMENT_ACCESS_UNACCESSED] = "unaccessed"
};

int gdt_segment_descriptor_construct
(
    gdt_segment_descriptor *p_gdt_segment_descriptor,
    void                   *p_base,
    void                   *p_limit,
    enum gdt_flag_bits_e   access,
    enum gdt_flag_bits_e   flags
)
{

    // Populate the struct
    *p_gdt_segment_descriptor = (gdt_segment_descriptor)
    {
        .p_base  = p_base,
        .p_limit = p_limit,
        .access  = access,
        .flags   = flags
    };

    // Success
    return 1;
}

int gdt_segment_descriptor_print ( gdt_segment_descriptor *p_gdt_segment_descriptor )
{

    // Initialized data
    u8 flags  = p_gdt_segment_descriptor->flags,
       access = p_gdt_segment_descriptor->access;

    // Formatting
    format("=== x86 GDT segment descriptor @ 0x%x ===\n", p_gdt_segment_descriptor);
    
    // Edge case
    if
    (
        (
            p_gdt_segment_descriptor->p_base  ||
            p_gdt_segment_descriptor->p_limit ||
            p_gdt_segment_descriptor->access  ||
            p_gdt_segment_descriptor->flags 
        ) == 0
    ) goto null_descriptor;

    // Formatting
    format("\trange  : [ 0x%x - 0x%x ]\n", p_gdt_segment_descriptor->p_base, (size_t)p_gdt_segment_descriptor->p_limit << (12 * ((flags & GDT_FLAG_GRANULARITY_PAGE) > 0)));

    // Access qualities
    {

        // Prologue
        format("\taccess : [ ");

        // Print the present flag
        format("%s | ", gdt_access_present_strings[( access & SEGMENT_ACCESS_PRESENT_MASK )]);
        
        // Print the privilege level flag
        format("%s | ", gdt_access_privilege_strings[( access & SEGMENT_ACCESS_PRIVILEGE_MASK )]);
        
        // Print the type flag
        format("%s | ", gdt_access_type_strings[( access & SEGMENT_ACCESS_TYPE_MASK )]);
        
        // Print the executable flag
        format("%s | ", gdt_access_executable_strings[( access & SEGMENT_ACCESS_CODE_DATA_MASK )]);
        
        // Print the conforming/direction flag
        if ( access & SEGMENT_ACCESS_CODE_DATA_MASK )
            
            // Print the conforming flag
            format("%s | ", gdt_access_conforming_strings[( access & SEGMENT_ACCESS_CONFORMING_DIRECTION_MASK )]);

        else
            // Print the direction flag
            format("%s | ", gdt_access_direction_strings[( access & SEGMENT_ACCESS_CONFORMING_DIRECTION_MASK )]);

        // Print the read/write flag
        if ( access & SEGMENT_ACCESS_CODE_DATA_MASK )
            
            // Print the read flag
            format("%s | ", gdt_access_code_read_write_strings[( access & SEGMENT_ACCESS_READ_WRITE_MASK )]);

        else
            // Print the read/write flag
            format("%s | ", gdt_access_data_read_write_strings[( access & SEGMENT_ACCESS_READ_WRITE_MASK )]);
        
        // Print the accessed flag
        format("%s", gdt_access_accessed_strings[( access & SEGMENT_ACCESS_ACCESSED_MASK)]);
          
        // Epilogue
        format(" ]\n");
    }
    
    // Flags
    {

        // Prologue
        format("\tflags : [ ");

        // Print the granularity
        format("%s | ", gdt_flag_granularity_strings[( flags & GDT_FLAG_GRANULARITY_MASK )]);
        
        // Print the mode
        format("%s", gdt_flag_mode_strings[( flags & GDT_FLAG_MODE_MASK )]);

        // Epilogue
        format(" ]\n");
    }

    // Success
    return 1;

    null_descriptor:
        format("\t[NULL]\n", p_gdt_segment_descriptor);
        return 1;
}
