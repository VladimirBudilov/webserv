#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include "WebServHeaders.hpp"

class Socket {
protected:
    struct sockaddr_in _addr;
    int _socket;
public:
    Socket(int domain, int service, int protocol, int port, u_long interface);
    void bindSocket();
    void listenSocket();
    void unlinkSocket();
    void checkSocket(int connection);
    virtual ~Socket(){};
    int getSocket() const { return _socket; };
};

#endif //WEBSERV_SOCKET_HPP
