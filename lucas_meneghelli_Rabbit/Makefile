localtest:
	- mkdir localtest
	- mkdir localtest/server
	- cp src/server.rb localtest/server
	- mkdir localtest/lucas
	- cp src/client.rb localtest/lucas
	- base64 /dev/urandom | head -c 1M > localtest/lucas/lucas-01.chat
	- base64 /dev/urandom | head -c 1M > localtest/lucas/lucas-02.chat
	- mkdir localtest/pedro
	- cp src/client.rb localtest/pedro
	- base64 /dev/urandom | head -c 1M > localtest/pedro/pedro-01.chat
	- mkdir localtest/joao
	- cp src/client.rb localtest/joao
	- base64 /dev/urandom | head -c 1M > localtest/joao/joao-01.chat

lucas_meneghelli:
	- mkdir lucas_meneghelli
	- mkdir lucas_meneghelli/server
	- cp src/server.rb lucas_meneghelli/server
	- mkdir lucas_meneghelli/paul
	- cp src/client.rb lucas_meneghelli/paul
	- base64 /dev/urandom | head -c 1M > lucas_meneghelli/paul/paul-01.chat
	- mkdir lucas_meneghelli/john
	- cp src/client.rb lucas_meneghelli/john
	- mkdir lucas_meneghelli/mary
	- cp src/client.rb lucas_meneghelli/mary

.PHONY: clean

clean:
	-rm src/*.chat
	-rm src/*.serv
	-rm src/*.client*
	-rm localtest -r
	-rm lucas_meneghelli -r
