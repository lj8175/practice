#include <stdio.h>

static int same = 8;

int func1(){
  static int same;
  printf("same in func1 %d\n", same);
}

int func2(){
  static int same;
  printf("same in func2 %d\n", same);
}

int main(){
  static int same;
  same = 5;
  func1();
  func2();
  printf("same in main  %d\n", same);
}
//output 0 0 5
