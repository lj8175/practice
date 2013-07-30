#include "arrays.h"
#include <stdlib.h>
int array_max(int *arr, size_t size)
{
    int max = INT_MIN;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] > max)
        {
            max = arr[i];
        }
    }
    return max;
}

int array_min(int *arr, size_t size)
{
    int min = INT_MAX;
    for (int i = 0; i < size; i++)
    {
        if (arr[i] < min)
        {
            min = arr[i];
        }
    }
    return min;
}

int array_sum(int *arr, size_t size)
{
    int sum = 0;
    for (int i = 0; i < size; i++)
    {
        sum += arr[i];
    }
    return sum;
}

int array_gcd(int *arr, size_t size)
{
    size_t i_min=0;
    size_t cnt = size;
    int *array = (int*) malloc(size*sizeof(int));
    if(!array)
    {
        return -1; //no memory to use
    }

    for(int i=0; i<size; i++)
    {
        array[i] = arr[i];
        if (array[i] == 0)
        {
            --cnt;
        }
        if(array[i]<array[i_min])
        {
            i_min = i;
        }
    }
    
    if(array[i_min] == 1) return  1;
    if(array[i_min] <  0) return -1;
    if(array[i_min] ==  0)
    {
        i_min = find_arrmin_index(array, size, 0);
    }

    while(cnt > 1)
    {
        for(int i=0; i<size; i++)
        {
            if(i != i_min && array[i] != 0) 
            {
                array[i] -= array[i_min];
                if(array[i] == 0)
                {
                    cnt--;
                }
                else if(array[i] < array[i_min])
                {
                    i_min = i;
                }
            }
        }

    }

    int rtn = array_sum(array, size);
    free(array);
    return rtn;
}

size_t find_arrmin_index(int *arr, size_t size, int except)
{
    size_t min_index = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (arr[i] < arr[min_index] && arr[i] != except)
        {
            min_index = i;
        }
    }
    return min_index;
}

void array_copy(int *dest, size_t dsize, int *src, size_t ssize)
{
    for (int i = 0; i < ssize; ++i)
    {
        if (i < dsize)
        {
            dest[i] = src[i];
        }
        else
        {
            break;
        }
    }

}

