all: localtest lucas_meneghelli

localtest:
	- mkdir localtest
	- mkdir localtest/server
	- javac src/Message.java src/FileManager.java src/Chat.java src/Server.java -d localtest/server
	- mkdir localtest/lucas
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d localtest/lucas
	- base64 /dev/urandom | head -c 1M > localtest/lucas/lucas-01.chat
	- base64 /dev/urandom | head -c 1M > localtest/lucas/lucas-02.chat
	- mkdir localtest/pedro
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d localtest/pedro
	- base64 /dev/urandom | head -c 1M > localtest/pedro/pedro-01.chat
	- mkdir localtest/joao
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d localtest/joao
	- base64 /dev/urandom | head -c 1M > localtest/joao/joao-01.chat

lucas_meneghelli:
	- mkdir lucas_meneghelli
	- mkdir lucas_meneghelli/server
	- javac src/Message.java src/FileManager.java src/Chat.java src/Server.java -d lucas_meneghelli/server
	- mkdir lucas_meneghelli/paul
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d lucas_meneghelli/paul
	- base64 /dev/urandom | head -c 1M > lucas_meneghelli/paul/paul-01.chat
	- mkdir lucas_meneghelli/john
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d lucas_meneghelli/john
	- base64 /dev/urandom | head -c 1M > lucas_meneghelli/john/john-01.chat
	- mkdir lucas_meneghelli/mary
	- javac src/Message.java src/FileManager.java src/Chat.java src/Client.java -d lucas_meneghelli/mary
	- base64 /dev/urandom | head -c 1M > lucas_meneghelli/mary/mary-01.chat

.PHONY: clean

clean:
	-rm localtest -r
	-rm lucas_meneghelli -r
