#include "../header/socket_utils.h"

int create_socket(int port)
{
    struct sockaddr_in addr;
    int server_socket;
    int bind_result;
    int listen_result;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(1);
    }

    // Pour reutiliser les ports directement.
    const int enable = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    bind_result = bind(server_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (bind_result == -1) {
        perror("bind");
        exit(1);
    }

    listen_result = listen(server_socket, 5);
    if (listen_result == -1) {
        perror("listen");
        exit(1);
    }

    return server_socket;
}