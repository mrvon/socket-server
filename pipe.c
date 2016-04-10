#include <unistd.h>
#include <stdio.h>

int main(int argc, char const* argv[])
{
    int fd[2];

    if (pipe(fd)) {
        fprintf(stderr, "create pipe pair failed.\n");
        return 0;
    }

	int recvctrl_fd = fd[0];
	int sendctrl_fd = fd[1];

    char send_buff[] = "Hello world";
    char recv_buff[1024] = {0};

    int write_count = write(sendctrl_fd, send_buff, sizeof(send_buff));
    int read_count = read(recvctrl_fd, recv_buff, sizeof(recv_buff));

    fprintf(stdout, "Write: %d Read: %d\n", write_count, read_count);
    fprintf(stdout, "Read: %s\n", recv_buff);

    return 0;
}
