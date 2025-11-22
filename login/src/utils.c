#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <ctype.h>

int is_empty_advanced(const char *s)
{
    if (!s)
        return 1;

    while (*s)
    {
        unsigned char c = (unsigned char)*s;
        if (isprint(c))
            return 0;
        s++;
    }
    return 1;
}