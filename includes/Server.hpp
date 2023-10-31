#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "WebServHeaders.hpp"
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"
#include "EventManager.hpp"
#include "ServerConfig.hpp"
#include "webserv.hpp"

class ServerSocket;
class EventManager;
class ClientSocket;
class ServerConfig;

class Server {
private:
    EventManager *_eventManager;
    std::vector<ServerSocket> _serverSockets;
    std::list<ClientSocket> _clientSockets;
    std::vector<ServerConfig> _serverConfigs;
public:
    Server();
    ~Server(){};
    void parseConfigFile(const std::string &configFile);
    void start();
    const std::vector<ServerConfig> &getServerConfigs() const;
    void addServerSocketsToEventManager();
    void generateServerSockets();
    ServerSocket &getServerSocketBySocketFd(int socket);
    std::vector<ServerConfig> getServerConfigsByHostAndPort(std::string host, int port);
};


#endif //WEBSERV_SERVER_HPP
