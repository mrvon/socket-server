#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int
main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *ai_list;
    struct addrinfo *ai_ptr;

    int fd;
    int status;
    int j;

    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    status = getaddrinfo(argv[1], argv[2], &hints, &ai_list);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully connect(2).
       If socket(2) (or connect(2)) fails, we (close the socket
       and) try the next address. */

    for (ai_ptr = ai_list; ai_ptr != NULL; ai_ptr = ai_ptr->ai_next) {
        fd = socket(ai_ptr->ai_family, ai_ptr->ai_socktype,
                ai_ptr->ai_protocol);
        if (fd == -1)
            continue;

        if (connect(fd, ai_ptr->ai_addr, ai_ptr->ai_addrlen) != -1)
            break;                  /* Success */

        close(fd);
    }

    if (ai_ptr == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(ai_list);           /* No longer needed */

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    for (j = 3; j < argc; j++) {
        len = strlen(argv[j]) + 1;
        /* +1 for terminating null byte */

        if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

        if (write(fd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

        nread = read(fd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Received %ld bytes: %s\n", (long) nread, buf);
    }

    exit(EXIT_SUCCESS);
}

