#ifndef WEBSERV_EVENTMANAGER_HPP
#define WEBSERV_EVENTMANAGER_HPP

#include "WebServHeaders.hpp"
#include "Socket.hpp"
#include "ClientSocket.hpp"

class Socket;
class ClientSocket;

class EventManager {
public:
    EventManager();
    ~EventManager(){};

    void registerListeningEvent(int socket);

    int getKq() const;

    void loop(std::list<Socket> &serverSockets, std::list<ClientSocket> &clientSockets);
    int getMaxEvents() const;
private:
    static const int maxEvents = 63000;
    int _kq;
    typedef struct kevent kEvent;
    std::list<kEvent> _eventsList;
    kEvent _eventsArr[maxEvents];

    int getEventNumbers();
};


#endif //WEBSERV_EVENTMANAGER_HPP
