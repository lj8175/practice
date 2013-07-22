#ifndef _PUSH_ENCRYPT_H__
#define _PUSH_ENCRYPT_H__

class CPushEncrypt
{
public:
    static char* RSA_Encrypt(char *str,char *keyPath);
    static char* RSA_Decrypt(char *str,char *keyPath);
};

#endif
