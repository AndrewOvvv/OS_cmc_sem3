#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>

enum
{
    FIRST_VALID_INDEX = 2,
    LAST_VALID_INDEX = 0xffd,
    EOF_INDEX = 0xfff,
    TOTAL_INDICES = 0x1000,
    BLOCK_SIZE = 512,
};


uint16_t peek(uint8_t* data, uint16_t which)
{
    if(which < FIRST_VALID_INDEX || which > LAST_VALID_INDEX)
        return EOF_INDEX;
    uint8_t* chk_p = data + 3*(which/2);
    uint16_t ans;
    if(which % 2)
        ans = (chk_p[1] >> 4) | (chk_p[2] << 4);
    else
        ans = ((chk_p[1] & 15) << 8) | chk_p[0];
    if(ans < FIRST_VALID_INDEX || ans > LAST_VALID_INDEX)
        ans = EOF_INDEX;
    return ans;
}


int 
main(int argc, const char** argv)
{
    if (argc < 2)
        return 1;
    int fd = open(argv[1], O_RDONLY);
    if(fd < 0)
        return 1;
    off_t o = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    if(o > 3*TOTAL_INDICES/2)
        o = 3*TOTAL_INDICES/2;
    char array[3*TOTAL_INDICES/2] = {0};
    char* p = array;
    char* endp = array + sizeof(array) / sizeof(*array);
    while(p < endp)
    {
        ssize_t chk = read(fd, p, endp-p);
        if(chk <= 0)
        {
            close(fd);
            return 1;
        }
        p += chk;
    }
    close(fd);
    uint16_t start_block = 0;
    uint32_t file_offset = 0;
    scanf("%" PRIu16 "d%" PRIu32 "d", &start_block, &file_offset);
    while(file_offset >= BLOCK_SIZE)
    {
        file_offset -= BLOCK_SIZE;
        start_block = peek(array, start_block);
    }
    if(start_block == EOF_INDEX)
        printf("0 0\n");
    else
        printf("%" PRIu16 "d %" PRIu32 "d\n", start_block, file_offset);
    return 0;
}
