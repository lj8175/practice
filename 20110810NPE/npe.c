#include <stdio.h>

struct String{
  int count;
};

int main(){
  struct String *s = NULL;
  printf("%d\n", s->count);
}
