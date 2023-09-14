#include "../../includes/EventManager.hpp"

EventManager::EventManager() {
    _kq = kqueue();
    std::cout << "kq = " << _kq << std::endl;
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

void EventManager::registerEvent(int socket) {

    struct kevent event;
    EV_SET(&event, socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(_kq, &event, 1, NULL, 0, NULL);
    _eventsList.push_back(event);
}

void EventManager::loop(std::list<Socket> &serverSockets) {

    int clientSocket;
    struct kevent clientInterest;
    struct clientState {
        std::string read;
        size_t much_written;
    } clientState;

    std::string response = "Hello, world\n";

    while (true) {
        int numEvents = kevent(_kq, NULL, 0, _eventsArr, maxEvents, NULL);
        if (numEvents == -1) {
            perror("Ошибка при ожидании событий в kqueue");
            exit(1);
        }

        assert(numEvents == 1);

        for (int i = 0; i < numEvents; ++i) {
            int socket = _eventsArr[i].ident;

            if (serverSockets.end() != std::find(serverSockets.begin(), serverSockets.end(), socket))
            {
                std::cout << "event on server socket\n" << std::endl;
                sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                clientSocket = accept(socket, (struct sockaddr *) &clientAddr, &clientAddrLen);

                if (clientSocket == -1) {
                    perror("Ошибка при принятии подключения");
                }

                std::cout << "Принято новое подключение\n";

                // assert(fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) != -1);

                EV_SET(&clientInterest, clientSocket , EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(_kq, &clientInterest, 1, NULL, 0, NULL);
            }
            else if (socket == clientSocket) {
                std::cout << "Event on client socket" << std::endl;

                struct kevent event = _eventsArr[i];

                switch (event.filter) {
                case EVFILT_READ: {
                    char *buf = (char *) malloc(event.data + 1);
                    assert(buf);

                    recv(clientSocket, buf, event.data, 0);

                    buf[event.data] = 0;

                    clientState.read += buf;

                    printf("read: %s\n", buf);
                    if (event.flags & EV_EOF) {
                        std::cout << "eof" << std::endl;
                        //clientState = {};
                        close(clientSocket);
                    }

                    free(buf);

                    if (clientState.read == "get\n") {
                        // int n = send(clientSocket, "ok\n", 3, 0);
                        std::cout << "need to respond" << std::endl;

                        clientState.much_written = 0;
                        struct kevent clientWrite;
                        EV_SET(&clientWrite, clientSocket, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
                        kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                    }
                    break;
                }
                        case EVFILT_WRITE: {

                            assert(response.size() != 0);

                            int can_write = event.data;
                            int length = response.size();

                            int much_left_to_write = length - clientState.much_written;

                            if (can_write > much_left_to_write)
                                can_write = much_left_to_write;

                            int much_written = send(
                                    clientSocket,
                                    (response.substr(clientState.much_written)).c_str(),
                                    can_write,
                                    0
                            );

                            assert(much_written == can_write);

                            clientState.much_written += can_write;

                            if (clientState.much_written == response.size()) {
                                struct kevent clientWrite;
                                EV_SET(&clientWrite, clientSocket, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
                                kevent(_kq, &clientWrite, 1, NULL, 0, NULL);
                            }
                            break;
                        }
                }


            }
        }
    }
}

int EventManager::getMaxEvents() const {
    return maxEvents;
}
