#include <stdio.h>
#include <iostream>
#include <strings.h>
#include <string.h>

using namespace std;


string hex_string(const char* str, int size)
{
    const int len = 2*size+1;
    char tstr[len];
    bzero(tstr, len);
    for (int i=0; i<size; ++i)
    {
        snprintf(tstr+2*i, len-2*i, "%X%X", (str[i] >> 4) & 0xf, str[i] & 0xf);
    }
    return string(tstr);
}

string bin_string(const char* str, int size)
{
    string s;
    if (0!=(size%2))
    {
        return s;
    }
    const int len = size/2;
    char tstr[len], ttmp[2];
    bzero(tstr, len);
    bzero(ttmp, 2);
    
    for (int i=0; i<len; ++i)
    {
        unsigned int c1, c2;
        ttmp[0] = str[2*i];
        sscanf(ttmp, "%X", &c1);
        ttmp[0] = str[2*i+1];
        sscanf(ttmp, "%X", &c2);
        tstr[i] = char((c1<<4)|(c2));
    }
    s.assign(tstr,len);
    return s;
}


/*
int main()
{
    char* str = (char*)"12346 7890";
    string s = hex_string(str, strlen(str));
    cout << s << endl;
    s = bin_string(s.c_str(), s.size());
    cout << s << endl;

}
*/
