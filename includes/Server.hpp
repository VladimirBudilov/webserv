#ifndef WEBSERV_SERVER_HPP
#define WEBSERV_SERVER_HPP

#include "WebServHeaders.hpp"

class EventManager;
class Socket;

class Server {
private:
    Socket *_socket;
    std::list <int> _serverSockets;
    EventManager *_eventManager;

public:
    Server(){};
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
