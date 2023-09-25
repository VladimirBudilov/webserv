#include "Server.hpp"

Server::Server() {
    _eventManager = new EventManager();
}

void Server::start() {
    std::cout << "Server started" << std::endl;
    generateServerSockets();
    addServerSocketsToEventManager();
    _eventManager->loop(_serverSockets, _clientSockets);
}

void Server::addServerSocketsToEventManager() {
    std::vector<ServerSocket>::iterator it = _serverSockets.begin();
    for (; it != _serverSockets.end(); ++it) {
        _eventManager->registerListeningEvent(it->getSocket());
    }
}

void Server::generateServerSockets() {
    std::vector<ServerConfig>::iterator it = _serverConfigs.begin();
    std::vector<ClientSocket::t_socketData > uniqueSockets;

    for (; it != _serverConfigs.end(); ++it) {
        ClientSocket::t_socketData data;
        data.port = it->getPort();
        data.ip = it->getHost();
        data.config = *it;
        uniqueSockets.insert(data);
    }

}

void Server::parseConfigFile(const std::string &configFile) {
    std::ifstream infile(configFile.c_str());
    if (!infile.is_open())
        configError();
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(infile, line)) {
        lines.push_back(line);
    }
    infile.close();
    removeComments(lines);
    for (int i = 0; i < static_cast<int>(lines.size()); ++i) {
        if (lines[i] == "server {")
            parseServer(lines, _serverConfigs, i);
        else
            configError();
    }
}

const std::vector<ServerConfig> &Server::getServerConfigs() const {
    return _serverConfigs;
}
