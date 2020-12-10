all: classes

classes: classes
	- mkdir bin
	- mkdir bin/server
	- javac src/Message.java src/FileManager.java src/Chat.java src/Server.java -d bin/server
	- mkdir bin/lucas
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d bin/lucas
	- base64 /dev/urandom | head -c 1M > bin/lucas/lucas-01.chat
	- base64 /dev/urandom | head -c 1M > bin/lucas/lucas-02.chat
	- mkdir bin/pedro
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d bin/pedro
	- base64 /dev/urandom | head -c 1M > bin/pedro/pedro-01.chat
	- mkdir bin/joao
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d bin/joao
	- base64 /dev/urandom | head -c 1M > bin/joao/joao-01.chat

.PHONY: clean

clean:
	-rm bin -r
