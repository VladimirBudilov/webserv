#ifndef WEBSERV_EVENTMANAGER_HPP
#define WEBSERV_EVENTMANAGER_HPP

#include "WebServHeaders.hpp"

class Socket;

class EventManager {
public:
    EventManager();
    ~EventManager(){};

    void registerEvent(int socket);
    void loop(std::list<Socket> &serverSockets);
    int getMaxEvents() const;
private:
    static const int maxEvents = 63000;
    int _kq = kqueue();
    typedef struct kevent kEvent;
    std::list<kEvent> _eventsList;
    kEvent _eventsArr[maxEvents];

};


#endif //WEBSERV_EVENTMANAGER_HPP
