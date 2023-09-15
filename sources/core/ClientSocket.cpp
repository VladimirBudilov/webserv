#include "ClientSocket.hpp"

ClientSocket::kEvent &ClientSocket::getClientInterest() {
    return _clientInterest;
}

void ClientSocket::Read(const std::string &read) {
    ClientSocket::_read = read;
}

void ClientSocket::MuchWritten(size_t muchWritten) {
    _much_written = muchWritten;
}

ClientSocket::ClientSocket(int socket, int kq) {

    struct sockaddr_in clientAddr;

    _socket = socket;
    _much_written = 0;
    socklen_t clientAddrLen = sizeof(clientAddr);
    setSocket(accept(socket, (struct sockaddr *) &clientAddr, &clientAddrLen));
    checkSocket(_socket);

    fcntl(_socket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    EV_SET(&_clientInterest, _socket , EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &_clientInterest, 1, NULL, 0, NULL);

}

void ClientSocket::setClientInterest(const ClientSocket::kEvent &clientInterest) {
    _clientInterest = clientInterest;
}

