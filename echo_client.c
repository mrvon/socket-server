#include "socket_server.h"

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>
#include <pthread.h>


static void*
loop(void* ud) {
    struct socket_server *ss = ud;
	struct socket_message result;

	for (;;) {
		int type = socket_server_poll(ss, &result, NULL);
		// DO NOT use any ctrl command (socket_server_close , etc. ) in this thread.
		switch (type) {
		case SOCKET_EXIT:
            break;
		case SOCKET_DATA:
			printf("message(%lu) [id=%d] size=%d\n", result.opaque, result.id, result.ud);
            printf("%s", result.data);
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
			break;
		}
	}

    return NULL;
}

int main(int argc, char const* argv[])
{
	struct socket_server * ss = socket_server_create();
    int l = socket_server_connect(ss, 200, "127.0.0.1", 8888);
	socket_server_start(ss, 201, l);

    pthread_t pid;
    pthread_create(&pid, NULL, loop, (void*)ss);

    const int size = 1024;
    while (1) {
        char* line = (char*)malloc(size);
        assert(line);

        if (! fgets(line, size, stdin)) {
            break;
        }

        socket_server_send(ss, l, line, strlen(line));
    }

	socket_server_exit(ss);
	pthread_join(pid, NULL); 

	socket_server_release(ss);

    return 0;
}
