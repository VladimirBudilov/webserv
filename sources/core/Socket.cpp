#include "../../includes/Socket.hpp"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface) {
    _addr.sin_family = domain;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = htonl(interface);
    _socket = socket(service, protocol, 0);
    checkSocket(_socket);
}

void Socket::checkSocket(int connection) {
    if (connection < 0) {
        perror("socket failed to connect");
        exit(1);
    }
}

void Socket::bindSocket() {
    checkSocket(bind(_socket, (struct sockaddr *) &_addr, sizeof(_addr)));
}

void Socket::listenSocket() {
    checkSocket(listen(_socket, SOMAXCONN));
}
