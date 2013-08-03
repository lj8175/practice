#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>


#define BUFFER_SIZE 1024
#define CHECK_RET(x) {if(x){printf("error: %s %d\n", __FILE__, __LINE__);return ret;}}

int AES_Encrypt(const char *key, const char *str, int strLen, char* enStr) 
{
    if ((str==NULL)||(key==NULL)||(enStr==NULL)||(strLen<=0))
    {
        return -1;
    }
    unsigned char iv[AES_BLOCK_SIZE];
    bzero(iv, AES_BLOCK_SIZE);
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -1;
    }
    AES_cfb8_encrypt((unsigned char*)str, (unsigned char*)enStr, strLen, &aes, iv, NULL, AES_ENCRYPT);
    return 0;
}

int AES_Decrypt(const char *key, const char *str, int strLen, char* deStr)
{
    if ((str==NULL)||(key==NULL)||(deStr==NULL)||(strLen<=0))
    {
        return -1;
    }
    unsigned char iv[AES_BLOCK_SIZE];
    bzero(iv, AES_BLOCK_SIZE);
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -1;
    }
    AES_cfb8_encrypt((unsigned char*)str, (unsigned char*)deStr, strLen, &aes, iv, NULL, AES_DECRYPT);
    return 0;
}


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
    if (argc>1)
    {
        source = argv[1];
    }
    const int len = strlen(source);
    int ret = 0;
    char aes_en[BUFFER_SIZE]={0};
    char aes_de[BUFFER_SIZE]={0};

    printf("size: %d, source  is  : %s\n", len, source);
    ret = AES_Encrypt(source,source,len,aes_en); CHECK_RET(ret);
    hex_print(aes_en, strlen(source));
    ret = AES_Decrypt(source,aes_en,len,aes_de); CHECK_RET(ret);
    aes_de[len] = '\0';
    printf("size: %d, decrypt is  : %s\n", len, aes_de);
    return 0;
}
