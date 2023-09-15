
#ifndef WEBSERV_CLIENTSOCKET_HPP
#define WEBSERV_CLIENTSOCKET_HPP

#include "WebServHeaders.hpp"
#include "Socket.hpp"


class ClientSocket : public Socket {
private:
    typedef struct kevent kEvent;
    kEvent _clientInterest;
    std::string _read;
    size_t _much_written;
    ClientSocket(){}; // private default constructor
public:
    ClientSocket(int socket, int kq);

    void setClientInterest(const kEvent &clientInterest) {
        ClientSocket::_clientInterest = clientInterest;
    }
    kEvent &getClientInterest();

    void Read(const std::string &read);

    void MuchWritten(size_t muchWritten);

    const std::string &getRead() const {
        return _read;
    }
    size_t getMuchWritten() const {
        return _much_written;
    }
};


#endif //WEBSERV_CLIENTSOCKET_HPP
