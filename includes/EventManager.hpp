#ifndef WEBSERV_EVENTMANAGER_HPP
#define WEBSERV_EVENTMANAGER_HPP

#include "WebServHeaders.hpp"
#include "ServerSocket.hpp"
#include "ClientSocket.hpp"

class ServerSocket;
class ClientSocket;

class EventManager {
public:
    EventManager();
    ~EventManager(){};
    void registerListeningEvent(int socket);
    int getKq() const;
    void loop(std::list<ServerSocket> &serverSockets, std::list<ClientSocket> &clientSockets);
    int getMaxEvents() const;
private:
    static const int maxEvents = 63000;
    int _kq;
    typedef struct kevent kEvent;
    std::list<kEvent> _eventsList;
    kEvent _eventsArr[maxEvents];

    int getEventNumbers();

    int getServerSocketFd(std::list<ServerSocket> &Sockets, int currentEventSocket) const;

    int getClientSocketFd(std::list<ClientSocket> &Sockets, int currentEventSocket) const;

    void validareEOF(const ClientSocket &clientSocket, const kEvent &event) const;
};


#endif //WEBSERV_EVENTMANAGER_HPP
