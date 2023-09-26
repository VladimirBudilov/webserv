#ifndef WEBSERV_SERVERSOCKET_HPP
#define WEBSERV_SERVERSOCKET_HPP

#include "WebServHeaders.hpp"
#include "ServerConfig.hpp"

class ServerConfig;

class ServerSocket {
protected:
    struct sockaddr_in _addr;
    int _socket;
    std::vector<ServerConfig> _config;
    ServerSocket(){};
public:
    ServerSocket(std::string const &IP, int port, const std::vector<ServerConfig>& configs);
    virtual ~ServerSocket(){};

    void bindSocket();
    void listenSocket();
    void unlinkSocket();

    void checkSocket(int connection);

    const std::vector<ServerConfig> &getConfig() const;

    int getSocket() const { return _socket; };
    struct sockaddr_in getAddr() { return _addr; };
    void setSocket(int socket) { _socket = socket; };
    void setAddr(struct sockaddr_in addr) { _addr = addr; };

    ServerSocket(const ServerSocket &socket);
    ServerSocket &operator=(const ServerSocket &socket);
    bool operator==(const ServerSocket &socket) const;
};

#endif //WEBSERV_SERVERSOCKET_HPP
