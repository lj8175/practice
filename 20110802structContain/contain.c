#include <stddef.h>  //#define offsetof(TYPE, MEMBER) __builtin_offsetof (TYPE, MEMBER)
#include <stdio.h>
#include <stdlib.h>

#undef offsetof
#define offsetof(TYPE, MEMBER) ((size_t)&((TYPE *)0)->MEMBER)

#ifndef containerOf
#define containerOf(__item, __type, __field)			\
      ((__type *)(((char *) (__item)) - offsetof(__type, __field)))
#endif

#pragma pack(1)

struct s {
  int i;
  char c;
  double d;
  char a[];
};

struct s member;
#define m_d member.d

int main(void){

  struct s *mem;
  m_d = time(NULL);
  mem = containerOf(&m_d, struct s, d);
  printf("%.0f == %.0f\n", m_d, mem->d);


  /* Output is compiler dependent */

  printf("offsets: i=%ld; c=%ld; d=%ld a=%ld\n",
          (long) offsetof(struct s, i),
          (long) offsetof(struct s, c),
          (long) offsetof(struct s, d),
          (long) offsetof(struct s, a));
  printf("sizeof(struct s)=%ld\n", (long) sizeof(struct s));

  exit(EXIT_SUCCESS);
}
