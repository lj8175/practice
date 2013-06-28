#ifndef __TLOG_H
#define __TLOG_H

#include <stdarg.h>
#include <pthread.h>

class TLog
{
public:
    static TLog* getInstance(const char* path="./log.txt", int level=TLog::ALL);
    void log(int level, const char *fmt, ...);
    void error(const char *fmt, ...);
    void warn(const char *fmt, ...);
    void info(const char *fmt, ...);
    void debug(const char *fmt, ...);

private:
    TLog(const char* path, int level);
    ~TLog();

public:
    static int OFF;
    static int FATAL;
    static int ERROR;
    static int WARN;
    static int INFO;
    static int DEBUG;
    static int TRACE;
    static int ALL;

    static int MAX_BUF;
    
private:
    int m_level;
    int m_fd;
    static TLog *m_logger;


    class TLogLock
    {
    public:
        TLogLock()
        {
            pthread_mutex_lock(&m_mutex);
        }
        ~TLogLock()
        {
            pthread_mutex_unlock(&m_mutex);
        }
    private:
        static pthread_mutex_t m_mutex;
    };
};


#endif
