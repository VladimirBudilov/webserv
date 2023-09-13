#include "../../includes/Server.hpp"

void Server::start() {
    std::cout << "Server started" << std::endl;
    _socket = new Socket(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY);
    _socket->bindSocket();
    _socket->listenSocket();
    _eventManager = new EventManager();
    _eventManager->registerSignal(_socket->getSocket());
    _eventManager->loop(_socket->getSocket());
}
