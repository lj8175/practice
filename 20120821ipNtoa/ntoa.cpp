#include<string>
#include<iostream>
#include<stdio.h>
#include <arpa/inet.h>

using namespace std;

int main()
{
  in_addr_t val = inet_addr("10.1.152.139");
  cout << val << endl;
  struct in_addr ip;
  ip.s_addr = val;
  char *p1 = inet_ntoa(ip);
  string s1(p1);

  ip.s_addr = val-1;
  p1 = inet_ntoa(ip);
  string s2(p1);
  cout<<s1<<endl;
  cout<<s2<<endl;
  cout<<string(p1)<<endl;
  cout<<string(inet_ntoa(ip))<<endl;

}
