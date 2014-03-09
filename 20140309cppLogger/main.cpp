#include "log.h"
#include <time.h>

int main()
{

    PUSH_TRACE("hello from logger~");

    push_log_init(".", LEVEL_TRACE, "cpplogger");
    PUSH_TRACE("hello from logger[%d]", time(NULL));
    push_log_close();
    return 0;
}
