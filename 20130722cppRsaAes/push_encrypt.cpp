#include "push_encrypt.h"
#include <string.h>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>


char* CPushEncrypt::RSA_Encrypt(char *str,char *keyPath)
{
    if ((str==NULL)||(keyPath==NULL))
    {
        return NULL;
    }
    char *p_en;
    RSA *p_rsa;
    FILE *file;
    int rsa_len;
    if((file=fopen(keyPath,"r"))==NULL){
        perror("open key file error");
        return NULL;    
    }   
    if((p_rsa=PEM_read_RSA_PUBKEY(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        return NULL;
    }   
    rsa_len=RSA_size(p_rsa);
    p_en=(char *)malloc(rsa_len+1);
    if (NULL==p_en)
    {
        return NULL;
    }
    memset(p_en,0,rsa_len+1);
    if(RSA_public_encrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_en,p_rsa,RSA_NO_PADDING)<0){
        return NULL;
    }
    RSA_free(p_rsa);
    fclose(file);
    return p_en;

}

char* CPushEncrypt::RSA_Decrypt(char *str,char *keyPath)
{
    if ((str==NULL)||(keyPath==NULL))
    {
        return NULL;
    }
    char *p_de;
    RSA *p_rsa;
    FILE *file;
    int rsa_len;
    if((file=fopen(keyPath,"r"))==NULL){
        perror("open key file error");
        return NULL;
    }
    if((p_rsa=PEM_read_RSAPrivateKey(file,NULL,NULL,NULL))==NULL){
        ERR_print_errors_fp(stdout);
        return NULL;
    }
    rsa_len=RSA_size(p_rsa);
    p_de=(char *)malloc(rsa_len+1);
    if (NULL==p_de)
    {
        return NULL;
    }
    memset(p_de,0,rsa_len+1);
    if(RSA_private_decrypt(rsa_len,(unsigned char *)str,(unsigned char*)p_de,p_rsa,RSA_NO_PADDING)<0){
        return NULL;
    }
    RSA_free(p_rsa);
    fclose(file);
    return p_de;
}
