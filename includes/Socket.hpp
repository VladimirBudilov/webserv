#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP

#include "WebServHeaders.hpp"

class Socket {
protected:
    struct sockaddr_in _addr;
    int _socket;
    Socket(){};
public:
    Socket(std::string const &IP, int port);
    Socket(std::string const &IP, int service, int protocol, int port, u_long interface);
    virtual ~Socket(){};

    void bindSocket();
    void listenSocket();
    void unlinkSocket();

    void checkSocket(int connection);

    int getSocket() const { return _socket; };
    struct sockaddr_in getAddr() { return _addr; };
    void setSocket(int socket) { _socket = socket; };
    void setAddr(struct sockaddr_in addr) { _addr = addr; };
    bool operator==(const int &socket) const { return _socket == socket; };
};

#endif //WEBSERV_SOCKET_HPP
