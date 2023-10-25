
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
    ClientSocket(){}; // private default constructor
    bool _isReadyToMakeResponse;
public:
    int ResponseSize;
    Request Request;
    Response Response;
    ClientSocket(int socket, int kq, const std::vector<ServerConfig> &configs);

	std::string generate_autoindex(const std::string &rootPath, const std::string &location);

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

    void generateCGIResponse(const std::string &path, const Location &location);

    void generateResponse();

    ClientSocket(const ClientSocket &socket);
    ClientSocket &operator=(const ClientSocket &socket);
    ~ClientSocket(){};
    ///generate == operator
    bool operator==(const ClientSocket &socket) const;

    void getFoolPath(std::string &pathToUpdate) const;

    void getDataByFullPath(const std::string &path, const ServerConfig &config, const Location &location);

    void getErrorPageData(const std::string &errorRoot);

    void generateErrorPage(const ServerConfig &config, int errorNumber);

    bool isValidMethod(const std::string &method, const Location &location);

    bool isCGI(std::string path);

    bool CanMakeResponse();

    void setIsReadyToMakeResponse(bool isReadyToMakeResponse);
};


#endif //WEBSERV_CLIENTSOCKET_HPP
