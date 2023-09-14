#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "WebServHeaders.hpp"

class EventManager;
class Socket;

class Server {
private:
    std::list<Socket> _listeningSockets;
    std::list<Socket> _clientSockets;
    EventManager *_eventManager = new EventManager();

public:
    Server()= default;
    ~Server()= default;

    void start();
    void stop();
    void restart();
    void reload();
    void status();
    void configtest();
    void help();
    void version();
};


#endif //WEBSERV_SERVER_HPP
