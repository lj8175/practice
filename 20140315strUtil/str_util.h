#ifndef _STRUTIL_H_
#define _STRUTIL_H_

#include <stdint.h>
#include <string>
#include <vector>
#include <sstream>
using std::string;
using std::vector;

class CStrUtil
{
public:
	static string ltrim(const string &str, const string &cutstr = " ");
	static string rtrim(const string &str, const string &cutstr = " ");
	static string trim(const string &str, const string &cutstr = " ");
	static string toString(int val);
	static string toString(long val);
	static string toString(unsigned int val);
	static string toString(unsigned long val);
	static void split(const string &delimiter, const string &str, vector<string> &ar);
	static string replace(const string &search, const string &replaceStr, const string &subject);
	static string long2ip(long val);
};

#endif //_STRUTIL_H_
