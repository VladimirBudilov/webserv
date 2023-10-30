#include "EventManager.hpp"

void EventManager::loop(std::vector<ServerSocket> &serverSockets, std::list<ClientSocket> &clientSockets) {
    int serverSocketFd;
    int clientSocketFd;
    int currentEventSocketFd;
    int eventsNumber;
    while (true) {
        eventsNumber = getEventsNumber();
        for (int i = 0; i < eventsNumber; ++i) {
            /// get current event socket fd
            currentEventSocketFd = _eventsArr[i].ident;
            serverSocketFd = getServerSocketFd(serverSockets, currentEventSocketFd);
            clientSocketFd = getClientSocketFd(clientSockets, currentEventSocketFd);
            if (serverSocketFd != -1) {
                /// new connection with server
                ClientSocket clientSocket(currentEventSocketFd, _kq,
                                          getServerSocketBySocketFd(serverSockets, currentEventSocketFd).getConfig());
                clientSockets.push_back(clientSocket);
                /// client-server communication
            } else if (clientSocketFd != -1) {
                ClientSocket &clientSocket = getClientSocketBySocketFd(clientSockets, currentEventSocketFd);
                kEvent event = _eventsArr[i];
                switch (event.filter) {
                    ///if client socket event on reading
                    case EVFILT_READ: {
                        readRequest(clientSocket, event);
                        createResponse(clientSocket);
                        break;
                    }
                        ///if client socket event on writing
                    case EVFILT_WRITE: {
                        writeResponse(clientSocket, clientSockets);
                        break;
                    }
                }
            }
        }
    }
}

void EventManager::writeResponse(ClientSocket &clientSocket, std::list<ClientSocket> &clientSockets) const {
    int bufToWrite = 1024;
    std::string response = clientSocket.Response.ResponseData;
    int &sentLength = clientSocket.Response.sentLength;
    int length = clientSocket.Response.ResponseData.size();
    int writingRemainder = length - clientSocket.Response.sentLength;
    if (bufToWrite > writingRemainder)
        bufToWrite = writingRemainder;
    int wasSent = send(
            clientSocket.getSocket(),
            (response.substr(sentLength).c_str()),
            bufToWrite,
            0
    );
    if(wasSent == -1)
    {
        close(clientSocket.getSocket());
        RemoveClientSocketEvent(clientSocket);
        clientSockets.remove(clientSocket);
        return;
    }
    sentLength += wasSent;
    if (sentLength >= length) {
        RemoveClientSocketEvent(clientSocket);
        close(clientSocket.getSocket());
        clientSockets.remove(clientSocket);
    }
}

void EventManager::readRequest(ClientSocket &clientSocket, const EventManager::kEvent &event) const {
    char buf[1024];
    int received = recv(clientSocket.getSocket(), buf, sizeof(buf), 0);
    if (received == -1) {
        close(clientSocket.getSocket());
        RemoveClientSocketEvent(clientSocket);
        return;
    }
    if (received > 0)
        buf[received] = '\0';
    clientSocket.Request.RequestData.append(buf, received);
    validateEOF(clientSocket, event);
}

void EventManager::createResponse(ClientSocket &clientSocket) const {
    if (!clientSocket.CanMakeResponse())
        return;
    if (!clientSocket.isValidRequest()) {
        RemoveClientSocketEvent(clientSocket);
        close(clientSocket.getSocket());
        return;
    }
    clientSocket.generateResponse();
    if (!clientSocket.Response.ResponseData.empty())
        addClientSocketEvent(clientSocket);
    RemoveClientSocketEvent(clientSocket);
}

void EventManager::RemoveClientSocketEvent(const ClientSocket &clientSocket) const {
    struct kevent currentReadingEvent;
    EV_SET(&currentReadingEvent, clientSocket.getSocket(), EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(_kq, &currentReadingEvent, 1, NULL, 0, NULL);
}

void EventManager::addClientSocketEvent(const ClientSocket &clientSocket) const {
    struct kevent clientWrite;
    EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
    kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
}

void EventManager::registerListeningEvent(int socket) {
    struct kevent event;
    EV_SET(&event, socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(_kq, &event, 1, NULL, 0, NULL);
    _eventsList.push_back(event);
}

void EventManager::validateEOF(ClientSocket &clientSocket, const EventManager::kEvent &event) const {
    ///check that request is empty and EOF
    if (event.flags & EV_EOF && clientSocket.Request.RequestData.size() == 0) {
        close(clientSocket.getSocket());
    }
}

int EventManager::getEventsNumber() {
    int numEvents = kevent(_kq, NULL, 0, _eventsArr, maxEvents, NULL);
    if (numEvents == -1) {
        perror("Ошибка при ожидании событий в kqueue");
        exit(1);
    }
    return numEvents;
}

int EventManager::getMaxEvents() const {
    return maxEvents;
}

EventManager::EventManager() {
    _kq = kqueue();
    if (_kq == -1) {
        perror("Ошибка при создании kqueue");
        exit(1);
    }
}

int EventManager::getServerSocketFd(std::vector<ServerSocket> &Sockets, int currentEventSocket) const {
    for (int i = 0; i < (int) Sockets.size(); ++i) {
        if (Sockets[i].getSocket() == currentEventSocket) {
            return currentEventSocket;
        }
    }
    return -1;
}

ClientSocket &EventManager::getClientSocketBySocketFd(std::list<ClientSocket> &clientSockets, int fd) {
    std::list<ClientSocket>::iterator it;
    for (it = clientSockets.begin(); it != clientSockets.end(); ++it) {
        if (it->getSocket() == fd) {
            return *it;
        }
    }
    throw std::runtime_error("Client socket not found");
}

ServerSocket &EventManager::getServerSocketBySocketFd(std::vector<ServerSocket> &serverSockets, int fd) {
    for (int i = 0; i < (int) serverSockets.size(); ++i) {
        if (serverSockets[i].getSocket() == fd) {
            return serverSockets[i];
        }
    }
    throw std::runtime_error("Server socket not found");
}

int EventManager::getClientSocketFd(std::list<ClientSocket> &Sockets, int currentEventSocket) const {
    std::list<ClientSocket>::iterator it;
    for (it = Sockets.begin(); it != Sockets.end(); ++it) {
        if (it->getSocket() == currentEventSocket) {
            return currentEventSocket;
        }
    }
    return -1;
}


