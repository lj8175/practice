##一、生成用于签发client和server证书的ca根证书。
mkdir -p ca
 
#生成CA私钥，1024位RSA私钥，这里简单起见，没有对私钥进行加密。
openssl genrsa -out ca/ca.key 1024
 
#生成ca的证书请求文件
openssl req -new -out ca/ca.csr -key ca/ca.key
 
#用CA私钥进行自签名，生成ca的证书文件
openssl x509 -req -in ca/ca.csr -out ca/ca.cert -signkey ca/ca.key -days 365
#days参数指定过期时间
 
##二、利用ca签发生成客户端证书。
mkdir -p client
 
#生成client私钥
openssl genrsa -out client/client.key 1024
 
#生成client证书请求文件
openssl req -new -out client/client.csr -key client/client.key
 
#ca签发client证书
openssl x509 -req -in client/client.csr -out client/client.crt -signkey client/client.key \
 -CA ca/ca.cert -CAkey ca/ca.key -CAcreateserial -days 365
 
##三、利用ca签发生成服务端证书。
mkdir -p server
 
#生成server私钥
openssl genrsa -out server/server.key 1024
 
#生成server证书请求文件
openssl req -new -out server/server.csr -key server/server.key
 
#利用ca签发server端证书
openssl x509 -req -in server/server.csr -out server/server.crt -signkey server/server.key \
 -CA ca/ca.cert -CAkey ca/ca.key -CAcreateserial -days 365
