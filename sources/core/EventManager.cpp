#include "EventManager.hpp"

void EventManager::loop(std::list<ServerSocket> &serverSockets, std::list<ClientSocket> &clientSockets) {

    int serverSocketFd;
    int clientSocketFd;
    int currentEventSocketFd;
    while (true) {
        int numEvents = getEventNumbers();

        for (int i = 0; i < numEvents; ++i) {
            currentEventSocketFd = _eventsArr[i].ident;
            serverSocketFd = getServerSocketFd(serverSockets, currentEventSocketFd);
            clientSocketFd = getClientSocketFd(clientSockets, currentEventSocketFd);
            if (serverSocketFd != -1) {
                /// new connection
                ClientSocket clientSocket(currentEventSocketFd, _kq);
                clientSockets.push_back(clientSocket);
            } else if (clientSocketFd != -1) {
                ClientSocket &clientSocket = *(std::find(clientSockets.begin(), clientSockets.end(),
                                                         currentEventSocketFd));
                kEvent event = _eventsArr[i];
                switch (event.filter) {
                    case EVFILT_READ: {
                        char buf[1024];
                        int recived = recv(clientSocket.getSocket(), buf, sizeof(buf), 0);
                        buf[recived] = '\0';
                        clientSocket.Request.RequestData += buf;
                        validareEOF(clientSocket, event);

                        if (clientSocket.isValidRequest()) {
                            std::cout << "need to respond" << std::endl;
                            if(clientSocket.Request.hasCGI())
                                clientSocket.generateCGIResponse();
                            else
                            {
                                clientSocket.generateStaticResponse();
                            }
                            struct kevent clientWrite;
                            EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                            kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                        }
                        break;
                    }
                    case EVFILT_WRITE: {
                        if(clientSocket.Response.ResponseData.size() == 0)
                        {
                            struct kevent clientWrite;
                            EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                            continue;
                        }
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
                        if(wasSent == bufToWrite)
                        {
                            struct kevent clientWrite;
                            EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                        }
                        sentLength += wasSent;
                        if (sentLength == length) {
                            struct kevent clientWrite;
                            EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                            kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                        }
                        break;
                    }
                }
            }
        }
    }
}


void EventManager::registerListeningEvent(int socket) {

    struct kevent event;
    EV_SET(&event, socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(_kq, &event, 1, NULL, 0, NULL);
    _eventsList.push_back(event);
}

void EventManager::validareEOF(const ClientSocket &clientSocket, const EventManager::kEvent &event) const {
    if (event.flags & EV_EOF) {
        std::cout << "eof" << std::endl;
        close(clientSocket.getSocket());
    }
}

int EventManager::getClientSocketFd(std::list<ClientSocket> &Sockets, int currentEventSocket) const {
    std::list<ClientSocket>::iterator serverSocket = std::find(Sockets.begin(), Sockets.end(), currentEventSocket);
    return serverSocket != Sockets.end() ? (serverSocket->getSocket()) : -1;
}

int EventManager::getServerSocketFd(std::list<ServerSocket> &Sockets, int currentEventSocket) const {
    std::list<ServerSocket>::iterator serverSocket = std::find(Sockets.begin(), Sockets.end(), currentEventSocket);
    return serverSocket != Sockets.end() ? (serverSocket->getSocket()) : -1;
}

int EventManager::getEventNumbers() {
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

int EventManager::getKq() const {
    return _kq;
}

EventManager::EventManager() {
    _kq = kqueue();
    if (_kq == -1) {
        perror("Ошибка при создании kqueue");
        exit(1);
    }
}


