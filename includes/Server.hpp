#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "WebServHeaders.hpp"
#include "Socket.hpp"
#include "ClientSocket.hpp"
#include "EventManager.hpp"


class Socket;
class EventManager;
class ClientSocket;

class Server {
private:
    EventManager *_eventManager;
    std::list<Socket> _serverSockets;
    std::list<ClientSocket> _clientSockets;

public:

    Server();
    ~Server(){};

    void start();
    void stop();
    void restart();
    void reload();
    void status();
    void configtest();
    void help();
    void version();

    void addServerSocketsToEventManager();
    void generateServerSockets();
};


#endif //WEBSERV_SERVER_HPP
