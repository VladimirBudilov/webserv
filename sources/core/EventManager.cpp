#include "../../includes/EventManager.hpp"

EventManager::EventManager() {
    _kq = kqueue();
    if(_kq == -1) {
        perror("Ошибка при создании kqueue");
        exit(1);
    }
}

std::string generateResponse() {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: text/plain\r\n";
    response << "Content-Length: 13\r\n";
    response << "\r\n";
    response << "11111111111\r\n";
    return response.str();
}

void EventManager::registerListeningEvent(int socket) {

    struct kevent event;
    EV_SET(&event, socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(_kq, &event, 1, NULL, 0, NULL);
    _eventsList.push_back(event);
}

void EventManager::loop(std::list<Socket> &serverSockets, std::list<ClientSocket> &clientSockets) {

    std::string response = "Hello, world\n";
    while (true) {
        int numEvents = getEventNumbers();

        for (int i = 0; i < numEvents; ++i) {
            int socket = _eventsArr[i].ident;

            if (serverSockets.end() != std::find(serverSockets.begin(), serverSockets.end(), socket))
            {
                std::cout << "event on server socket\n" << std::endl;
                ClientSocket clientSocket(socket, _kq);
                clientSockets.push_back(clientSocket);
            }
            else if (clientSockets.end() != std::find(clientSockets.begin(), clientSockets.end(), socket)) {
                std::cout << "Event on client socket" << std::endl;
                ClientSocket clientSocket = *(std::find(clientSockets.begin(), clientSockets.end(), socket));
                kEvent event = _eventsArr[i];

                switch (event.filter) {
                case EVFILT_READ: {
                    char *buf = (char *) malloc(event.data + 1);
                    assert(buf);
                    recv(clientSocket.getSocket(), buf, event.data, 0);
                    buf[event.data] = 0;
                    clientSocket.Read(buf);
                    printf("_read: %s\n", buf);
                    if (event.flags & EV_EOF) {
                        std::cout << "eof" << std::endl;
                        //clientState = {};
                        close(clientSocket.getSocket());
                    }

                    free(buf);

                    if (clientSocket.getRead() == "get\n") {
                        // int n = send(clientSocket, "ok\n", 3, 0);
                        std::cout << "need to respond" << std::endl;
                        clientSocket.MuchWritten(0);
                        struct kevent clientWrite;
                        EV_SET(&clientWrite, clientSocket.getSocket(), EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                        kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                    }
                    break;
                }
                        case EVFILT_WRITE: {

                            assert(response.size() != 0);
                            int can_write = event.data;
                            int length = response.size();
                            int much_left_to_write = length - clientSocket.getMuchWritten();
                            if (can_write > much_left_to_write)
                                can_write = much_left_to_write;
                            int much_written = send(
                                    clientSocket.getSocket(),
                                    (response.substr(clientSocket.getMuchWritten()).c_str()),
                                    can_write,
                                    0
                            );

                            assert(much_written == can_write);

                            clientSocket.MuchWritten(can_write);
                            if (clientSocket.getMuchWritten() == response.size()) {
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


