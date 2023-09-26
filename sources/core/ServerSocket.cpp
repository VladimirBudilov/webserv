#include "ServerSocket.hpp"

ServerSocket::ServerSocket(std::string const &IP, int port, const std::vector<ServerConfig>& configs) {
    _config = configs;
    in_addr_t addr = inet_addr(IP.c_str());
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = addr;
    _socket = socket(AF_INET, SOCK_STREAM, 0);
    checkSocket(_socket);
    int opt = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int)) < 0) {
        perror("setsockopt failed to set SO_REUSEADDR flag for socket in ServerSocket constructor (ServerSocket.cpp)");
        exit(1);
    }
    fcntl(_socket, F_SETFL, O_NONBLOCK);
    bindSocket();
    listenSocket();
}

void ServerSocket::checkSocket(int connection) {
    if (connection < 0) {
        perror("socket failed to connect. fix config file (ServerSocket.cpp)");
        exit(1);
    }
}

void ServerSocket::bindSocket() {
    checkSocket(bind(_socket, (struct sockaddr *) &_addr, sizeof(_addr)));
}

void ServerSocket::listenSocket() {
    checkSocket(listen(_socket, SOMAXCONN));
}

const std::vector<ServerConfig> &ServerSocket::getConfig() const {
    return _config;
}

ServerSocket::ServerSocket(const ServerSocket &socket) {
    *this = socket;

}

ServerSocket &ServerSocket::operator=(const ServerSocket &socket) {
    if (this != &socket) {
        _addr = socket._addr;
        _socket = socket._socket;
        _config = socket._config;
    }
    return *this;
}

bool ServerSocket::operator==(const ServerSocket &socket) const {
    return _socket == socket._socket;
}