#ifndef WEBSERV_EVENTMANAGER_HPP
#define WEBSERV_EVENTMANAGER_HPP

#include "WebServHeaders.hpp"


class EventManager {
public:
    EventManager()=default;
    ~EventManager()=default;

    void registerEvent(int socket);
    void loop(const std::list<Socket> &serverSockets);
    int getMaxEvents() const;
private:
    int _kq = kqueue();
    typedef struct kevent kEvent;
    std::list<kEvent> _events;
    int _maxEvents = 1000;
};


#endif //WEBSERV_EVENTMANAGER_HPP
