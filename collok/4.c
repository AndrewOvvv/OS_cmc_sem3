int 
IsUsed(unsigned char *arr, int blk, int limit)
{
    if (blk < 0 || limit < blk) {
        return -1;
    }
    return ((arr[blk / 8] >> (blk % 8)) & 1);
}
