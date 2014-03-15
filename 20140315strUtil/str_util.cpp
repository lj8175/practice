#include "str_util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

string CStrUtil::ltrim(const string &str, const string &cutstr)
{
	string retstr("");
	int pos = str.find_first_not_of(cutstr);
	if (pos > 0)
	{
		retstr = str.substr(pos);
	}
	else if (pos == 0)
	{
		return str;
	}

	return retstr;
}

string CStrUtil::rtrim(const string &str, const string &cutstr)
{
	string retstr("");
	string::size_type pos = str.find_last_not_of(cutstr);
	if (pos < str.size() - 1)
	{
		retstr = str.substr(0, pos + 1);
	}
	else if (pos == str.size() - 1){
		return str;
	}

	return retstr;
}

string CStrUtil::trim(const string &str, const string &cutstr)
{
	int pos1, pos2;
	pos1 = str.find_first_not_of(cutstr);
	pos2 = str.find_last_not_of(cutstr);

	if ((pos1 < 0) || (pos2 < 0))
	{
		return "";
	}

	return str.substr(pos1, pos2 - pos1 + 1);
}

string CStrUtil::toString(int val)
{
	char buff[15];

	memset(buff, 0, 15);
	snprintf(buff, 14, "%d", val);
	return string(buff);
}

string CStrUtil::toString(long val)
{
	char buff[33];

	memset(buff, 0, 33);
	int ret = snprintf(buff, 32, "%ld", val);
	if (ret < 0)
	{
		return "";
	}

	return string(buff);
}

string CStrUtil::toString(unsigned int val)
{
	char buff[33];

	memset(buff, 0, 33);
	int ret = snprintf(buff, 32, "%u", val);
	if (ret < 0)
	{
		return "";
	}

	return string(buff);
}

string CStrUtil::toString(unsigned long val)
{
	char buff[33];

	memset(buff, 0, 33);
	int ret = snprintf(buff, 32, "%lu", val);
	if (ret < 0)
	{
		return "";
	}

	return string(buff);
}

void CStrUtil::split(const string &delimiter, const string &str, vector<string> &ar)
{
	ar.clear();

	string::size_type pos;
	string::size_type nStart = 0;
	string cookie;

	pos = str.find(delimiter, nStart);
	while (string::npos != pos)
	{
		cookie = str.substr(nStart, pos - nStart);
		ar.push_back(cookie);

		nStart = pos + 1;
		pos = str.find(delimiter, nStart);
	}

	if (nStart < str.size())
	{
		cookie = str.substr(nStart);
		ar.push_back(cookie);
	}
}

string CStrUtil::replace(const string &search, const string &replaceStr, const string &subject)
{
	if (search.empty() || replaceStr.empty() || subject.empty())
	{
		return subject;
	}

	string::size_type pos;
	string ::size_type nStart = 0;
	string retStr = "";

	pos = subject.find(search, nStart);
	while (string::npos != pos)
	{
		retStr += subject.substr(nStart, pos - nStart);
		retStr += replaceStr;

		nStart = pos + search.size();
		pos = subject.find(search, nStart);
	}

	if (nStart < subject.size())
	{
		retStr += subject.substr(nStart);
	}

	return retStr;
}

string CStrUtil::long2ip(long val)
{
    char buff[16];
    uint8_t *p = (uint8_t*)&val;
    snprintf(buff, 16, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
    return buff;
}

