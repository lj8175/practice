#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>
#include "log.h"
 
#define MAX_LOG_CNT	1000
#define LOG_MAX_BUF_LENGTH 4096
static int has_init = 0;
static int log_level;
static off_t log_size;
static char log_pre[32] = "default";
static char log_dir[256] = ".";
static struct fds_t {
	int seq;
	int opfd;
	u_short day;
} fds_info;

static inline void log_file_name(int seq, char* file_name, time_t now)
{
	struct tm tm;

	localtime_r(&now, &tm);
	snprintf (file_name, FILENAME_MAX-1, "%s/%s.log.%04d%02d%02d.%03d", log_dir, log_pre, 
					tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, seq);
}

static int request_log_seq()
{
	char file_name[FILENAME_MAX];
	int seq;
	time_t now = time (NULL);
	/*
	 * find last log and tag with seq
	 */
	for (seq = 0; seq < MAX_LOG_CNT; seq ++) {
		log_file_name (seq, file_name, now);

		if (access (file_name, F_OK)) {
			if (seq == 0)
				return 0;
			else
				break;
		}
	}

	if (seq == MAX_LOG_CNT) {
		fprintf (stderr, "too many log files\n");
		return -1;
	}
	
	return seq == 0 ? seq : seq - 1;
}

static int open_fd(time_t now)
{
	char file_name[FILENAME_MAX];
	struct tm *tm;
	int val;

	log_file_name (fds_info.seq, file_name, now);
	fds_info.opfd = open (file_name, O_WRONLY|O_CREAT|O_APPEND, 0644);
	//bug fix: 20130731 (fds_info.opfd > 0) => (fds_info.opfd >= 0)
	if (fds_info.opfd >= 0)
    {
		tm = localtime (&now);
		fds_info.day = tm->tm_yday;
		
		val = fcntl(fds_info.opfd, F_GETFD, 0);
		val |= FD_CLOEXEC;
		fcntl(fds_info.opfd, F_SETFD, val);
	}
	return fds_info.opfd;
}

static int shift_fd (time_t now)
{
	off_t length; 
	struct tm *tm;
	
	if (fds_info.opfd < 0 && unlikely (open_fd (now) < 0)) 
		return -1;

	length = lseek (fds_info.opfd, 0, SEEK_END);
	tm = localtime (&now);
	if (likely (length < log_size && fds_info.day == tm->tm_yday)) 
		return 0;

	close (fds_info.opfd);
	if ( fds_info.day != tm->tm_yday)
		fds_info.seq = 0;
	else
		fds_info.seq ++;

	return open_fd (now);
}

void push_write_log (int lvl, const char* fmt, ...)
{
	struct tm tm;
	int pos, end;
	va_list ap;
	time_t now;
    char log_buffer[LOG_MAX_BUF_LENGTH];

	if (unlikely (!has_init)) 
	{
		va_start(ap, fmt);
		switch (lvl) {
			case LEVEL_FATAL:
			case LEVEL_ERROR:
				vfprintf (stderr, fmt, ap);
				break;
			default:
				vfprintf (stdout, fmt, ap);
				break;
		}
		va_end(ap);
		return;
	}

	if (lvl > log_level)
	{
	    return ;
	}

	now = time (NULL); 
	if (unlikely (shift_fd (now) < 0))
	{
	    return ;
	}

	localtime_r(&now, &tm);
	struct timeval tv;
	gettimeofday(&tv, NULL);

	pos = snprintf (log_buffer, LOG_MAX_BUF_LENGTH-1, "[%s][%02d:%02d:%02d.%03d][%u:%lu]", 
			((char*[]){
					"0", "FATAL", "ERROR", "WARN",
					"INFO", "DEBUG", "TRACE"
			})[lvl],
			tm.tm_hour, tm.tm_min, tm.tm_sec, (int)(tv.tv_usec/1000),
			getpid(), pthread_self());
	va_start(ap, fmt);
	end = vsnprintf (log_buffer + pos, LOG_MAX_BUF_LENGTH-pos-1, fmt, ap);
	va_end(ap);

	write (fds_info.opfd, log_buffer, end + pos);

	return;
}

int push_log_init (const char* dir, int lvl, const char* pre_name)
{
	int ret_code = -1;

	//重复初始化则直接正确退出
	if (likely (has_init))
	{
	    ret_code = 0;
	    goto exit_entry;
	}

	(lvl > LEVEL_TRACE) ? (log_level = LEVEL_TRACE) : (log_level = lvl);
	(lvl < LEVEL_ERROR) ? (log_level = LEVEL_ERROR) : (log_level = lvl);
	log_size = 1<<30; //max single file size 1GB

	if (access (dir, W_OK)) {
		fprintf (stderr, "access log dir %s error, %m\n", dir);
		goto exit_entry;
	}
	strncpy (log_dir, dir, sizeof (log_dir) - 1);
	if (pre_name != NULL)
		strncpy (log_pre, pre_name, sizeof (log_pre) - 1);

	fds_info.opfd = -1;
	fds_info.seq = request_log_seq();

	if (fds_info.seq  < 0)
		goto exit_entry;
	
	has_init = 1;
	ret_code = 0;

exit_entry:
	return ret_code;
}

void push_log_close (void)
{
    if(-1 != fds_info.opfd)
    {
        close (fds_info.opfd);
	}
    has_init = 0;
}

