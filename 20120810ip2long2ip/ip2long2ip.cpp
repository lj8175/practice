#include<string>
#include<iostream>
#include<stdio.h>
#include <arpa/inet.h>

using namespace std;

unsigned long ip2long(const char* ip)
{
  unsigned long v[4];
  sscanf(ip,"%lu.%lu.%lu.%lu", &v[0], &v[1], &v[2], &v[3]);
  return (v[0]<<24) + (v[1]<<16) + (v[2]<<8) + v[3];
}

string long2ip(unsigned long v)
{
  char ip[20] = {0};
  snprintf(ip, 20, "%lu.%lu.%lu.%lu",
           (v&0xff000000)>>24,
           (v&0x00ff0000)>>16,
           (v&0x0000ff00)>>8,
           (v&0x000000ff)
          );
  return string(ip);
}


int main()
{
  unsigned long val = ip2long("10.1.152.138");
  val = ip2long("127.0.0.1");
  cout << val << endl;
  for(int i=val; i<val+10; i++)
  {
    cout << long2ip(i)<<endl;
  }
  val = inet_addr("10.1.152.139");
  cout << val << endl;
}
