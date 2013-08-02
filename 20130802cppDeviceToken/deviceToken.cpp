#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "push_encrypt.h"
#include <string>
#include <iostream>

using namespace std;

#define DTTKEY "tpnspushproject5" //16char
#define DTTRANDOM 13

string hex_string(const char* str, int size);
string bin_string(const char* str, int size);

string DeviceId_to_token(string deviceId)
{
    //deviceId奇数字节置反位
    for(size_t i=0; i<deviceId.size(); i+=2)
    {
        deviceId[i] = ~(deviceId[i]);
    }
    char enstr[deviceId.size()];
    bzero(enstr, deviceId.size());
    CPushEncrypt::AES_Encrypt(DTTKEY, deviceId.c_str(), deviceId.size(), enstr, DTTRANDOM);
    //加密后偶数字节置反位
    for(size_t i=1; i<deviceId.size(); i+=2)
    {
        enstr[i] = ~(enstr[i]);
    }
    return hex_string(enstr, deviceId.size());
}

string Token_to_deviceId(string token)
{
    string s = bin_string(token.c_str(), token.size());
    for(size_t i=1; i<s.size(); i+=2)
    {
        s[i] = ~(s[i]);
    }
    char destr[s.size()];
    bzero(destr, s.size());
    CPushEncrypt::AES_Decrypt(DTTKEY, s.c_str(), s.size(), destr, DTTRANDOM);
    for(size_t i=0; i<s.size(); i+=2)
    {
        destr[i] = ~(destr[i]);
    }
    return string(destr, s.size());

}




int main(int argc, char** argv)
{
    char *source=(char*)"1234567890123456";
    if (argc>1)
    {
        source = argv[1];
    }
    string s = DeviceId_to_token(string(source));
    cout << s.size() << endl;
    cout << s << endl;
    /*
    char c = 128;
    printf("%X\n", (c>>4)&0xf);
    printf("%X\n", (c)&0xf);
    */
    s = Token_to_deviceId(s);
    cout << s << endl;
    return 0;
}
