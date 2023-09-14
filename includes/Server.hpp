#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "WebServHeaders.hpp"

class Socket;
class EventManager;

class Server {
private:
    std::list<Socket> _listeningSockets;
    std::list<Socket> _clientSockets;
    EventManager *_eventManager;

public:

    Server();
    ~Server(){};

    void start(int port);
    void stop();
    void restart();
    void reload();
    void status();
    void configtest();
    void help();
    void version();
};


#endif //WEBSERV_SERVER_HPP
