#ifndef _PUSH_ENCRYPT_H__
#define _PUSH_ENCRYPT_H__

class CPushEncrypt
{
public:
    /*
     *keyPath is the path of the key
     *str is the string need to encrypt
     *enStr is the encrypted of str(the length is decided by key len 1024:128 512:64)
    */
    static int RSA_Encrypt(const char *keyPath, const char *str, char *enStr);
    static int RSA_Decrypt(const char *keyPath, const char *str, char *deStr);
    //note : key is 128 bit, 16 byte;
    /*
     *key is 128 bit, 16 byte;
     *str is which need to encrypt
     *strLen is the lenth of str
     *enStr is the encrypted str to user
     *enStrLen is the length of enStr
    */
    static int AES_Encrypt(const char *key, const char *str, int strLen, char* enStr, int &enStrLen);
    static int AES_Decrypt(const char *key, const char *str, int strLen, char* deStr);
};

#endif
