all : client

client : client.c
	gcc $< -o $@ -lssl -lcrypto

clean :
	rm server
