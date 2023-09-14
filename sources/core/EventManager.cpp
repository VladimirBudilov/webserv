#include "../../includes/EventManager.hpp"

void EventManager::registerSignal(int socket) {

    EV_SET(&_ev, socket , EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
    kevent(_kq, &_ev, 1, NULL, 0, NULL);
}

EventManager::EventManager() {
    _kq = kqueue();
if (_kq == -1) {
        perror("Ошибка при создании kqueue");
        exit(1);
    }
}

void EventManager::loop(int serverSocket) {
    kEvent events[_maxEvents];
    static int clientSocket;
    static struct kevent clientInterest;

    while (true) {
        int numEvents = kevent(_kq, NULL, 0, events, _maxEvents, NULL);
        if (numEvents == -1) {
            perror("Ошибка при ожидании событий в kqueue");
            exit(1);
        }

        assert(numEvents == 1);

        for (int i = 0; i < numEvents; ++i) {
            int socket = events[i].ident;

            if (socket == serverSocket) {
                sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);

                if (clientSocket == -1) {
                    perror("Ошибка при принятии подключения");
                }

                std::cout << "Принято новое подключение\n";

                assert(fcntl(clientSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC) != -1);

                EV_SET(&clientInterest, clientSocket , EVFILT_READ, EV_ADD | EV_ONESHOT, 0, 0, NULL);
                kevent(_kq, &clientInterest, 1, NULL, 0, NULL);
/*
                else {
                    // Обработка нового подключения
                    std::cout << "Принято новое подключение\n";

                    char buffer[1024];
                    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                    if (bytesRead > 0) {
                        std::string request(buffer, bytesRead);
                        if (request.find("GET") != std::string::npos) {
                            std::string response = generateResponse();
                            send(clientSocket, response.c_str(), response.size(), 0);
                        }
                    }
                    // close(clientSocket);
                }
*/
            }
            else if (socket == clientSocket) {
                std::cout << "Event on client socket" << std::endl;

                struct kevent event = events[i];

                char buf[event.data + 1];

                recv(clientSocket, buf, event.data, 0);

                buf[event.data] = 0;

                printf("read: %s\n", buf);
                if (event.fflags & EV_EOF) {
                    std::cout << "eof" << std::endl;
                }
            }
        }
    }
}

int EventManager::getMaxEvents() const {
    return _maxEvents;
}
