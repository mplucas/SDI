all: tests tools.o client server main_file distribute

tests: tests
	- mkdir ../localtest
	- mkdir ../localtest/server
	- mkdir ../localtest/client01
	- base64 /dev/urandom | head -c 1M > ../localtest/client01/lucas-01.chat
	- base64 /dev/urandom | head -c 1M > ../localtest/client01/lucas-02.chat
	- mkdir ../localtest/client02
	- base64 /dev/urandom | head -c 1M > ../localtest/client02/pedro-01.chat
	- mkdir ../localtest/client03
	- base64 /dev/urandom | head -c 1M > ../localtest/client03/joao-01.chat
	- mkdir ../lucas_meneghelli
	- mkdir ../lucas_meneghelli/server
	- mkdir ../lucas_meneghelli/paul
	- base64 /dev/urandom | head -c 1M > ../lucas_meneghelli/paul/paul-01.chat
	- mkdir ../lucas_meneghelli/john
	- mkdir ../lucas_meneghelli/mary

hw.h: hw.x
	rpcgen hw.x
	
hw_svc.c hw_clnt.c hw_client.c: hw.h

hw_xdr.o: hw_xdr.c
	gcc -o hw_xdr.o -c hw_xdr.c

tools.o: tools.c
	gcc -c tools.c -o tools.o

main_file: main_file.c tools.o
	gcc -o main_file tools.o main_file.c

client: hw_client.o hw_clnt.o hw_xdr.o
	gcc -o client tools.o hw_client.o hw_clnt.o hw_xdr.o -lnsl

server: hw_server.o hw_svc.o
	gcc -o server tools.o hw_server.o hw_svc.o hw_xdr.o -lnsl

distribute: distribute
	-cp server ../lucas_meneghelli/server
	-cp client ../lucas_meneghelli/john
	-cp client ../lucas_meneghelli/paul
	-cp client ../lucas_meneghelli/mary
	-cp server ../localtest/server
	-cp client ../localtest/client01
	-cp client ../localtest/client02
	-cp client ../localtest/client03

.PHONY: clean

clean:
	-rm *.o
	-rm client*
	-rm ../client*/client
	-rm ../client*/*.client*
	-rm server*
	-rm main_file
	-rm hw.h
	-rm hw_clnt.c
	-rm hw_svc.c
	-rm ../localtest -r
	-rm ../lucas_meneghelli -r
