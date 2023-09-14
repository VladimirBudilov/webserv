#ifndef WEBSERV_EVENTMANAGER_HPP
#define WEBSERV_EVENTMANAGER_HPP

#include "WebServHeaders.hpp"


class EventManager {
public:
    EventManager()=default;
    ~EventManager()=default;

    void registerSignal(int socket);
    void loop(int serverSocket);
    int getMaxEvents() const;
private:
    int _kq;
    typedef struct kevent kEvent;
    std::list<kEvent> _events;
    int _maxEvents = 1000;
};


#endif //WEBSERV_EVENTMANAGER_HPP
