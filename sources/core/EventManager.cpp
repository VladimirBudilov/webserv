#include "EventManager.hpp"


void EventManager::registerListeningEvent(int socket) {

    struct kevent event;
    EV_SET(&event, socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(_kq, &event, 1, NULL, 0, NULL);
    _eventsList.push_back(event);
}

void EventManager::loop(std::list<ServerSocket> &serverSockets, std::list<ClientSocket> &clientSockets) {

    while (true) {
        int numEvents = getEventNumbers();

        for (int i = 0; i < numEvents; ++i) {
            int currentEventSocket = _eventsArr[i].ident;

            if (serverSockets.end() != std::find(serverSockets.begin(), serverSockets.end(), currentEventSocket)) {
                std::cout << "event on server currentEventSocket\n" << std::endl;
                ClientSocket clientSocket(currentEventSocket, _kq);
                clientSockets.push_back(clientSocket);
            } else if (clientSockets.end() !=
                       std::find(clientSockets.begin(), clientSockets.end(), currentEventSocket)) {
                std::cout << "Event on client currentEventSocket" << std::endl;
                ClientSocket &clientSocket = *(std::find(clientSockets.begin(), clientSockets.end(),
                                                        currentEventSocket));
                std::cout << "socket " << clientSocket.getSocket() << std::endl;
                kEvent event = _eventsArr[i];
                switch (event.filter) {
                    case EVFILT_READ: {
                        char buf[1024];
                        int recived = recv(clientSocket.getSocket(), buf, sizeof(buf), 0);
                        buf [recived] = '\0';
                        clientSocket.Request.RequestData += buf;
                        if (event.flags & EV_EOF) {
                            std::cout << "eof" << std::endl;
                            close(clientSocket.getSocket());
                        }
                        std::cout << clientSocket.Request.RequestData << std::endl;
                        if (clientSocket.isRequestReady()) {
                            std::cout << "need to respond" << std::endl;


                            clientSocket.Response.ResponseData = "HTTP/1.1 200 OK\n"
                                                                  "Server: webserv\n"
                                                                  "Content-Type: text/html\n"
                                                                  "Content-Length: 12\n"
                                                                  "\n"
                                                                  "Hello world!";
                            clientSocket.Response.sentLength = 0;
                            struct kevent clientWrite;
                            EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                            kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                        }
                        break;
                    }
                    case EVFILT_WRITE: {
                        std::cout << "response data" << clientSocket.Response.ResponseData << std::endl;
                        assert(clientSocket.Response.ResponseData.size() != 0);
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
                        assert(wasSent == bufToWrite);
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


