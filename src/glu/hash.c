// Header
#include "hash.h"

// Function definitions
hash64 hash_crc64 ( const void *const k, size_t l )
{

    // Argument check
    if ( k == (void *) 0 ) goto no_k;

    // External data
    extern unsigned long long crc64_table[];

    // Initialized data
    const unsigned char *p = k;
    hash64 h = 0xFFFFFFFFFFFFFFFF;

    // Iterate through the input
    for (size_t i = 0; i < l; i++)

        // Update the CRC
        h = crc64_table[(h ^ p[i]) & 0xFF] ^ (h >> 8);

    // Success
    return h ^ 0xFFFFFFFFFFFFFFFF;

    // Error handling
    no_k: return 0;
}

