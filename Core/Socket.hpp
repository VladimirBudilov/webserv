#ifndef WEBSERV_SOCKET_HPP
#define WEBSERV_SOCKET_HPP


#include <string>
#include <netinet/in.h>

class Socket {
private:
    struct sockaddr_in _addr;
    int _socket;
    int _connection;
public:
    Socket(int domain, int service, int protocol, int port, u_long interface);
     virtual int connectSocket(int socket, const struct sockaddr *addr) = 0;
    virtual ~Socket(){};
};


#endif //WEBSERV_SOCKET_HPP
