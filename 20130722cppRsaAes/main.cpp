#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "push_encrypt.h"
#include <openssl/aes.h>

#define OPENSSLKEY "rsa.key"
#define PUBLICKEY  "rsa_pub.key"

#define BUFFER_SIZE 1024
#define CHECK_RET(x) {if(x){printf("error: %s %d\n", __FILE__, __LINE__);return ret;}}

void hex_print(char* str, int size)
{
    printf("size: %d, hex content: ", size);
    for (int i=0; i<size; ++i) 
    {
         printf("%x%x", (str[i] >> 4) & 0xf, str[i] & 0xf);
    }
    printf("\n");
}


int main(int argc, char** argv)
{
    char *source=(char*)"1234567890123456";
    if (argc>0)
    {
        source = argv[1];
    }
    int ret = 0;
    char ptr_en[BUFFER_SIZE],ptr_de[BUFFER_SIZE],aes_en[BUFFER_SIZE],aes_de[BUFFER_SIZE];
    printf("len: %d, source  is  : %s\n", strlen(source), source);
    ret=CPushEncrypt::RSA_Encrypt(PUBLICKEY,source,ptr_en); CHECK_RET(ret);
    hex_print(ptr_en, strlen(ptr_en));
    ret=CPushEncrypt::RSA_Decrypt(OPENSSLKEY,ptr_en,ptr_de); CHECK_RET(ret);
    printf("len: %d, decrypt is  : %s\n", strlen(ptr_de), ptr_de);

    int aes_len=0;
    ret = CPushEncrypt::AES_Encrypt(ptr_de,source,strlen(source),aes_en,aes_len); CHECK_RET(ret);
    hex_print(ptr_de, aes_len);
    ret = CPushEncrypt::AES_Decrypt(ptr_de,aes_en, aes_len, aes_de); CHECK_RET(ret);
    printf("len: %d, decrypt is  : %s\n", strlen(aes_de), aes_de);
    return 0;
}
