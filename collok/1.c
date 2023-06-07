#include <stdio.h>
#include <limits.h>


typedef unsigned int addr_t;
enum 
{
    SEG_BITS = 3,
    OFFSET_BITS = (sizeof(addr_t) * CHAR_BIT - SEG_BITS)
};


typedef struct segment 
{
    unsigned base;
    unsigned size;
} segment;


unsigned int 
VirtIntoPhys(segment *SegTable, unsigned int VirtAddr)
{
    unsigned int seg_number = (VirtAddr >> (sizeof(VirtAddr) * CHAR_BIT - 3));

    //unsigned int seg_offset = ((VirtAddr << 3u) >> 3u);
    unsigned int seg_offset = VirtAddr & (((1u << 29) - 1));

    segment *current_segment = &SegTable[seg_number];

    if (seg_offset >= current_segment->size) {
        eixt(25);
    }

    return current_segment->base + seg_offset;
} 
