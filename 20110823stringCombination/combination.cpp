#include <cstdio>
#include <cstring>
#include <vector>

using namespace std;
void Combination(char* string, int number, vector<char>& result);

void Combination(char* string)
{
    if(string == NULL)
        return;
 
    int length = strlen(string);
    vector<char> result;
    for(int i = 1; i <= length; ++ i)
    {
        Combination(string, i, result);
    }
}
 
void Combination(char* string, int number, vector<char>& result)
{
    if(number == 0)
    {
        vector<char>::iterator iter = result.begin();
        for(; iter < result.end(); ++ iter)
            printf("%c", *iter);
        printf("\n");
 
        return;
    }
 
    if(*string == '\0')
        return;
 
    result.push_back(*string);
    Combination(string + 1, number - 1, result);
    result.pop_back();
 
    Combination(string + 1, number, result);
}

int main(){
    Combination((char*)"abc");
}
