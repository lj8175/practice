#include <stdio.h>

static void heap_adjust(int arr[], int begin, int end)
{
    int save = arr[begin];
    for(int i=2*begin; i<=end; i*=2)
    {
        if(i<end && arr[i]<arr[i+1])
        {
            i+=1;
        }
        if(arr[begin] >= arr[i])
        {
            break;
        }
        arr[begin] = arr[i];
        begin = i;
    }
    arr[begin] = save;
}

void heap_sort(int array[], int size)
{
    int arr[size+1];
    arr[0] = 0;
    for (int i=0; i<size; ++i)
    {
        arr[i+1] = array[i];
    }

    for(int i=size/2; i>0; --i)
    {
        heap_adjust(arr, i, size);
    }
    for(int i=size; i>1; --i)
    {
        int tmp = arr[1];
        arr[1] = arr[i];
        arr[i] = tmp;
        heap_adjust(arr, 1, i-1); 
    }

    for (int i=0; i<size; ++i)
    {
        array[i] = arr[i+1];
    }

}

int main()
{
    int array[] = {3,1,2,4,5};
    int size = sizeof(array)/sizeof(int);
    heap_sort(array, size);
    for (int i=0; i<size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

}
