all: Server

CC = g++
Server: server.cc
	$(CC) -o server server.cc TLS.cc PathProcess.cc -lssl -lcrypto -lstdc++fs


.PHONY: clean genCrt genCrtFirst verify server
clean:
	rm server client
genCrtFirst:
	mkdir -p Crt
	openssl genrsa -out Crt/CA.key 2048
	openssl req -new -sha256 -x509 -days 365 -key Crt/CA.key -out Crt/CA.crt 
	openssl genrsa -out Crt/server.key 2048
	openssl req -new -key Crt/server.key -out Crt/server.csr 
	openssl x509 -sha256 -req -in Crt/server.csr -days 365 -CA Crt/CA.crt -CAkey Crt/CA.key -CAcreateserial -out Crt/server.crt 
genCrt:
	openssl genrsa -out Crt/server.key 2048
	openssl req -new -key Crt/server.key -out Crt/server.csr 
	openssl x509 -sha256 -req -in Crt/server.csr -days 365 -CA Crt/CA.crt -CAkey Crt/CA.key -CAserial Crt/CA.srl -out Crt/server.crt 
verify:
	openssl verify -CAfile Crt/CA.crt Crt/server.crt
server:
	./server

