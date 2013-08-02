#include "push_encrypt.h"
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/aes.h>

#define AES_KEY_LEN 16

int CPushEncrypt::RSA_Encrypt(const char *keyPath, const char *str, char *enStr)
{
    if ((str==NULL)||(keyPath==NULL)||(enStr==NULL))
    {
        return -1;
    }
    RSA *p_rsa;
    FILE *file;
    if((file=fopen(keyPath,"r"))==NULL){
        return -1;    
    }   
    if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
        return -1;
    }   
    if(RSA_public_encrypt(AES_KEY_LEN,(unsigned char *)str,(unsigned char*)enStr,p_rsa,RSA_PKCS1_PADDING)<0){
        return -1;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 0;

}

int CPushEncrypt::RSA_Decrypt(const char *keyPath, const char *str, char *deStr)
{
    if ((str==NULL)||(keyPath==NULL)||(deStr==NULL))
    {
        return -1;
    }
    RSA *p_rsa;
    FILE *file;
    int rsa_len;
    if((file=fopen(keyPath,"r"))==NULL){
        return -1;
    }
    if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
        return -1;
    }
    rsa_len=RSA_size(p_rsa);
    if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char*)deStr,p_rsa,RSA_PKCS1_PADDING)<0){
        return -1;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 0;
}


int CPushEncrypt::AES_Encrypt(const char *key, const char *str, int strLen, char* enStr, int random)
{
    if ((str==NULL)||(key==NULL)||(enStr==NULL)||(strLen<=0))
    {
        return -1;
    }
    unsigned char iv[AES_BLOCK_SIZE];
    bzero(iv, AES_BLOCK_SIZE);
    for (int i = 0; i < 4; i++) {
        int pos = i * 4;
        char value = (char)((random >> 8 * i) & 0xff);
        iv[pos] = value;
        iv[pos + 1] = value;
        iv[pos + 2] = value;
        iv[pos + 3] = value;
    }
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -1;
    }
    AES_cfb8_encrypt((unsigned char*)str, (unsigned char*)enStr, strLen, &aes, iv, NULL, AES_ENCRYPT);
    return 0;
}

int CPushEncrypt::AES_Decrypt(const char *key, const char *str, int strLen, char* deStr, int random)
{
    if ((str==NULL)||(key==NULL)||(deStr==NULL)||(strLen<=0))
    {
        return -1;
    }
    unsigned char iv[AES_BLOCK_SIZE];
    bzero(iv, AES_BLOCK_SIZE);
    for (int i = 0; i < 4; i++) {
        int pos = i * 4;
        char value = (char)((random >> 8 * i) & 0xff);
        iv[pos] = value;
        iv[pos + 1] = value;
        iv[pos + 2] = value;
        iv[pos + 3] = value;
    }
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -1;
    }
    AES_cfb8_encrypt((unsigned char*)str, (unsigned char*)deStr, strLen, &aes, iv, NULL, AES_DECRYPT);
    return 0;
}
