#include "ClientSocket.hpp"
#include "DataStorage.hpp"

ClientSocket::kEvent &ClientSocket::getClientInterest() {
    return _clientInterest;
}

void ClientSocket::Read(const std::string &read) {
    ClientSocket::_read = read;
}

void ClientSocket::MuchWritten(size_t muchWritten) {
    _much_written = muchWritten;
}

ClientSocket::ClientSocket(int socket, int kq, const std::vector<ServerConfig> &configs) {

    _config = configs;
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

bool ClientSocket::isValidRequest() {
    if(Request.RequestData.empty())
        return false;
    Request. parse_request(Request.RequestData);
    if(Request.isVersion())
        return true;
    ///TODO add correct Request validation
    return false;
}

void ClientSocket::generateCGIResponse() {



    const char *pythonScriptPath = "/Users/vbudilov/Desktop/WebServ/webserv/www/bin-cgi/data.py";
    const char *pythonInterpreter = "/usr/local/bin/python3"; // Путь к интерпретатору Python

    // Аргументы для Python скрипта
    char *const pythonArgs[] = {const_cast<char *>(pythonInterpreter),
                                const_cast<char *>(pythonScriptPath), nullptr};

    // Переменные окружения (в данном случае не указываем)
    char *const pythonEnv[] = {nullptr};
    int fd = open("/Users/vbudilov/Desktop/WebServ/webserv/www/bin-cgi/data.txt", O_RDWR | O_CREAT, 0666);
    int pid = fork();
    if (!pid) // Создаем дочерний процесс
    {
        // Запускаем скрипт с помощью execve
        dup2(fd, 1);
        if (execve(pythonInterpreter, pythonArgs, pythonEnv) == -1) {
            perror("Ошибка при выполнении execve");
            exit(1);
        }
        close(fd);
    }
    waitpid(pid, NULL, 0); // Ждем завершения дочернего процесса
    std::string response;
    std::ifstream file("/Users/vbudilov/Desktop/WebServ/webserv/www/bin-cgi/data.txt");
    std::string str;
    while (std::getline(file, str)) {
        response += str + "\n";
    }
    file.close();
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
    Response.sentLength = 0;
}

void ClientSocket::generateStaticResponse() {
    std::string method = Request.getMethod();
    std::string location = Request.getPath();
    ServerConfig currentConfig;
    Location currentLocation;

    std::string root;

    ///TODO find way to choose correct config?

    ///TODO check method

    ///go through first config and find location
    std::vector<Location> locations = _config[0].getLocations();
    currentConfig = _config[0];
    for (size_t j = 0; j < locations.size(); j++) {
        if (locations[j].getPath() == location) {
            root = locations[j].getRoot();
            currentLocation = locations[j];
            break;
        }
    }
    ///create response for autoindex
    if(currentLocation.isAutoindex())
    {
        std::cout << "autoindex" << std::endl;
        ///TODO add autoindex
        return;
    }
    if(root.empty())
    {
        ///TODO add error page
        std::cout << "error page" << std::endl;
        generateErrorPage(currentConfig);
        return;
    }
    ///create response for index
    if(root[root.size() - 1] == '/')
        root += currentLocation.getIndex();
    getFoolPath(root);
    getDataByFullPath(root, currentConfig);
}

void ClientSocket::getDataByFullPath(const std::string &path, const ServerConfig &currentConfig) {
    std::ifstream file(path.c_str());
    std::string str;
    std::string response;
    if (file.is_open()) {
        while (std::getline(file, str)) {
            response += str + "\n";
        }
        file.close();
    } else {
        generateErrorPage(currentConfig);
        return;
    }
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
    Response.sentLength = 0;
}

void ClientSocket::generateErrorPage(const ServerConfig &currentConfig) {
    std::__1::map<short, std::string> errors =  currentConfig.getErrorPages();
    std::__1::map<short, std::string>::iterator it = errors.find(404);
    Response.generateErrorStatus(404);
    std::string errorRoot = it->second;
    getFoolPath(errorRoot);
    getErrorPageData(errorRoot);
}

void ClientSocket::getFoolPath(std::string &pathToUpdate) const {
    size_t found = pathToUpdate.find("/FULL_PATH_TO_FILE");
    pathToUpdate.replace(found, sizeof("/FULL_PATH_TO_FILE") - 1, DataStorage::root);
}

ClientSocket::ClientSocket(const ClientSocket &socket)  : ServerSocket(socket) {
    _socket = socket._socket;
    _clientInterest = socket._clientInterest;
    _config = socket._config;
    _read = socket._read;
    _much_written = socket._much_written;
    Request = socket.Request;
    Response = socket.Response;
}

ClientSocket &ClientSocket::operator=(const ClientSocket &socket) {
    if (this == &socket)
        return *this;
    _socket = socket._socket;
    _clientInterest = socket._clientInterest;
    _config = socket._config;
    _read = socket._read;
    _much_written = socket._much_written;
    Request = socket.Request;
    Response = socket.Response;
    return *this;
}

bool ClientSocket::operator==(const ClientSocket &socket) const {
    return _socket == socket._socket;
}

void ClientSocket::getErrorPageData(const std::string &errorRoot) {
    std::ifstream file(errorRoot.c_str());
    std::string str;
    std::string response;
    if (file.is_open()) {
        while (std::getline(file, str)) {
            response += str + "\n";
        }
        file.close();
    } else {
        //generate error page
        std::cout << "error page" << std::endl;
        Response.Body = "Error page not found";
        Response.ResponseData = Response.Status + Response.Body;
        return;
    }
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
    Response.sentLength = 0;
}

