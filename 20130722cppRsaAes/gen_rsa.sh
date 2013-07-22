#!/usr/bin/env bash
openssl genrsa -out rsa.key 1024
#openssl genrsa -out test.key 512 
openssl rsa -in rsa.key -pubout -out rsa_pub.key
#openssl rsautl -encrypt -in hello.txt -inkey test_pub.key -pubin -out hello.en
#openssl rsautl -decrypt -in hello.en -inkey test.key -out hello.de
