all: socket-server echo-server echo-client test

socket-server : socket_server.c test.c
	gcc -g -Wall -o $@ $^ -lpthread

echo-server : socket_server.c echo_server.c
	gcc -g -Wall -o $@ $^ -lpthread

echo-client : socket_server.c echo_client.c
	gcc -g -Wall -o $@ $^ -lpthread

test : socket_server.c test.c
	gcc -g -Wall -o $@ $^ -lpthread

clean:
	rm socket-server echo-server echo-client test
