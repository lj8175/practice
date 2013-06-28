#include <stdio.h>
#define M 10
#define N 5

void cmn(int res[], int m, int n, int pos=0, int val=0)
{
    if (pos==n)
    {
        for (int i=0; i<n; i++)
        {
            printf("%d ", res[i]);
        }
        printf("\n");
        return;
    }
    for ( ; val+n-pos<=m; val++)
    {
        res[pos] = val+1;
        cmn(res, m, n, pos+1, val+1);
    }
}

int main()
{
    int arr[N];
    cmn(arr, M, N);
}
