#include <openssl/ssl.h>
#include <openssl/crypto.h>
#include <openssl/err.h>
#include <openssl/bio.h>
#include <openssl/pkcs12.h>

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 4433
#define CERT_PATH "./sslclientcert.pem"
#define KEY_PATH  "./sslclientkey.pem"
#define CAFILE "./demoCA/cacert.pem"

static SSL_CTX *g_sslctx = NULL;



int connect_to_server(int fd ,char* ip,int port){
    struct sockaddr_in svr;
    memset(&svr,0,sizeof(svr));
    svr.sin_family = AF_INET;
    svr.sin_port = htons(port);
    if(inet_pton(AF_INET,ip,&svr.sin_addr) <= 0){
        printf("invalid ip address!\n");
        return -1;
    }

    if(connect(fd,(struct sockaddr *)&svr,sizeof(svr))){
        printf("connect error : %s\n",strerror(errno));
        return -1;
    }
   
    return 0;
}



//客户端证书内容输出
void print_client_cert(char* path)
{
    X509 *cert =NULL;
    FILE *fp = NULL;
    fp = fopen(path,"rb");

    //从证书文件中读取证书到x509结构中，passwd为1111,此为生成证书时设置的
    cert = PEM_read_X509(fp, NULL, NULL, "1111");
    X509_NAME *name=NULL;
    char buf[8192]={0};
    BIO *bio_cert = NULL;
    //证书持有者信息
    name = X509_get_subject_name(cert);
    X509_NAME_oneline(name,buf,8191);
    printf("ClientSubjectName:%s\n",buf);
    memset(buf,0,sizeof(buf));
    bio_cert = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(bio_cert, cert);
    //证书内容
    BIO_read( bio_cert, buf, 8191);
    printf("CLIENT CERT:\n%s\n",buf);
    if(bio_cert)BIO_free(bio_cert);
    fclose(fp);
    if(cert) X509_free(cert);
}

//在SSL握手时，验证服务端证书时会被调用，res返回值为1则表示验证成功，否则为失败
static int verify_cb(int res, X509_STORE_CTX *xs)
{
    printf("SSL VERIFY RESULT :%d\n",res);
    switch (xs->error)
    {
        case X509_V_ERR_UNABLE_TO_GET_CRL:
            printf(" NOT GET CRL!\n");
            return 1;
        default :
            break;
    }
    return res;
}



int sslctx_init()
{

#if 0
    BIO *bio = NULL;
    X509 *cert = NULL;
    STACK_OF(X509) *ca = NULL;
    EVP_PKEY *pkey =NULL;
    PKCS12* p12 = NULL;
    X509_STORE *store =NULL;
    int error_code =0;
#endif

    int ret =0;
    print_client_cert(CERT_PATH);
    //registers the libssl error strings
    SSL_load_error_strings();
    //registers the available SSL/TLS ciphers and digests
    SSL_library_init();
    //creates a new SSL_CTX object as framework to establish TLS/SSL
    g_sslctx = SSL_CTX_new(SSLv23_client_method());
    if(g_sslctx == NULL){
        ret = -1;
        goto end;
    }

    //passwd is supplied to protect the private key,when you want to read key
    SSL_CTX_set_default_passwd_cb_userdata(g_sslctx,"123456");

    //set cipher ,when handshake client will send the cipher list to server
    SSL_CTX_set_cipher_list(g_sslctx,"HIGH:MEDIA:LOW:!DH");

    //SSL_CTX_set_cipher_list(g_sslctx,"AES128-SHA");
    //set verify ,when recive the server certificate and verify it
    //and verify_cb function will deal the result of verification
    SSL_CTX_set_verify(g_sslctx, SSL_VERIFY_PEER, verify_cb);

    
    //sets the maximum depth for the certificate chain verification that shall
    //be allowed for ctx
    SSL_CTX_set_verify_depth(g_sslctx, 10);

    //load the certificate for verify server certificate, CA file usually load
    SSL_CTX_load_verify_locations(g_sslctx,CAFILE, NULL);

    //load user certificate,this cert will be send to server for server verify
    if(SSL_CTX_use_certificate_file(g_sslctx,CERT_PATH,SSL_FILETYPE_PEM) <= 0){
        printf("certificate file error!\n");
        ret = -1;
        goto end;
    }

    //load user private key
    if(SSL_CTX_use_PrivateKey_file(g_sslctx,KEY_PATH,SSL_FILETYPE_PEM) <= 0){
        printf("privatekey file error!\n");
        ret = -1;
        goto end;
    }

    if(!SSL_CTX_check_private_key(g_sslctx)){
        printf("Check private key failed!\n");
        ret = -1;
        goto end;
    }

end:
    return ret;
}



void sslctx_release()
{
    EVP_cleanup();
    if(g_sslctx){
        SSL_CTX_free(g_sslctx);
    }
    g_sslctx= NULL;
}

//打印服务端证书相关内容
void print_peer_certificate(SSL *ssl)
{
    X509* cert= NULL;
    X509_NAME *name=NULL;
    char buf[8192]={0};
    BIO *bio_cert = NULL;

    //获取server端证书
    cert = SSL_get_peer_certificate(ssl);

    //获取证书拥有者信息
    name = X509_get_subject_name(cert);
    X509_NAME_oneline(name,buf,8191);
    printf("ServerSubjectName:%s\n",buf);
    memset(buf,0,sizeof(buf));
    bio_cert = BIO_new(BIO_s_mem());
    PEM_write_bio_X509(bio_cert, cert);
    BIO_read( bio_cert, buf, 8191);
    //server证书内容
    printf("SERVER CERT:\n%s\n",buf);
    if(bio_cert)BIO_free(bio_cert);
    if(cert)X509_free(cert);
}



int main(int argc, char** argv){
    int fd = -1 ,ret = 0;
    SSL *ssl = NULL;
    char buf[1024] ={0};
    //初始化SSL
    if(sslctx_init()){
        printf("sslctx init failed!\n");
        goto out;
    }

    //客户端socket建立tcp连接
    fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd < 0){
        printf("socket error:%s\n",strerror(errno));
        goto out;
    }
    if(connect_to_server(fd ,IP,PORT)){
        printf("can't connect to server:%s:%d\n",IP,PORT);
        goto out;
    }

    ssl = SSL_new(g_sslctx);
    if(!ssl){
        printf("can't get ssl from ctx!\n");
        goto out;
    }

    SSL_set_fd(ssl,fd);

    //建立与服务端SSL连接
    ret = SSL_connect(ssl);
    if(ret != 1){
        int err = ERR_get_error();
        printf("Connect error code: %d ,string: %s\n",err,ERR_error_string(err,NULL));
        goto out;
    }

    //输入服务端证书内容
    print_peer_certificate(ssl);

    SSL_write(ssl,"sslclient test!",strlen("sslclient test!"));
    SSL_read(ssl,buf,1024);
    printf("%s\n", buf);

    //关闭SSL连接
    SSL_shutdown(ssl);

out:
    if(fd >0)close(fd);
    if(ssl != NULL){
        SSL_free(ssl);
        ssl = NULL;
    }
    if(g_sslctx != NULL) sslctx_release();
    return 0;
}
