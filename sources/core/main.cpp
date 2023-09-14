#include "../../includes/WebServHeaders.hpp"

int main (int argc, char **argv)
{
    Server server;
    int port = 8080;

    if (argc == 2)
        port = atoi(argv[1]);
    server.start(port);
    return 0;
}