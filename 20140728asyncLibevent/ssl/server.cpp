#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
 
#include <openssl/ssl.h>
#include <event2/event.h>
#include <event2/util.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/bufferevent_ssl.h>
 
static void ssl_read_cb(struct bufferevent *, void *);
static void ssl_event_cb(struct bufferevent *, short, void *);
 
static void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd,
        struct sockaddr *address, int socklen, void *ctx)
{
    struct event_base *base = evconnlistener_get_base(listener);
    SSL *ssl = SSL_new((SSL_CTX *)ctx);
    if (NULL == ssl)
    {
        fprintf(stderr, "Could not create new SSL\n");
        exit(EXIT_FAILURE);
    }
    struct bufferevent *bev = bufferevent_openssl_socket_new(base, fd, ssl,
            BUFFEREVENT_SSL_ACCEPTING, BEV_OPT_CLOSE_ON_FREE);
    if (!bev)
    {
        fprintf(stderr, "Could not create new ssl bufferevent\n");
        exit(EXIT_FAILURE);
    }
    
    bufferevent_setcb(bev, ssl_read_cb, NULL, ssl_event_cb, NULL);
 
    bufferevent_enable(bev, EV_READ | EV_WRITE);
}
 
static void accept_error_cb(struct evconnlistener *listener, void *ctx)
{
    int err = EVUTIL_SOCKET_ERROR();
    fprintf(stderr, "Got an error %d on listener: %s\n", 
            err, evutil_socket_error_to_string(err));
    exit(EXIT_FAILURE);
}
 
static void ssl_read_cb(struct bufferevent *bev, void *arg)
{
    char recvbuf[1024] = {'\0'};
    if (bufferevent_read(bev, recvbuf, 1024) > 0)
    {
        printf("Recv from client: %s\n", recvbuf);
    }
}
 
static void ssl_event_cb(struct bufferevent *bev, short events, void *arg)
{
    if (events & BEV_EVENT_CONNECTED)
    {
        #define MSG "Hello client"
        printf("Server: client connected!\n");
        bufferevent_write(bev, MSG, sizeof(MSG));
        bufferevent_write(bev, MSG "1234", sizeof(MSG "1234"));
    }
    else if (events & BEV_EVENT_EOF)
    {
        printf("Server: client disconnected!\n");
        bufferevent_free(bev);
    }
    else
    {
        int err = EVUTIL_SOCKET_ERROR();
        fprintf(stderr, "Got an error %d on ssl socket: %s\n", 
            err, evutil_socket_error_to_string(err));
        bufferevent_free(bev);
    }
}
 
SSL_CTX* init_ssl_ctx(const char *certfile, const char *keyfile, const char *cafile)
{
    /* Load encryption & hashing algorithms for the SSL program */
    SSL_library_init();
 
    /* Load the error strings for SSL & CRYPTO APIs */
    SSL_load_error_strings();
 
    /* Create an SSL_METHOD structure (choose an SSL/TLS protocol version) */
    const SSL_METHOD *meth = SSLv23_server_method();
 
    /* Create an SSL_CTX structure */
    SSL_CTX *ctx = SSL_CTX_new (meth);
    if (NULL == ctx)
    {
        printf("Could not new SSL_CTX\n");
        return NULL;
    }
 
    /* Load the CA cert file*/
    if (SSL_CTX_load_verify_locations(ctx, cafile, NULL) <= 0)
    {
        printf("Could not load ca cert file\n");
    }
 
    /* Load the server certificate into the SSL_CTX structure */
    if (SSL_CTX_use_certificate_file(ctx, certfile, SSL_FILETYPE_PEM) <= 0)
    {
        printf("Could not use certificate file\n");
    }
 
    /* Load the private-key corresponding to the client certificate */
    if (SSL_CTX_use_PrivateKey_file(ctx, keyfile, SSL_FILETYPE_PEM) <= 0)
    {
        printf("Could not use private key file\n");
    }
 
    /* Check if the client certificate and private-key matches */
    if (!SSL_CTX_check_private_key(ctx))
    {
        printf("Private key does not match certfile\n");
    }
 
    return ctx;
}
 
int main(int argc, char *argv[])
{
    struct event_base *base = event_base_new();
    if (!base)
    {
        fprintf(stderr, "Could not open evnet base\n");
        exit(EXIT_FAILURE);
    }
 
    SSL_CTX *ctx = init_ssl_ctx("./PEMS/server/server.crt", "./PEMS/server/server.key", "./PEMS/ca/ca.cert");
    if (NULL == ctx)
    {
        exit(EXIT_FAILURE);
    }
 
    struct sockaddr_in servaddr;
    struct sockaddr *sa = (struct sockaddr *)&servaddr;
    int socklen = sizeof(servaddr);
    if (evutil_parse_sockaddr_port("127.0.0.1:1234", sa, &socklen) == -1)
    {
        fprintf(stderr, "Could not parse address\n");
        exit(EXIT_FAILURE);
    }
 
    struct evconnlistener *listener = evconnlistener_new_bind(base, accept_conn_cb, ctx,
            LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, sa, sizeof(servaddr));
    if (NULL == listener)
    {
        fprintf(stderr, "Could not create listener\n");
        exit(EXIT_FAILURE);
    }
    evconnlistener_set_error_cb(listener, accept_error_cb);
 
    event_base_dispatch(base);
 
    return 0;
}
