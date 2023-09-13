#ifndef WEBSERV_EVENTMANAGER_HPP
#define WEBSERV_EVENTMANAGER_HPP

#include "WebServHeaders.hpp"

class EventManager {
public:
    EventManager();
    ~EventManager(){};

    void registerSignal(int socket);
    void loop(int serverSocket);

private:
    int _kq;
    struct kevent _ev;
};


#endif //WEBSERV_EVENTMANAGER_HPP
