#include <stdio.h>
#include <stdint.h>

#define MAX(x,y) ((x)>(y)?(x):(y))

int maxSumSubArr(int arr[], int* begin, int* end)
{
	int max = INT32_MIN, sum = -1, pres, s, e;
	for(int i=*begin; i<*end; i++)
	{
		if(sum<0)
		{
			sum=arr[i];
			s = i;
		}
		else sum+=arr[i];
			
		if(sum>max)
		{
			pres = s;
			max = sum;
			e = i+1;
		}
	}
	*begin = (s>e)?pres:s;
	*end = e;
	return max;
}

int MaxSum(int *arr, int n)
{
	int Start[n], All[n];
	Start[n-1] = arr[n-1];
	All[n-1]=arr[n-1];
	for(int i=n-2;i>=0;i--)
	{
		Start[i] = MAX(arr[i], arr[i]+Start[i+1]);
		All[i] = MAX(Start[i], All[i+1]);
	}
	return All[0];
}

int main()
{
	int arr[] = {3, 15, -1, 6, -8, 9, 7,-6};
	//int arr[] = {1, -2, 3, 10, -4, 7, 2, -5};
	//int arr[] = {-1, -2, -3, -4};
	//int arr[] = {-4,-3,-2,-1};
	//int arr[] = {-4,-1,-2,-3};
	int begin=0;
	int end = sizeof(arr)/sizeof(int);
	int sum = maxSumSubArr(arr, &begin, &end);
	int sum1 = MaxSum(arr,sizeof(arr)/sizeof(int));
	printf("sum[%d], sum1[%d], begin[%d], end[%d]\n",sum, sum1, begin, end);
	
}