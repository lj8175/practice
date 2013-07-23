#include "push_encrypt.h"
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/aes.h>


int CPushEncrypt::RSA_Encrypt(const char *keyPath, const char *str, char *enStr)
{
    if ((str==NULL)||(keyPath==NULL)||(enStr==NULL))
    {
        return -1;
    }
    RSA *p_rsa;
    FILE *file;
    int rsa_len;
    if((file=fopen(keyPath,"r"))==NULL){
        return -1;    
    }   
    if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
        return -1;
    }   
    rsa_len=RSA_size(p_rsa);
    if(RSA_public_encrypt(rsa_len,(unsigned char *)str,(unsigned char*)enStr,p_rsa,RSA_NO_PADDING)<0){
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
    if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char*)deStr,p_rsa,RSA_NO_PADDING)<0){
        return -1;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 0;
}


int CPushEncrypt::AES_Encrypt(const char *key, const char *str, int strLen, char* enStr, int &enStrLen)
{
    if ((str==NULL)||(key==NULL)||(enStr==NULL)||(strLen<=0))
    {
        return -1;
    }
    unsigned char iv[AES_BLOCK_SIZE];
    bzero(iv, AES_BLOCK_SIZE);
    if ((strLen+1)%AES_BLOCK_SIZE == 0)
    {
        enStrLen = strLen + 1;
    } 
    else 
    {
        enStrLen = ((strLen+1)/AES_BLOCK_SIZE+1) * AES_BLOCK_SIZE;
    }
    unsigned char* p_in = NULL;
    p_in = (unsigned char*)calloc(enStrLen, sizeof(unsigned char));
    if (p_in==NULL)
    {
        return -1;
    }
    strncpy((char*)p_in, str, strlen(str));
    AES_KEY aes;
    if (AES_set_encrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -1;
    }
    AES_cbc_encrypt(p_in, (unsigned char*)enStr, enStrLen, &aes, iv, AES_ENCRYPT);
    free(p_in);
    return 0;
}

int CPushEncrypt::AES_Decrypt(const char *key, const char *str, int strLen, char* deStr)
{
    if ((str==NULL)||(key==NULL)||(deStr==NULL)||(strLen<=0))
    {
        return -1;
    }
    unsigned char iv[AES_BLOCK_SIZE];
    bzero(iv, AES_BLOCK_SIZE);
    if ((strLen%AES_BLOCK_SIZE)!=0)
    {
        return -1;
    }
    AES_KEY aes;
    if (AES_set_decrypt_key((unsigned char*)key, 128, &aes) < 0)
    {
        return -1;
    }
    AES_cbc_encrypt((unsigned char*)str, (unsigned char*)deStr, strLen, &aes, iv, AES_DECRYPT);
    return 0;
}
