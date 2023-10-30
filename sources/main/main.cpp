#include "Server.hpp"

int main(int argc, char **argv) {
    Server server;
    if(argc > 2)
    {
        std::cout << "Too many arguments" << std::endl;
        return 0;
    }
    if(argc == 1)
    {
        server.parseConfigFile(DataStorage::root + "/webserv.conf");
    }
    else
    {
        server.parseConfigFile(DataStorage::root + "/" + argv[1]);
    }
	server.start();
    return 0;
}
