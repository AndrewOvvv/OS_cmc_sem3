int
is_used_data_block(char *bit_array, int block_number, int max_block_number) 
{
    if (0 <= block_number && block_number <= max_block_number) {
        int byte_number = block_number / CHAR_BIT;
        int bit_in_byte = block_number % CHAR_BIT;
        
        if (((1 << bit_in_byte) & (unsigned char)bit_array[byte_number])) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return -1;
    }
}
