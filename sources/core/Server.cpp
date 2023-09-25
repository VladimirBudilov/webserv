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

    std::set<std::pair<std::string, int> > uniquePairs;
    for (; it != _serverConfigs.end(); ++it)
        uniquePairs.insert(std::make_pair(it->getHost(), it->getPort()));
    std::set<std::pair<std::string, int> >::iterator it2 = uniquePairs.begin();
    for (; it2 != uniquePairs.end(); ++it2) {
        std::vector<ServerConfig> serverConfigs = getServerConfigsByHostAndPort(it2->first, it2->second);
        ServerSocket serverSocket(it2->first, it2->second, serverConfigs);
        _serverSockets.push_back(serverSocket);
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

std::vector<ServerConfig> Server::getServerConfigsByHostAndPort(std::string host, int port) {
    std::vector<ServerConfig> serverConfigs;
    std::vector<ServerConfig>::iterator it = _serverConfigs.begin();
    for (; it != _serverConfigs.end(); ++it) {
        if (it->getHost() == host && it->getPort() == port)
            serverConfigs.push_back(*it);
    }
    return serverConfigs;
}

ServerSocket &Server::getServerSocketBySocketFd(int socket) {
    std::vector<ServerSocket>::iterator it = _serverSockets.begin();
    for (; it != _serverSockets.end(); ++it) {
        if (it->getSocket() == socket)
            return *it;
    }
    throw std::exception();
}
