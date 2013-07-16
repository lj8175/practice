#include "buffer.h"
#include <string.h>
#include <stdio.h>

#define NUM_FIRST 5
#define NUM_SECOND 10

int main()
{
    CCharBuffer buf;
    buf.SetCapacity(NUM_FIRST);
    strncpy(buf, "abcdefghigklmnopq", NUM_FIRST);
    buf[NUM_FIRST-1] = 0;
    printf("size:%d\tcontent:%s\n", buf.GetCapacity(), (char*)buf);

    buf.ExpandIfNeed(NUM_SECOND);
    strncpy(buf, "abcdefghigklmnopq", NUM_SECOND);
    buf[NUM_SECOND-1] = 0;
    printf("size:%d\tcontent:%s\n", buf.GetCapacity(), (char*)buf);

}
