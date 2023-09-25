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
    void stop();
    void restart();
    void reload();
    void status();
    void configtest();
    void help();
    void version();

    const std::vector<ServerConfig> &getServerConfigs() const;
    void addServerSocketsToEventManager();
    void generateServerSockets();
};


#endif //WEBSERV_SERVER_HPP
