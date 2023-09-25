#ifndef WEBSERV_SERVERSOCKET_HPP
#define WEBSERV_SERVERSOCKET_HPP

#include "WebServHeaders.hpp"
#include "ServerConfig.hpp"

class ServerConfig;

class ServerSocket {
protected:
    struct sockaddr_in _addr;
    int _socket;
    ServerConfig _config;
    ServerSocket(){};
public:
    ServerSocket(std::string const &IP, int port, ServerConfig config);
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

    typedef struct socketData
    {
        std::string ip;
        int port;
        ServerConfig config;
    } t_socketData;
};

#endif //WEBSERV_SERVERSOCKET_HPP
