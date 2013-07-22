#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "push_encrypt.h"

#define OPENSSLKEY "rsa.key"
#define PUBLICKEY  "rsa_pub.key"


int main()
{
    char *source="Hello World!";
    char *ptr_en,*ptr_de;
    printf("len: %d, source  is  : %s\n", strlen(source), source);
    ptr_en=CPushEncrypt::RSA_Encrypt(source,PUBLICKEY);
    printf("len: %d, encrypt is  : %s\n\n\n", strlen(ptr_en), ptr_en);
    ptr_de=CPushEncrypt::RSA_Decrypt(ptr_en,OPENSSLKEY);
    printf("len: %d, decrypt is  : %s\n", strlen(ptr_de), ptr_de);
    if(ptr_en!=NULL){
        free(ptr_en);
    }   
    if(ptr_de!=NULL){
        free(ptr_de);
    }   
    return 0;
}
