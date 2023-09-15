#include "Server.hpp"

Server::Server() {
    _eventManager = new EventManager();
}

void Server::start() {
    generateServerSockets();
    addServerSocketsToEventManager();
    _eventManager->loop(_serverSockets, _clientSockets);
}

void Server::addServerSocketsToEventManager() {
    std::list<ServerSocket>::iterator it = _serverSockets.begin();
    for (; it != _serverSockets.end(); ++it) {
        _eventManager->registerListeningEvent(it->getSocket());
    }
}

void Server::generateServerSockets() {
    std::cout << "Server started" << std::endl;
    std::vector<ServerConfig>::iterator it = _serverConfigs.begin();
    std::set<std::pair<std::string, int> > uniquePairs;
    for (; it != _serverConfigs.end(); ++it)
        uniquePairs.insert(std::make_pair(it->getHost(), it->getPort()));
    std::set<std::pair<std::string, int> >::iterator it2 = uniquePairs.begin();
    for (; it2 != uniquePairs.end(); ++it2) {
        _serverSockets.push_back(ServerSocket(it2->first, it2->second));
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
