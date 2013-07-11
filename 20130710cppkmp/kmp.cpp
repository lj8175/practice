#include <vector>
#include <string>
#include <iostream>
#include <string.h>

using namespace std;

static void getNext(const char* str, int next[])
{
    int i=0, j=-1;
    const int len=strlen(str);
    next[i] = j;
    while (i < len)
    {
        if ((j == -1) || (str[i] == str[j]))
        {
            ++i;
            ++j;
            if (i < len)
            {
                next[i] = j;
            }
        } 
        else
        {
            j = next[j];
        }
        
    }
}

int kmpIndex(const char* str, const char* search, int pos=0)
{
    int i=pos, j=0;
    const int strLen = strlen(str);
    const int searchLen = strlen(search);
    int next[searchLen];
    getNext(search, next);
    while ((i<strLen) && (j<searchLen))
    {
        if ((j==-1) || (str[i]==search[j]))
        {
            ++i;
            ++j;
        }
        else
        {
            j = next[j];
        }
    }

    if (j>=searchLen)
    {
        return i-searchLen;
    }
    else
    {
        return -1;
    }

}

int main()
{
    char *str = (char*)"acabaabaabcacaabc";
    char *search= (char*)"abaabcac";
    cout << kmpIndex(str, search) << endl;
}
