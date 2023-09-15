#include "../../includes/Server.hpp"

Server::Server() {
    _eventManager = new EventManager();
}

void Server::start() {
    generateServerSockets();
    addServerSocketsToEventManager();
    _eventManager->loop(_serverSockets, _clientSockets);
}

void Server::addServerSocketsToEventManager() {

    for(int i = 0; i < _serverSockets.size(); ++i) {
        _eventManager->registerListeningEvent(_serverSockets[i]->getSocket());
    }
}

void Server::generateServerSockets() {
    std::cout << "Server started" << std::endl;
    Socket socket1("0.0.0.0", 8080);
    Socket socket2("0.0.0.0", 8080 + 1);
    Socket socket3("0.0.0.0", 8080 + 2);
    _serverSockets.push_back(socket1);
    _serverSockets.push_back(socket2);
    _serverSockets.push_back(socket3);

}
