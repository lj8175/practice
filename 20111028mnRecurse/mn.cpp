#include<list>  
#include<iostream>  
using namespace std;  
  
list<int>list1;  
int find_factor(int sum, int n)   
{  
    // �ݹ����  
    if(n <= 0 || sum <= 0){
        if(0 == sum) return 1;
        return 0;
    }
      
    // ����ҵ��Ľ��  
    if(sum == n)  
    {  
        // ��תlist  
        list1.reverse();  
        for(list<int>::iterator iter = list1.begin(); iter != list1.end(); iter++)  
            cout << *iter << " + ";  
        cout << n << endl;  
        list1.reverse();      
    }  
    int l=0, r=0;
    list1.push_front(n);      //���͵�01��������  
    l+=find_factor(sum-n, n-1);   //��n��n-1��������sum-n  
    list1.pop_front();  
    r+=find_factor(sum, n-1);     //����n��n-1��������sum   

    return l+r;
}  
  
int main()  
{  
    int sum, n;  
    cout << "��������Ҫ���ڶ��ٵ���ֵsum:" << endl;  
    cin >> sum;  
    cout << "��������Ҫ��1.....n������ȡֵ��n��" << endl;  
    cin >> n;  
    cout << "���п��ܵ����У����£�" << endl;  
    int count = find_factor(sum,n);  
    cout << "���� " << count <<" �����" << endl;
    return 0;  
}  
