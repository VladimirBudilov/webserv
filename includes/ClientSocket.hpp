
#ifndef WEBSERV_CLIENTSOCKET_HPP
#define WEBSERV_CLIENTSOCKET_HPP

#include "WebServHeaders.hpp"
#include "ServerSocket.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include "Server.hpp"
#include "ServerConfig.hpp"

class ServerConfig;

class ClientSocket : public ServerSocket {
private:
    typedef struct kevent kEvent;
    kEvent _clientInterest;
    std::string _read;
    size_t _much_written;
    ServerConfig *_serverConfig;
    ClientSocket(){}; // private default constructor

public:
    Request Request;
    Response Response;
    ClientSocket(int socket, int kq);

    void setClientInterest(const kEvent &clientInterest);
    kEvent &getClientInterest();

    void Read(const std::string &read);

    void MuchWritten(size_t muchWritten);

    const std::string &getRead() const {
        return _read;
    }
    size_t getMuchWritten() const {
        return _much_written;
    }

    bool isValidRequest();

    void generateCGIResponse();

    void generateStaticResponse();

};


#endif //WEBSERV_CLIENTSOCKET_HPP
