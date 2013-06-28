#ifndef _TBASE_TLOG_H_
#define _TBASE_TLOG_H_
#include <time.h>
#include <stdarg.h>

//多线程共享log对象的时候,需要使用_THREAD_SAFE编译本组件
#ifdef _THREAD_SAFE
#include <pthread.h>
#endif

#define DEFAULT_MAX_FILE_SIZE  (1 << 30)     //最大文件SIZE为1G
#define DEFAULT_MAX_FILE_NO    1000          //默认最大文件编号
#define MAX_PATH_LEN           256           //最大路径长度
#define MAX_LOG_LEN            4096          //一次日志最大长度
#define LOG_FLAG_TIME          0x01          //打印时间戳
#define LOG_FLAG_TID           0x02			 //打印线程ID
#define LOG_FLAG_LEVEL         0x04			 //打印日志级别

#define ERR_FD_NO              0x00          //初始文件句柄号，默认是标准输出

namespace tbase {
	namespace tlog {
	
		//日志类型
		enum LOG_TYPE
		{
			LOG_TYPE_CYCLE = 0,
			LOG_TYPE_DAILY,
			LOG_TYPE_HOURLY,
			LOG_TYPE_CYCLE_DAILY,
			LOG_TYPE_CYCLE_HOURLY
		};
		//日志级别
		enum LOG_LEVEL
		{
			LOG_TRACE = 0,
			LOG_DEBUG,
			LOG_NORMAL,
			LOG_ERROR,
			LOG_FATAL,
            LOG_BOOT,
			LOG_NONE    //当要禁止日志的时候,可以设置此级别LOG_NONE
  		};

		//钩子函数原型
		//fmt:		格式字符串
		//ap:       可变参数集合
		//返回值:   返回0表示不记录到日志文件,否则记录到日志文件
		typedef int (*log_hook) (const char *fmt, va_list ap);

		//日志类
		class CTLog
		{
			public:
				CTLog();
				~CTLog();

				//初始化日志
				//log_level:		日志级别
				//log_type:			日志类型
				//log_path:			日志存放目录
				//name_preifx:		日志文件名前缀
				//max_file_size:	每个日志文件的最大长度
				//max_file_no:		日志文件最大个数
				//返回值:			0成功,否则失败
				int log_open(int log_level, int log_type, const char* log_path, const char* name_prefix, int max_file_size, int max_file_no);
				//设置日志级别
				//level:		新的日志级别
				//返回值:		老的日志级别
				int log_level(int level);
				int get_level(void);
				//打印格式化日志
				void log_i(int flag, int log_level, const char *fmt, ...);
				//打印bin日志
				void log_i_bin(int log_level, const char* buf, int len);
				//设置钩子函数
				void log_set_hook(log_hook hook);
				//把二进制数据转换为可显示的hex字符串
				//bin:		二进制数据
				//len:      数据长度
				//buf:      字符串缓冲区
				//返回值:   字符串指针
				static const char* to_hex(const char* bin, int len, char* buf);
			protected:
				int log_level_;
				int log_type_;
				char log_path_[MAX_PATH_LEN];
				char name_prefix_[MAX_PATH_LEN];
				int max_file_size_;
				int max_file_no_;	

				int log_fd_;
				int cur_file_size_;
				int cur_file_no_;
				time_t pre_time_;
				char filename_[MAX_PATH_LEN];
				char buffer_[MAX_LOG_LEN];
				log_hook hook_;
				
				void close_file();
				void log_file_name(char* filepath, char* filename);
				int open_file();
				void init_cur_file_no();
				void force_rename(const char* src_pathname, const char* dst_pathname);
				int shift_file();

				void get_time(int& buff_len);
				void get_tid(int& buff_len);
				//void get_file(int& buff_len);
				void get_level(int& buff_len, int level=-1);
#ifdef _THREAD_SAFE
				class CLogLock
				{
					public:
						CLogLock()
						{
							pthread_mutex_lock(&mutex_);
						}
						~CLogLock()
						{
							pthread_mutex_unlock(&mutex_);
						}
					private:
						static pthread_mutex_t mutex_;	
				};
#endif
		};
	}
}
//用户使用应该调用如下跟函数对应的宏
#define LOG_OPEN						log_open 
#define LOG_P(lvl, fmt, args...)  		log_i(LOG_FLAG_TIME, lvl, fmt"\n", ##args)
#define LOG_P_NOTIME(lvl, fmt, args...) log_i(0, lvl, fmt"\n", ##args)
#define LOG_P_PID(lvl, fmt, args...) 	log_i(LOG_FLAG_TIME | LOG_FLAG_TID, lvl, fmt"\n", ##args)
#define LOG_P_LEVEL(lvl, fmt, args...)	log_i(LOG_FLAG_TIME | LOG_FLAG_LEVEL, lvl, fmt"\n", ##args)
#define LOG_P_FILE(lvl, fmt, args...)	log_i(LOG_FLAG_TIME, lvl, "[%-10s][%-4d][%-10s]"fmt"\n", __FILE__, __LINE__, __FUNCTION__, ##args)
#define LOG_P_ALL(lvl, fmt, args...)	log_i(LOG_FLAG_TIME | LOG_FLAG_LEVEL | LOG_FLAG_TID, lvl, "[%-10s][%-4d][%-10s]"fmt"\n",__FILE__, __LINE__, __FUNCTION__, ##args)
#define LOG_P_BIN						log_i_bin
#define SET_HOOK   						log_set_hook
#endif 
