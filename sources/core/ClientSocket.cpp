#include "ClientSocket.hpp"
#include "DataStorage.hpp"
#include <algorithm>

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
    EV_SET(&_clientInterest, _socket, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &_clientInterest, 1, NULL, 0, NULL);
}

void ClientSocket::setClientInterest(const ClientSocket::kEvent &clientInterest) {
    _clientInterest = clientInterest;
}

bool ClientSocket::isValidRequest() {
    if (Request.RequestData.empty())
        return false;
    Request.parse_request(Request.RequestData);
    if (Request.isVersion())
        return true;
    ///TODO add correct Request validation
    return false;
}

void ClientSocket::generateCGIResponse() {
    std::string method = Request.getMethod();
    std::string location = Request.getPath();
    std::string host = Request.getHeaders().find("Host")->second;
    bool autoindex = Request.getArgs().find("autoindex") != Request.getArgs().end();
    std::string path = Request.getArgs().find("path")->second;
    ServerConfig currentConfig;
    Location currentLocation;
    std::string root;
    host = host.substr(0, host.find(':'));
    currentConfig = _config[0];
    std::vector<Location> locations = _config[0].getLocations();
    ///get config by host another will be default
    std::string configHost;
    for (size_t i = 0; i < _config.size(); i++) {
        if (_config[i].getHost() == host) {
            currentConfig = _config[i];
            locations = _config[i].getLocations();
            break;
        }
    }
    ///go through first config and find location
    for (size_t i = 0; i < locations.size(); i++) {
        if (locations[i].getPath() == location) {
            root = locations[i].getRoot();
            currentLocation = locations[i];
            break;
        }
    }
    if (root.empty() && !autoindex) {
        std::cout << "empty root" << std::endl;
        generateErrorPage(currentConfig, 404);
        return;
    }


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
    std::string host = Request.getHeaders().find("Host")->second;
    bool autoindex = Request.getArgs().find("autoindex") != Request.getArgs().end();
    std::string path = Request.getArgs().find("path")->second;
    ServerConfig currentConfig;
    Location currentLocation;
    std::string root;
    host = host.substr(0, host.find(':'));
    currentConfig = _config[0];
    std::vector<Location> locations = _config[0].getLocations();
    ///get config by host another will be default
    std::string configHost;
    for (size_t i = 0; i < _config.size(); i++) {
        if (_config[i].getHost() == host) {
            currentConfig = _config[i];
            locations = _config[i].getLocations();
            break;
        }
    }
    ///go through first config and find location
    for (size_t j = 0; j < locations.size(); j++) {
        if (locations[j].getPath() == location) {
            root = locations[j].getRoot();
            currentLocation = locations[j];
            break;
        }
    }
    if (root.empty() && !autoindex) {
        std::cout << "empty root" << std::endl;
        generateErrorPage(currentConfig, 404);
        return;
    }
    ///check method
    if (!isValidMethod(method, currentLocation) && !autoindex) {
        std::cout << "invalid method" << std::endl;
        generateErrorPage(currentConfig, 405);
        return;
    }
    ///create response for autoindex
    if (currentLocation.isAutoindex() || autoindex) {
        std::cout << "autoindex" << std::endl;
        std::string html = generate_autoindex(DataStorage::root + "/www", path + Request.getPath());
        Response.Body = html;
        Response.ResponseData = Response.Status + Response.Body;
        return;
    }
    ///create response for index
    if (root[root.size() - 1] == '/')
        root += currentLocation.getIndex();
    ///check if path with CGI extension
    if (isCGI(root)) {
        std::cout << "CGI" << std::endl;
        generateCGIResponse(root);
        return;
    }
    getFoolPath(root);
    getDataByFullPath(root, currentConfig);
}

std::string ClientSocket::generate_autoindex(const std::string &rootPath, const std::string &location) {
	DIR* dir;
	struct dirent* ent;
	struct stat filestat;
	std::stringstream html;
	std::string path = rootPath + location;

	html << "<html><body><ul>";

	if ((dir = opendir(path.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string filepath = path + "/" + ent->d_name;
			stat(filepath.c_str(), &filestat);

			std::string mod_time = ctime(&filestat.st_mtime);
			mod_time = mod_time.substr(0, mod_time.size()-1);  // remove trailing newlinelocation + "/"
			if(location[location.size() - 1] == '/')
			{
				html << "<li><a href=\"" << location + ent->d_name << "?autoindex=1\">" << ent->d_name << "</a> "
					 << " (size: " << filestat.st_size << ", "
					 << "modified: " << mod_time << ")</li>";
			} else
			{
				html << "<li><a href=\"" << location + "/" + ent->d_name << "?autoindex=1\">" << ent->d_name << "</a> "
					 << " (size: " << filestat.st_size << ", "
					 << "modified: " << mod_time << ")</li>";
			}
		}
		closedir(dir);
	} else {
		std::fstream file(path.c_str());
		if(!file.is_open())
			html << "Error: cannot open file" << std::endl;
		else
		{
			file >> html.rdbuf();
			//std::cout << file << std::endl;
			file.close();
			Response.Status =  "HTTP/1.1 200 OK\n"
			"Content-Type: image/jpeg\n\n";
		}
	}

	html << "</ul></body></html>";

	return html.str();
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
        generateErrorPage(currentConfig, 0);
        return;
    }
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
    Response.sentLength = 0;
}

void ClientSocket::generateErrorPage(const ServerConfig &currentConfig, int errorNumber) {
    std::__1::map<short, std::string> errors = currentConfig.getErrorPages();
    std::__1::map<short, std::string>::iterator it = errors.find(errorNumber);
    Response.generateDefoultErrorPage(errorNumber);
    std::string errorRoot = it->second;
    getFoolPath(errorRoot);
    getErrorPageData(errorRoot);
}

void ClientSocket::getFoolPath(std::string &pathToUpdate) const {
    size_t found = pathToUpdate.find("/FULL_PATH_TO_FILE");
    pathToUpdate.replace(found, sizeof("/FULL_PATH_TO_FILE") - 1, DataStorage::root);
}

ClientSocket::ClientSocket(const ClientSocket &socket) : ServerSocket(socket) {
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
        Response.ResponseData = Response.Status + Response.Body;
        return;
    }
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
    Response.sentLength = 0;
}

bool ClientSocket::isValidMethod(const std::string &method, const Location &location) {
    if (method == "GET") {
        return location.getMethods()[0];
    }
    if (method == "POST") {
        return location.getMethods()[1];
    }
    if (method == "DELETE") {
        return location.getMethods()[2];
    }
    return false;
}

bool ClientSocket::isCGI(std::string path) {
    ///check that last 2 symbols is .py
    if (path.size() > 2 && path.substr(path.size() - 3, 3) == ".py")
        return true;
    return false;
}

