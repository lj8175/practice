#include <stdio.h>
#include <dlfcn.h>

void showBacktrace(){
  void *runf= __builtin_return_address(0);
  printf("Running address[%X]\n",runf);
  void *retf= __builtin_return_address(1);
  printf("Return  address[%X]\n",retf);

#ifdef __cplusplus
  Dl_info dlinfo;

  if(!dladdr(runf,&dlinfo)){perror("addr not found\n");return;}
  fprintf(stderr,"Running function: %X %s (%s)\n",runf,dlinfo.dli_sname,dlinfo.dli_fname);

  if(!dladdr(retf,&dlinfo)){perror("addr not found\n");return;}
  fprintf(stderr,"Return  function: %X %s (%s)\n",retf,dlinfo.dli_sname,dlinfo.dli_fname);
#endif
}

int funcB(){
  showBacktrace();
  return 10;
}
int funcA(){
  return funcB();
}

int main(){
  funcA();
  return 0;
}
