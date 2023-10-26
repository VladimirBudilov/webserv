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
    void loop(std::vector<ServerSocket> &serverSockets, std::list<ClientSocket> &clientSockets);
    int getMaxEvents() const;
private:
    static const int maxEvents = 63000;
    int _kq;
    typedef struct kevent kEvent;
    std::list<kEvent> _eventsList;
    kEvent _eventsArr[maxEvents];

    int getEventsNumber();
    int getServerSocketFd(std::vector<ServerSocket> &Sockets, int currentEventSocket) const;
    int getClientSocketFd(std::list<ClientSocket> &Sockets, int currentEventSocket) const;
    void validateEOF(ClientSocket &clientSocket, const EventManager::kEvent &event) const;
    void addClientSocketEvent(const ClientSocket &clientSocket) const;
    void RemoveClientSocketEvent(const ClientSocket &clientSocket) const;
    void readRequest(ClientSocket &clientSocket, const kEvent &event) const;
    void writeResponse(ClientSocket &clientSocket, std::list<ClientSocket> &clientSockets) const;

    ClientSocket &getClientSocketBySocketFd(std::list<ClientSocket> &clientSockets, int fd);

    ServerSocket & getServerSocketBySocketFd(std::vector<ServerSocket> &serverSockets, int fd);

    void createResponse(ClientSocket &clientSocket) const;
};


#endif //WEBSERV_EVENTMANAGER_HPP
