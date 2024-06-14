#include "../header/client.h"

void usage(char *argv0)
{
    fprintf(stderr, "usage: %s host port username\n", argv0);
    exit(1);
}

void connection(char *host, int port, char *username, int dest_socket)
{
    int connect_result;
    struct sockaddr_in addr;
    char status;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    inet_aton(host, &(addr.sin_addr));

    connect_result = connect(dest_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (connect_result < 0) {
        perror("Connect result");
        exit(1);
    }

    send(dest_socket, username, USERNAME_SIZE, 0);

    recv(dest_socket, &status, 1, 0);
    if (status == SUCCESS) {
        printf(WELCOME_MSG);
    } else {
        printf(AFULL_MSG);
        exit(1);
    }
}

int main(int argc, char *argv[])
{
    char *host;
    char *username;
    int port;
    int dest_socket;
    int recv_result;
    char ack;
    unsigned char buf[SIZE] = {0};
    unsigned char crc_buf[SIZE] = {0};
    unsigned char buf_to_send[2 * SIZE] = {0};
    size_t size_msg;

    if (argc != 4) {
        usage(argv[0]);
        exit(1);
    }

    host = argv[1];
    port = atoi(argv[2]);
    username = argv[3];

    dest_socket = socket(AF_INET, SOCK_STREAM, 0);

    connection(host, port, username, dest_socket);

    while (1) {
        memset(buf, 0, SIZE);
        memset(buf_to_send, 0, 2 * SIZE);

        printf("\e[1;37;44m %s \e[0;34m\e[0m ", username);
        fgets((char *)buf, SIZE, stdin);

        size_msg = strlen((char *)buf) + 1;

        for (size_t i = 0; i < size_msg; i++) {
            crc_buf[i] = crc(buf[i] << 8, POLY_GEN);
        }

        memcpy(buf_to_send, buf, size_msg);
        memcpy(buf_to_send + size_msg, crc_buf, size_msg);

        send(dest_socket, buf_to_send, 2 * size_msg, 0);
        printf("\n");

        if (strncmp((char *)buf, "/quit", 5) == 0) {
            printf("Disconnected\n");
            break;
        }

        recv_result = recv(dest_socket, &ack, 1, 0);
        if (recv_result < 1) {
            printf("Connection interrupted\n");
            break;
        }

        // Envoie le message jusqu'à recevoir un ACK.
        while (!ack) {
            SERVER_DISPLAY(NACK_MSG);
            send(dest_socket, buf_to_send, 2 * size_msg, 0);
            CLIENT_DISPLAY(username, RESEND_MSG)
            recv_result = recv(dest_socket, &ack, 1, 0);
            if (recv_result < 1) {
                printf("Connection interrupted\n");
                break;
            }
        }

        SERVER_DISPLAY(ACK_MSG);
    }
    close(dest_socket);

    return 0;
}