#include <stdint.h>


enum
{
    PRESENT_MASK = 1,
    WRITE_MASK = 2,
    EXECUTE_MASK = 4,
    PERMISSIONS_MASK = WRITE_MASK | EXECUTE_MASK,
    ADDR_MASK = 0x3fffc00,
    VM_SIZE = 0x4000000,
    MEMORY_SIZE = 0x4000000,
    FIRST_SHIFT = 18,
    SECOND_SHIFT = 10,
    IDX_MASK = 0xff,
};


int
lookup(
        void* memory, 
        unsigned int pt, 
        unsigned int va, 
        int mode, 
        unsigned int* pa)
{
    if (va >= VM_SIZE) {
        return 0; 
    }

    char* p_memory = memory;
    unsigned int idx1 = (va >> FIRST_SHIFT) & IDX_MASK;
    if (pt + 4 * idx1 + 4 > MEMORY_SIZE) {
        return 0;
    }

    unsigned int pd = *(uint32_t*)(p_memory + pt + 4 * idx1);
    if (!(pd & PRESENT_MASK)) {
        return 0;
    }

    pd &= ADDR_MASK;
    unsigned int idx2 = (va >> SECOND_SHIFT) & IDX_MASK;
    unsigned int pte = *(uint32_t*)(p_memory + pd + 4 * idx2);
    if (!(pte & PRESENT_MASK)) {
        return 0;
    }
    if ((pte & mode & PERMISSIONS_MASK) != (mode & PERMISSIONS_MASK)) {
        return 0;
    }
    *pa = (pte & ADDR_MASK) | (va & (~ADDR_MASK));
    return 1;
}
