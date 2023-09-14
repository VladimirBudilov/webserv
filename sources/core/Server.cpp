#include "../../includes/Server.hpp"

void Server::start() {
    std::cout << "Server started" << std::endl;
    _listeningSockets.emplace_back(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY);
    for(auto &socket : _listeningSockets) {
        socket.bindSocket();
        socket.listenSocket();
    }
    for(auto &socket : _listeningSockets) {
        _eventManager->registerSignal(socket.getSocket());
    }
    for(auto &socket : _clientSockets) {
        _eventManager->registerSignal(socket.getSocket());
    }

}
