/* In windows:
 * VC\include\stdarg.h
 * #define va_start _crt_va_start
 * #define va_arg _crt_va_arg
 * #define va_end _crt_va_end
 *
 * VC\include\vadefs.h
 * typedef char * va_list;
 * #define _crt_va_start(ap,v) ( ap = (va_list)_ADDRESSOF(v) + _INTSIZEOF(v) )
 * #define _crt_va_arg(ap,t) ( *(t *)((ap += _INTSIZEOF(t)) - _INTSIZEOF(t)) )
 * #define _crt_va_end(ap) ( ap = (va_list)0 )
 *
 * #define _ADDRESSOF(v) ( &(v) )
 * #define _INTSIZEOF(n) ( (sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1) )
 *
 * In linux: man va_start
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int myprintf(char* fmt,...){
  int i;
  va_list ap;

  va_start(ap,fmt);
  for(i=0;i<strlen(fmt);i++){
    switch(*(fmt+i)){
      case 'D':
        printf("%d",va_arg(ap,int));break;
      case 'C':
#if defined(_MSC_VER)
        printf("%c",va_arg(ap,char));break;
#elif defined(__GNUC__)
        printf("%c",va_arg(ap,int));break;
#endif
      default:
        printf("%c",*(fmt+i));
    }
  }
  va_end(ap);

}

void main(){
  char *s="sabcdefg";
  myprintf(" strlen(s)=D \n 98=C \n",strlen(s),98);
}
