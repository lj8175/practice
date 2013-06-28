#include "tlog.h"

int main()
{
    TLog* logger = TLog::getInstance();
    logger->error("what?");
}
