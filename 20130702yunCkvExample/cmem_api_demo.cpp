// cmem apid v3.0.13使用示例
// last update: 2013.6.2
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <string>
#include "trmem_cache_lib.h"  //包这个头文件即可，其他同文件在编译路径上

using namespace ssdasn;

// L5或者接入机配置
static int g_use_l5 = 0;
static char *g_access_ip[] ={
	"10.136.9.77",
	NULL
};
static unsigned int g_access_port = 9101;
static int g_modid = 12345;
static int g_cmdid = 56789;

//bid及bid密码配置
static int g_bid = 101020276;
static char *g_passwd = "1234abcd";

//超时参数根据业务机器到cmem接入机之间的网络延时以及key-value的长度和批操作数量而决定
//tcp连接超时配置
static int connect_timeout_ms = 2000;
//tcp读写超时配置
static int timeout_ms = 1000;
//故障接入机IP冻结时间配置
static int iFreezeSecs = 60;
//L5 API访问超时配置
static float l5_timeout_s = 0.2; 

int main(int argc, char *argv[])
{
	int ret;
	string str_last_access;
	unsigned short u_last_port;
	
	// 1. cmem api实例化
	// api实例只能以指针或者引用传递赋值。例如
	// 		trmem_client_api api1；
	//		trmem_client_api api2 = api1；  	(错误，不能以api实例构造新的实例)
	//      trmem_client_api api3;
	//		api3 = api1; 						(错误，不能按值传递)
	//		trmem_client_api *p_api4 = &api1; 	(正确，按指针传递）
	//		trmem_client_api &ref_api5 = api1;	（正确，按值传递）
	trmem_client_api m_st_api;
	
	// 2.设置L5 MODID:CMDID，或者接入机IP，以及TCP读写超时时间、L5接口超时时间或者故障IP冻结时间
	if (g_use_l5)
	{
		// 2.1使用L5容灾
		ret = m_st_api.config_l5_sid(g_modid, g_cmdid, timeout_ms, l5_timeout_s);
		if (ret)
		{
			printf("config_l5_sid error: ret=%d, errmsg=%s\n", ret, m_st_api.GetLastErr());
			return ret;
		}
	}
	else
	{
		// 2.2使用固定接入机IP，cmem api自身进行接入机容灾，故障IP冻结
		vector<TServerAddr> vec_server_addr;
		TServerAddr addr;
		int i=0;
		while (g_access_ip[i])
		{
			snprintf(addr.strIP, sizeof(addr.strIP), "%s", g_access_ip[i]);
			addr.usPort = g_access_port;
			vec_server_addr.push_back(addr);
			i++;
		}
		ret = m_st_api.config_server_addr(vec_server_addr, timeout_ms, iFreezeSecs);
		if (ret)
		{
			printf("config_server_addr error: ret=%d, errmsg=%s\n", ret, m_st_api.GetLastErr());
			return ret;
		}
	}
	// 3. 设置TCP连接超时时间
	m_st_api.config_connect_timeout(connect_timeout_ms);
	if (ret)
	{
		printf("config_connect_timeout error: ret=%d, errmsg=%s\n", ret, m_st_api.GetLastErr());
		return ret;
	}
	
	// 4. 设置bid密码
	ret = m_st_api.set_passwd(g_bid, g_passwd);
	if (ret)
	{
		// 密码格式不对，密码由8个数字或字母组成的字符串
		printf("set_passwd error! ret=%d\n", ret);
		return ret;
	}
	
	//  5. 具体get， set，del操作
	//普通字符串作为key
	string str_key = "abcd";
	string str_data = "test_data";
	ret = m_st_api.set(g_bid, str_key, str_data, 0, -1);
	if (ret)
	{
		m_st_api.GetLastServer(&str_last_access, &u_last_port);
		printf("set error! bid=%d, access ip:port=%s:%d, ret=%d, errmsg=%s\n", 
			g_bid, str_last_access.c_str(), u_last_port, ret,  m_st_api.GetLastErr());
		return ret;
	}
	printf("set key=%s, data=%s\n", str_key.c_str(), str_data.c_str());
	
	//uin作为key，需要转化成二进制字符串
	unsigned int uin = 1234;
	str_key.assign((char *)&uin, sizeof(uin));
	str_data = "test_data2";
	ret = m_st_api.set(g_bid, str_key, str_data, 0, -1);
	if (ret)
	{
		m_st_api.GetLastServer(&str_last_access, &u_last_port);
		printf("set error! bid=%d, access ip:port=%s:%d, ret=%d, errmsg=%s\n", 
			g_bid, str_last_access.c_str(), u_last_port, ret,  m_st_api.GetLastErr());
		return ret;
	}
	printf("set key=%u, data=%s\n", *(unsigned int*)str_key.c_str(), str_data.c_str());
	
	str_data = "";
	ret = m_st_api.get(g_bid, str_key, str_data, 0, -1);
	if (ret)
	{
		m_st_api.GetLastServer(&str_last_access, &u_last_port);
		printf("get error! bid=%d, access ip:port=%s:%d, ret=%d, errmsg=%s\n", 
			g_bid, str_last_access.c_str(), u_last_port, ret,  m_st_api.GetLastErr());
		return ret;
	}
	printf("get key=%u, data=%s\n", *(unsigned int*)str_key.c_str(), str_data.c_str());

	return 0;
}
