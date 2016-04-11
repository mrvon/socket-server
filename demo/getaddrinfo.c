#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char const* argv[]) {
    const char* host = "127.0.0.1";
    int port = 8888;

    int status;

    struct addrinfo ai_hints;
    struct addrinfo *ai_list = NULL;

    char port_str[16];
    sprintf(port_str, "%d", port);
    memset(&ai_hints, 0, sizeof(ai_hints));

    ai_hints.ai_family = AF_UNSPEC;
    ai_hints.ai_socktype = SOCK_DGRAM;
    ai_hints.ai_protocol = IPPROTO_UDP;

    status = getaddrinfo(host, port_str, &ai_hints, &ai_list);

    if (status != 0) {
        return -1;
    }

    freeaddrinfo(ai_list);
}
