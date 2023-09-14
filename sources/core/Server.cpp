#include "../../includes/Server.hpp"

Server::Server() {
    _eventManager = new EventManager();
}


void Server::start(int port) {
    std::cout << "Server started" << std::endl;
    Socket socket(AF_INET, SOCK_STREAM, 0 , port, INADDR_ANY);
    _listeningSockets.push_back(socket);
    for(auto &socket : _listeningSockets) {
        socket.bindSocket();
        socket.listenSocket();
    }
    for(auto &socket : _listeningSockets) {
        _eventManager->registerEvent(socket.getSocket());
    }
    _eventManager->loop(_listeningSockets);
}
