#ifndef WEBSERV_SERVERSOCKET_HPP
#define WEBSERV_SERVERSOCKET_HPP

#include "WebServHeaders.hpp"

class ServerSocket {
protected:
    struct sockaddr_in _addr;
    int _socket;
    ServerSocket(){};
public:
    ServerSocket(std::string const &IP, int port);
    virtual ~ServerSocket(){};

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

#endif //WEBSERV_SERVERSOCKET_HPP
