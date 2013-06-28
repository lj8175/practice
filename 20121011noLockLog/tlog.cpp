#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include "tlog.h"

int TLog::OFF = 0;
int TLog::FATAL = 1;
int TLog::ERROR = 2;
int TLog::WARN = 3;
int TLog::INFO = 4;
int TLog::DEBUG = 5;
int TLog::TRACE = 6;
int TLog::ALL = 7;

int TLog::MAX_BUF = 4096;

TLog* TLog::m_logger = NULL;
pthread_mutex_t TLog::TLogLock::m_mutex = PTHREAD_MUTEX_INITIALIZER;

TLog* TLog::getInstance(const char* path, int level)
{
    if (NULL==m_logger)
    {
        TLogLock guard;
        if (NULL==m_logger)
        {
            m_logger = new TLog(path, level);
        }
    }
    return m_logger;
}

TLog::TLog(const char* path, int level) : m_level(level)
{
    m_fd = open(path, O_CREAT|O_RDWR|O_APPEND, 0666);
}
TLog::~TLog(){}

void TLog::log(int level, const char *fmt, ...)
{
    if (level>m_level)
    {
        return;
    }

    char buf[TLog::MAX_BUF];
    va_list args;
    va_start(args, fmt);
    int size = vsnprintf(buf, TLog::MAX_BUF, fmt, args);
    va_end(args);
    
    write(m_fd, buf, size);
}

void TLog::error(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(TLog::ERROR, fmt, args);
    va_end(args);
}

void TLog::warn(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(TLog::WARN, fmt, args);
    va_end(args);
}

void TLog::info(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(TLog::INFO, fmt, args);
    va_end(args);
}

void TLog::debug(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    log(TLog::DEBUG, fmt, args);
    va_end(args);
}
