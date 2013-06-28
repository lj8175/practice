#include <stdio.h>

int main()
{
    char str1[] = "hello world";
    char str2[] = "hello world";
 
    char* str3 = "hello world";
    char* str4 = "hello world";
 
    if(str1 == str2)
        printf("str1 and str2 are same.\n");
    else
        printf("str1 and str2 are not same.\n");
 
    if(str3 == str4)
        printf("str3 and str4 are same.\n");
    else
        printf("str3 and str4 are not same.\n");
 
    return 0;
}
//str1 and str2 are not same.
//str3 and str4 are same.
