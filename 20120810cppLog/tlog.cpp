#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <time.h>
#include <linux/unistd.h>
#include <errno.h>
#include "tlog.h"

using namespace tbase::tlog;

#ifdef _THREAD_SAFE
pthread_mutex_t CTLog::CLogLock::mutex_ = PTHREAD_MUTEX_INITIALIZER;
#endif

#define SPRINT(buff, len, fmt, args...)   len += snprintf(buff + len, MAX_LOG_LEN - len - 1, fmt, ##args); 

CTLog::CTLog():log_level_(0), log_type_(0), max_file_size_(DEFAULT_MAX_FILE_SIZE), max_file_no_(DEFAULT_MAX_FILE_NO), log_fd_(ERR_FD_NO), cur_file_size_(0), cur_file_no_(0), pre_time_(0), hook_(NULL)
{
	memset(log_path_, 0x0, MAX_PATH_LEN);
	memset(name_prefix_, 0x0, MAX_PATH_LEN);
	memset(buffer_, 0x0, MAX_LOG_LEN);
}
CTLog::~CTLog()
{
	close_file();
}
inline void CTLog::get_time(int& buff_len)
{
	time_t now = time(NULL);
	struct tm tmm;
	localtime_r(&now, &tmm); 

	SPRINT(buffer_, buff_len, "[%04d-%02d-%02d %02d:%02d:%02d]", tmm.tm_year + 1900, tmm.tm_mon + 1, tmm.tm_mday, tmm.tm_hour, tmm.tm_min, tmm.tm_sec);

}
inline void CTLog::get_tid(int& buff_len)
{
	pid_t tid = syscall(__NR_gettid);
	SPRINT(buffer_, buff_len, "[%-5d]", tid);
}
inline void CTLog::get_level(int& buff_len, int level)
{
	static char* level_str[] = {"TRACE", "DEBUG", "NORMAL", "ERROR", "FATAL","BOOT"}; 
	if(level < 0)
		level = log_level_;
	SPRINT(buffer_, buff_len, "[%-6s]", level_str[level]);
}
void CTLog::log_file_name(char* filepath, char* filename)
{
	char timestr[64] = {0};
	struct tm tmm;
	localtime_r((time_t*)&(pre_time_), &tmm); 
	snprintf(timestr, sizeof(timestr)-1, "%04d%02d%02d%02d%02d%02d", 
			tmm.tm_year + 1900, tmm.tm_mon + 1, tmm.tm_mday, tmm.tm_hour, tmm.tm_min, tmm.tm_sec);

	switch (log_type_)
	{
		case LOG_TYPE_DAILY:
		case LOG_TYPE_CYCLE_DAILY:
			{
				timestr[8] = '.';
				timestr[9] = 0;
				break;
			}
		case LOG_TYPE_HOURLY:
		case LOG_TYPE_CYCLE_HOURLY:
			{
				timestr[10] = '.';
				timestr[11] = 0;
				break;
			}
		default:
			{
				timestr[0] = 0;
				break;
			}
	}
	if(filepath != NULL)
	{
		snprintf(filepath, MAX_PATH_LEN - 1, "%s/%s.%slog", log_path_, name_prefix_, timestr);
		if (filename != NULL)
		{
			strncpy(filename, filepath + strlen(log_path_) + 1, MAX_PATH_LEN - 1);
		}
	}
}
int CTLog::open_file()
{
    char filepath[MAX_PATH_LEN] = {0};
    log_file_name(filepath, filename_);

    if(((log_fd_ = open(filepath, O_CREAT | O_RDWR | O_APPEND, 0666)) > 0)
		 && ((cur_file_size_ = lseek(log_fd_, 0, SEEK_END)) >= 0))
    {
    	return 0;
    }
	else
	{	
    	cur_file_size_ = 0;
    	log_fd_ = ERR_FD_NO;
    	return -1;
	}	
}
void CTLog::init_cur_file_no()
{
    char base_file[MAX_PATH_LEN] = {0};
    char dest_file[MAX_PATH_LEN] = {0};
    
	log_file_name(base_file, NULL);
	
	int file_no = 1;
    time_t min_modify_time = 0;
    struct stat sb;
	
    for(int i = 1; ; i++)
    {
  		snprintf(dest_file, sizeof(dest_file)-1, "%s.%d", base_file, i);
        if (stat(dest_file, &sb) != 0)
        {
            file_no = i;
            break;
        }
        if (LOG_TYPE_CYCLE == log_type_ || LOG_TYPE_CYCLE_DAILY == log_type_ || LOG_TYPE_CYCLE_HOURLY == log_type_)
        {
            if (i >= max_file_no_)
            {
                break;
            }
            if (min_modify_time == 0 || sb.st_mtime < min_modify_time)
            {
                file_no = i;
                min_modify_time = sb.st_mtime;
            }
        }
    }

    cur_file_no_ = file_no;
}
void CTLog::force_rename(const char* src_pathname, const char* dst_pathname)
{
	unlink(dst_pathname);
	rename(src_pathname, dst_pathname);
}
void CTLog::close_file()
{
	if(log_fd_ > 0)
	{
		close(log_fd_);
		log_fd_ = ERR_FD_NO;
	}
}
const char* CTLog::to_hex(const char* bin, int len, char* buf)
{
	for(int i = 0; i < len; ++i)
	{
		//sprintf(buf + i * 3, "%02X ", bin[i]);
		snprintf(buf + i * 3, 3, "%02X ", bin[i]);
	}
	return buf;
}
int CTLog::shift_file()
{
	bool need_shift = false;
	int new_file_no = 0;
	struct tm cur_tm;
	struct tm pre_tm;
	time_t cur_time = time(NULL);
	localtime_r(&cur_time, &cur_tm);
	localtime_r(&pre_time_, &pre_tm);

	switch (log_type_)
	{
		case LOG_TYPE_DAILY:
			{
				if (cur_tm.tm_yday != pre_tm.tm_yday)
				{
					need_shift = true;
					new_file_no = 1;
				}
				break;
			}
		case LOG_TYPE_HOURLY:
			{
				if ((cur_tm.tm_yday != pre_tm.tm_yday) || (cur_tm.tm_hour != pre_tm.tm_hour))
				{
					need_shift = true;
					new_file_no = 1;
				}
				break;
			}
		case LOG_TYPE_CYCLE_DAILY:
			{
				if (cur_tm.tm_yday != pre_tm.tm_yday)
				{
					need_shift = true;
				}
				break;
			}
		case LOG_TYPE_CYCLE_HOURLY:
			{
				if ((cur_tm.tm_yday != pre_tm.tm_yday) || (cur_tm.tm_hour != pre_tm.tm_hour))
				{
					need_shift = true;
				}
				break;
			}
		default:
			break;
	}

	if (!need_shift && cur_file_size_ < max_file_size_)
	{
		return 0;
	}
	
#ifdef _THREAD_SAFE
	CLogLock guard;  //后面的操作需要互斥
#endif		
	
	if (cur_file_size_ >= max_file_size_)
	{
		need_shift = true;
		new_file_no = cur_file_no_ + 1;
		if (((LOG_TYPE_CYCLE == log_type_) || (LOG_TYPE_CYCLE_DAILY == log_type_) || (LOG_TYPE_CYCLE_HOURLY == log_type_)) && (new_file_no > max_file_no_))
		{
			new_file_no = 1;
		}
	}
	else if(!need_shift)
	{
		return 0;
	}

	close_file();

	char src_file[MAX_PATH_LEN] = {0};
	char dst_file[MAX_PATH_LEN] = {0};
	snprintf(src_file, sizeof(src_file)-1, "%s/%s", log_path_, filename_);
	snprintf(dst_file, sizeof(dst_file)-1, "%s.%d", src_file, cur_file_no_);
	force_rename(src_file, dst_file);

	pre_time_ = cur_time;
	cur_file_no_ = new_file_no;
	return open_file();
}
int CTLog::log_open(int log_level, int log_type, const char* log_path, const char* name_prefix, int max_file_size, int max_file_no)
{
	log_level_ = log_level;
	log_type_ = log_type;
	memset(name_prefix_, 0x0, MAX_PATH_LEN);
	strncpy(name_prefix_, name_prefix, MAX_PATH_LEN - 1);
	memset(log_path_, 0x0, MAX_PATH_LEN);
	strncpy(log_path_, log_path, MAX_PATH_LEN - 1);
	int pathlen = strlen(log_path_);
	if(log_path_[pathlen - 1] == '/')
	{
		log_path_[pathlen - 1] = 0;
	}

	max_file_size_ = max_file_size;
	if (max_file_size_ > DEFAULT_MAX_FILE_SIZE)
	{
		max_file_size_ = DEFAULT_MAX_FILE_SIZE;
	}
	max_file_no_ = max_file_no;
	pre_time_ = time(NULL);

	close_file();
	init_cur_file_no();
	return open_file();
}
int CTLog::log_level(int level)
{
    if(level < 0)			//get
	{
		return log_level_;
	}
	else					//set&return old
	{		
		int tmp = log_level_;
 		log_level_ = level;
 		return tmp;
	}	
}

int CTLog::get_level(void)
{
    return log_level_;
}

void CTLog::log_i(int flag, int log_level, const char *fmt, ...)
{
	if(log_level < log_level_)
		return;	

	if(shift_file() < 0)
		return;

	va_list ap;
	va_start(ap, fmt);
	
	if(hook_)
	{
		va_list ap2;
#if __GNUC__ < 3
		__va_copy(ap2, ap);
#else
        va_copy(ap2, ap);
#endif
		if(!hook_(fmt, ap2))
		{	
			va_end(ap2);
			va_end(ap);
			return;
		}	
	}
#ifdef _THREAD_SAFE
	CLogLock guard;  //buffer_需要互斥
#endif		
	int buff_len = 0;
	if(flag & LOG_FLAG_TIME)
		get_time(buff_len);		
	if(flag & LOG_FLAG_LEVEL)
		{
		 get_level(buff_len,log_level);	
		}
	if(flag & LOG_FLAG_TID)
		get_tid(buff_len);

	buff_len += vsnprintf(buffer_ + buff_len, MAX_LOG_LEN - buff_len - 1, fmt, ap);
	write(log_fd_, buffer_, buff_len);	
    cur_file_size_ += buff_len;
    
    if(LOG_BOOT == log_level)
        fwrite(buffer_, 1, buff_len, stderr);
    
	va_end(ap);
}
void CTLog::log_i_bin(int log_level, const char* buf, int len) 
{
	if(log_level < log_level_)
		return;	

	if(shift_file() < 0)
		return;

	write(log_fd_, buf, len);	
	cur_file_size_ += len;

    if(LOG_BOOT == log_level)
        fwrite(buf, 1, len, stderr);
}

void CTLog::log_set_hook(log_hook hook)
{
 	hook_ = hook;
}
