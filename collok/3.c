#include <fcntl.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

enum
{
    OFFSET_BITS = 9,
    PAGE_BITS = 23,
    NO_PAGE = 0xffffffff
};

unsigned 
To_PA(unsigned *Page_Tab, unsigned Virt_A, int PID)
{
    unsigned page = Virt_A >> OFFSET_BITS;
    if (Page_Tab[page] == NO_PAGE) {
        kill(PID, SIGPL);
        exit(NO_PAGE);
    }

    unsigned base = Page_Tab[page];
    unsigned offset = ((Virt_A << PAGE_BITS) >> PAGE_BITS);

    return base + offset;

}
