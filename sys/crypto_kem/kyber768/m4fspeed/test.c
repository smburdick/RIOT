#include <stdio.h>

int main(void)
{
    # if defined(__linux__) && defined(__GLIBC__) && ((__GLIBC__ > 2) || (__GLIBC_MINOR__ > 24))
        puts("check!");
    #endif
}
