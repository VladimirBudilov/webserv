#include <sys/socket.h>
#include "Socket.hpp"

Socket::Socket(int domain, int service, int protocol, int port, u_long interface)
{
    _addr.sin_family = domain;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = htonl(interface);

    _socket = socket(domain, service, protocol);
    if (_socket < 0)
    {
        perror("socket failed to connect");
        exit(1);
    }
    _connection = connectSocket(_socket, (struct sockaddr *)&_addr);
}


