#include "Server.hpp"

int main() {
    Server server;
	//server.parseConfigFile("/Users/vbudilov/Desktop/WebServ/webserv/webserv.conf");
	server.parseConfigFile("/Users/anatasha/MyProjects/webserv/webserv.conf");
	server.start();
    return 0;
}
