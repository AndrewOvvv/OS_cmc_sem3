int
get_block_size(unsigned start_block, unsigned *FAT)
{
    //check FAT_size > start_block
    int size = 0;
    while (start_block != 0) {
        start_block = FAT[start_block];
        ++size;
    }
    return size;
}
