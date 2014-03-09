#ifndef _LOG_H_
#define _LOG_H_

#include <sys/types.h>

#if __GNUC__ == 2 && __GNUC_MINOR__ < 96
#define __builtin_expect(x, expected_value) (x)
#endif

#ifndef likely
#define likely(x)  __builtin_expect(!!(x), 1)
#endif
#ifndef unlikely
#define unlikely(x)  __builtin_expect(!!(x), 0)
#endif

#define LEVEL_FATAL		1  /* critical conditions              */
#define LEVEL_ERROR		2  /* error conditions                 */
#define LEVEL_WARNING		3  /* warning conditions               */
#define LEVEL_INFO		4  /* informational                    */
#define LEVEL_DEBUG		5  /* debug-level messages             */
#define LEVEL_TRACE		6  /* trace-level messages             */

#define PUSHDETAIL(level, fmt, args...) \
	push_write_log (level, "[%s][%d][%s]: " fmt "\n", __FILE__, __LINE__, __FUNCTION__, ##args)

#define PUSH_FATAL(fmt, args...)		PUSHDETAIL(LEVEL_FATAL, fmt, ##args)
#define PUSH_ERROR(fmt, args...)		PUSHDETAIL(LEVEL_ERROR, fmt, ##args)
#define PUSH_WARN(fmt, args...)		    PUSHDETAIL(LEVEL_WARNING, fmt, ##args)
#define PUSH_INFO(fmt, args...)		    PUSHDETAIL(LEVEL_INFO, fmt, ##args)
#define PUSH_DEBUG(fmt, args...)		PUSHDETAIL(LEVEL_DEBUG, fmt, ##args)
#define PUSH_TRACE(fmt,args...)		    PUSHDETAIL(LEVEL_TRACE, fmt, ##args)

void push_write_log (int lvl, const char* fmt, ...);
int push_log_init (const char* dir, int lvl, const char* pre_name);
void push_log_close (void);

#endif //#ifndef LOG_H

