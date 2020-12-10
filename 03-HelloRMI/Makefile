all: classes

classes: classes
	- mkdir bin
	- mkdir bin/server
	- javac src/Message.java src/FileManager.java src/Chat.java src/Server.java -d bin/server
	- mkdir bin/client01
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d bin/client01
	- base64 /dev/urandom | head -c 1M > bin/client01/paul-01.chat

.PHONY: clean

clean:
	-rm bin -r
