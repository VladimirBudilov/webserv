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

bool ClientSocket::isValidRequest() {
    if(Request.RequestData[0] == 'G')
        return true;
    return false;
}

void ClientSocket::generateCGIResponse() {
    const char *pythonScriptPath = "/Users/vbudilov/Desktop/WebServ/webserv/www/bin-cgi/data.py";
    const char *pythonInterpreter = "/usr/bin/python2.7"; // Путь к интерпретатору Python

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
    std::string path = "/Users/vbudilov/Desktop/WebServ/webserv/www/staticHTML/index.html";
    std::ifstream file(path.c_str());
    std::string str;
    std::string response;
    if (file.is_open()) {
        while (std::getline(file, str)) {
            response += str + "\n";
        }
        file.close();
    } else {
        Response.Status = "HTTP/1.1 404 Not Found\r\n";
        Response.Body = "<html><body><h1>404 Not Found</h1></body></html>";
        Response.ResponseData = Response.Status + Response.Body;
        Response.sentLength = 0;
        return;
    }
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
    Response.sentLength = 0;
}

