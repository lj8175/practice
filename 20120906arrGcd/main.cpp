#include<iostream>
#include"arrgcd.h"

using namespace std;

int main()
{
    int arr[] = {2, 4, 6, 8};
//    int arr[] = {1, 3, 5, 7};
    cout << arrgcd(arr, sizeof(arr)/sizeof(int)) << endl;

}
