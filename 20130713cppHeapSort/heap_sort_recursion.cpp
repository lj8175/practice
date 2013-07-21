#include <stdio.h>

#define MAX(a,b) (((a)>(b))?(a):(b))
#define LEFT(x) (2*x)
#define RIGHT(x) (LEFT(x)+1)

inline void swap(int &a,int &b)
{
    int tmp = a;
    a = b;
    b = tmp;
}

void heap_adjust_recursion(int arr[], int begin, int end)
{
    int largest=begin;
    if((LEFT(begin)<=end) && (arr[LEFT(begin)]>arr[begin]))
    {
        largest=LEFT(begin);
    }
    if((RIGHT(begin)<=end) && (arr[RIGHT(begin)]>arr[largest]))
    {
        largest=RIGHT(begin);
    }
    if(largest!=begin)
    {
        swap(arr[begin],arr[largest]);
        heap_adjust_recursion(arr,largest,end);
    }
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
        heap_adjust_recursion(arr, i, size);
    }
    for(int i=size; i>1; --i)
    {
        swap(arr[1], arr[i]);
        heap_adjust_recursion(arr, 1, i-1); 
    }

    for (int i=0; i<size; ++i)
    {
        array[i] = arr[i+1];
    }

}

int main()
{
    //int array[] = {3,1,2,4,5};
    int array[] ={5, 3, 7, 5, 6, 2, 9, 4, 12};
    int size = sizeof(array)/sizeof(int);
    heap_sort(array, size);
    for (int i=0; i<size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

}
