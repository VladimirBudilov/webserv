#include "../../includes/Socket.hpp"

Socket::Socket(std::string const &IP, int port) {
    in_addr_t addr = inet_addr(IP.c_str());
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = addr;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    checkSocket(_socket);

    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
        perror("setsockopt");
        exit(1);
    }
    fcntl(_socket, F_SETFL, O_NONBLOCK);
    bindSocket();
    listenSocket();
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

Socket::Socket(const std::string &IP, int service, int protocol, int port, u_long interface) {
    //in_addr_t addr = inet_addr(IP.c_str());
    (void) IP;
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = htonl(interface);
    _socket = socket(AF_INET, service, protocol);
    checkSocket(_socket);
}
