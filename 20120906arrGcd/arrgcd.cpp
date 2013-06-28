#include <stdlib.h>
#include "arrgcd.h"

int arrgcd(int *array, size_t size)
{
    size_t i_min=0;
    size_t cnt = size;
    int *arr = (int*) malloc(size*sizeof(int));
    if(!arr)
    {
        return -1; //no memory to use
    }

    for(int i=0; i<size; i++)
    {
        arr[i] = array[i];
        if(arr[i]<arr[i_min])
        {
            i_min = i;
        }
    }
    
    if(arr[i_min] == 1) return 1;
    if(arr[i_min] <= 0) return 0;

    while(cnt != 1)
    {
        for(int i=0; i<size; i++)
        {
            if(i != i_min && arr[i] != 0) 
            {
                arr[i] -= arr[i_min];
                if(arr[i] == 0)
                {
                    cnt--;
                }
                else if(arr[i] < arr[i_min])
                {
                    i_min = i;
                }
            }
        }
        
    }
    int rtn = arr[i_min];
    free(arr);
    return rtn;
}

