#include <stdio.h>

int AddWithoutArithmetic(int num1, int num2)
{
        if(num2 == 0)
                return num1;
 
        int sum = num1 ^ num2;
        int carry = (num1 & num2) << 1;
 
        return AddWithoutArithmetic(sum, carry);
}

int main(){
    int num1 = 11;
    int num2 = 22;
    printf("%d + %d = %d\n", num1, num2, AddWithoutArithmetic(num1, num2));
}
