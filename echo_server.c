#include "socket_server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>


static void
loop(struct socket_server *ss) {
	int l = socket_server_listen(ss, 200, "127.0.0.1", 8888, 32);

	printf("listening %d\n", l);

	socket_server_start(ss, 201, l);

	struct socket_message result;

	for (;;) {
		int type = socket_server_poll(ss, &result, NULL);
		// DO NOT use any ctrl command (socket_server_close , etc. ) in this thread.
		switch (type) {
		case SOCKET_EXIT:
            break;
		case SOCKET_DATA:
			printf("message(%lu) [id=%d] size=%d\n", result.opaque, result.id, result.ud);
            socket_server_send(ss, result.id, result.data, result.ud);
			break;
		case SOCKET_CLOSE:
			printf("close(%lu) [id=%d]\n", result.opaque, result.id);
			break;
		case SOCKET_OPEN:
			printf("open(%lu) [id=%d] %s\n", result.opaque, result.id, result.data);
			break;
		case SOCKET_ERROR:
			printf("error(%lu) [id=%d]\n", result.opaque, result.id);
			break;
		case SOCKET_ACCEPT:
			printf("accept(%lu) [id=%d %s] from [%d]\n", result.opaque, result.ud, result.data, result.id);
            socket_server_start(ss, 202, result.ud);
			break;
		}
	}
}

int main(int argc, char const* argv[])
{
	struct socket_server * ss = socket_server_create();

	loop(ss);

	socket_server_release(ss);

    return 0;
}
