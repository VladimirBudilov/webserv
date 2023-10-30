#include "ClientSocket.hpp"
#include "DataStorage.hpp"
#include <algorithm>

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
    _isReadyToMakeResponse = false;
    ResponseSize = 0;
}

bool ClientSocket::isValidRequest() const {
    if (Request.RequestData.empty()) {
        std::cout << "empty request" << std::endl;
        return false;
    }
    return true;
}

void ClientSocket::generateCGIResponse(const std::string &path, const Location &location, const std::string &
pathToUpload) {
    const char *pythonScriptPath = path.c_str();
    const char *pythonInterpreter = location.getCgiPass().c_str();
    std::string pathInfo;
    std::string pathTranslated;
    std::string tmpBodyFile;
    int hasBody = Request.getMethod() == "POST" ? 1 : 0;
    char **pythonEnv = new char *[3 + Request.getArgs().size() + hasBody];
    std::map<std::string, std::string> env = Request.getArgs();
    if (!Request.getBody().empty()) {
        tmpBodyFile = "BODY_" + std::to_string(_socket);
        std::ofstream file(DataStorage::root + "/www/" + tmpBodyFile.c_str());
        file << Request.getBody();
        file.close();
    }
    pathInfo = path.substr(path.find(".py") + 3, path.size() - 1);
    pathInfo = "PATH_INFO=" + DataStorage::root + "/www" + pathToUpload;
    pathTranslated = "PATH_TRANSLATED=" + DataStorage::root + "/www/" + pathInfo;
    pythonEnv[0] = strdup(pathInfo.c_str());
    pythonEnv[1] = strdup(pathTranslated.c_str());
    ///put all args in env
    int i = 2;
    for (std::map<std::string, std::string>::iterator it = env.begin(); it != env.end(); it++) {
        std::string tmp = it->first + "=" + it->second;
        pythonEnv[i] = strdup(tmp.c_str());
        i++;
    }
    if (hasBody) {
        pythonEnv[i++] = strdup(("BODY_FILE=" + DataStorage::root + "/www/" + tmpBodyFile).c_str());
    }
    pythonEnv[i] = NULL;
    ///generate args for execve
    char **pythonArgs = new char *[3];
    pythonArgs[0] = strdup(pythonInterpreter);
    pythonArgs[1] = strdup(pythonScriptPath);
    pythonArgs[2] = NULL;
    std::string tmpCGIFile = "CGI" + std::to_string(_socket);
    int fdCGIFile = open((DataStorage::root + "/www/" + tmpCGIFile).c_str(), O_RDWR | O_CREAT, 0666);
    if (fdCGIFile == -1) {
        perror("Ошибка при открытии файла");
        exit(1);
    }
    int pid = fork();
    if (!pid) {
        dup2(fdCGIFile, 1);
        if (execve(pythonInterpreter, pythonArgs, pythonEnv) == -1) {
            perror("Ошибка при выполнении execve");
            exit(1);
        }
        close(fdCGIFile);
    }
    waitpid(pid, NULL, 0);
    std::ifstream file((DataStorage::root + "/www/" + tmpCGIFile).c_str(), std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);
        Response.ResponseData.resize(length);
        file.read(&Response.ResponseData[0], length);
        file.close();
    }
    remove((DataStorage::root + "/www/" + tmpCGIFile).c_str());
    remove((DataStorage::root + "/www/" + tmpBodyFile).c_str());
    delete[] pythonArgs;
    delete[] pythonEnv;
}

void ClientSocket::generateResponse() {
    ServerConfig currentConfig;
    Location currentLocation;
    std::string method = Request.getMethod();
    std::string location = Request.getPath();
    std::string fileToOpen;
    std::string pathAfterCGIScript;
    std::string root;

    /// split request path to file and place to path after cgi
    parseRequestPath(fileToOpen, pathAfterCGIScript, location);
    std::string host = Request.getHeaders().find("Host")->second;
    bool isAutoindex = Request.getArgs().find("autoindex") != Request.getArgs().end();
    host = host.substr(0, host.find(':'));
    currentConfig = _config[0];
    ///get config by host another will be default
    chooseConfig(host, currentConfig);
    ///get location by request path
    std::vector<Location> locations = currentConfig.getLocations();
    chooseLocation(host, currentConfig, locations);
    ///go through config and find location
    root = rootParsing(location, locations, currentLocation);
    if (currentLocation.isRedirect()) {
        ///generate redirect response with 301 code and Location header where will be currentLocation.getRedirect()
        generateRedirectResponse(currentLocation.getRedirectPath());
        return;
    }
    ///Validate request
    if (!isValidRequest(currentConfig, currentLocation, method, root, isAutoindex))
        return;
    ///create response for isAutoindex
    if (currentLocation.isAutoindex() || isAutoindex) {
        generateAutoindexResponse();
        return;
    }
    ///create response for DELETE request
    if (Request.getMethod() == "DELETE") {
        deleteFile(fileToOpen, root);
        return;
    }
    ///adjust path to file for location by default
    if (root[root.size() - 1] == '/' && fileToOpen.empty())
        root += currentLocation.getIndex();
    ///adjust path to file for location with file
    if (root[root.size() - 1] == '/' && !fileToOpen.empty())
        root += fileToOpen;
    getFoolPath(root);
    getDataByFullPath(root, currentConfig, currentLocation, pathAfterCGIScript);
}

void ClientSocket::deleteFile(const std::string &fileToOpen, std::string &root) {
    getFoolPath(root);
    if (remove((root + fileToOpen).c_str()) == -1) {
        Response.ResponseData = "HTTP/1.1 404 Not Found\r\n"
                                "Content-Type: application/json\r\n"
                                "\r\n"
                                "{\n"
                                "    \"status\": \"error\",\n"
                                "    \"message\": \"File not found.\"\n"
                                "}";
        return;
    }
    Response.ResponseData = "HTTP/1.1 200 OK\r\n"
                            "Content-Type: text/plain\r\n"
                            "\r\n"
                            "File successfully deleted.";
}

void ClientSocket::generateAutoindexResponse() {
    std::string autoindexLocation = Request.getArgs().find("path")->second + Request.getPath();
    std::string html = generateAutoindexPage(DataStorage::root + "/www", autoindexLocation);
    Response.Body = html;
    if (autoindexLocation.find('.') == std::string::npos)
        Response.GenerateContentType("костыль.html"); ///TODO: do not fix it)))
    else
        Response.GenerateContentType(autoindexLocation);
    Response.ResponseData = Response.Status + Response.Body;
}

bool ClientSocket::isValidRequest(const ServerConfig &currentConfig, const Location &currentLocation,
                                  const std::string &method,
                                  const std::string &root, bool isAutoindex) {
    bool isValidRequest = true;
    if (Request.hasError) {
        generateErrorPage(currentConfig, Request.Error);
        isValidRequest = false;
    } else if (root.empty() && !isAutoindex) {
        generateErrorPage(currentConfig, 404);
        isValidRequest = false;
    }
        ///check method
    else if (!isValidMethod(method, currentLocation) && !isAutoindex) {
        generateErrorPage(currentConfig, 405);
        isValidRequest = false;
    }
        ///check body size
    else if (Request.getHeaders().find("Content-Length") != Request.getHeaders().end()) {
        if (currentLocation.getMaxBodySize() != -1 && currentLocation.getMaxBodySize() != -1) {
            if (Request.getBody().size() < (size_t) currentLocation.getMaxBodySize()
                || Request.getBody().size() < currentConfig.getMaxBodySize()) {
                generateErrorPage(currentConfig, 413);
                isValidRequest = false;
            }
        }
    }
        /// check HTTP version
    else if (!Request.isVersion() && !isAutoindex) {
        generateErrorPage(currentConfig, 505);
        isValidRequest = false;
    }
    return isValidRequest;
}

void
ClientSocket::chooseLocation(const std::string &host, ServerConfig &currentConfig, std::vector<Location> &locations) {
    for (size_t i = 0; i < _config.size(); i++) {
        if (_config[i].getHost() == host) {
            currentConfig = _config[i];
            locations = _config[i].getLocations();
            break;
        }
    }
}

std::string ClientSocket::rootParsing(const std::string &location, const std::vector<Location> &locations,
                                      Location &currentLocation) const {
    std::string root;
    for (size_t j = 0; j < locations.size(); j++) {
        if (locations[j].getPath() == location) {
            root = locations[j].getRoot();
            currentLocation = locations[j];
            break;
        }
    }
    return root;
}

void ClientSocket::parseRequestPath(std::string &fileToOpen, std::string &placeToUpload,
                                    std::string &location) {
    ///if location has file name put it in fileToOpen and delete it from location
    if (location.find('.') != std::string::npos) {
        ///if location last element / remove it
        if (!Request.hasCGI()) {
            if (location[location.size() - 1] == '/' && location.size() > 1)
                location = location.substr(0, location.size() - 1);
            fileToOpen = location.substr(location.find_last_of('/') + 1, location.size() - 1);
            location = location.substr(0, location.find_last_of('/'));
            if (location.empty())
                location = "/";
        } else {
            placeToUpload = location.substr(location.find(".py") + 3, location.size() - 1);
            location = location.substr(0, location.find(".py") + 3);
            fileToOpen = location.substr(location.find_last_of('/') + 1, location.size() - 1);
            location = location.substr(0, location.find_last_of('/'));
        }
    }
}

std::string
ClientSocket::generateAutoindexPage(const std::string &rootPath, const std::string &location) {
    DIR *dir;
    struct dirent *ent;
    struct stat filestat;
    std::stringstream html;
    std::string path = rootPath + location;

    if ((dir = opendir(path.c_str())) != NULL) {
        html << "<html><body><ul>";
        while ((ent = readdir(dir)) != NULL) {
            std::string filepath = path + "/" + ent->d_name;
            stat(filepath.c_str(), &filestat);

            std::string mod_time = ctime(&filestat.st_mtime);
            mod_time = mod_time.substr(0, mod_time.size() - 1);  // remove trailing newlinelocation + "/"
            if (location[location.size() - 1] == '/') {
                html << "<li><a href=\"" << location + ent->d_name << "?autoindex=1\">" << ent->d_name << "</a> "
                     << " (size: " << filestat.st_size << ", "
                     << "modified: " << mod_time << ")</li>";
            } else {
                html << "<li><a href=\"" << location + "/" + ent->d_name << "?autoindex=1\">" << ent->d_name << "</a> "
                     << " (size: " << filestat.st_size << ", "
                     << "modified: " << mod_time << ")</li>";
            }
        }
        html << "</ul></body></html>";
        closedir(dir);
    } else {
        std::fstream file(path.c_str());
        if (!file.is_open())
            html << "Error: cannot open file" << std::endl;
        else {
            file >> html.rdbuf();
            file.close();
        }
    }
    return html.str();
}

void ClientSocket::getDataByFullPath(const std::string &path, const ServerConfig &config, const Location &location,
                                     const std::string
                                     &pathAfterCGIScript) {
    std::string response;
    ///handle cgi(.py scripts)
    if (isCGI(path)) {
        generateCGIResponse(path, location, pathAfterCGIScript);
        if (Response.ResponseData.empty())
            generateErrorPage(config, 500);
        return;
    }
    ///handle static files
    std::ifstream file(path.c_str(), std::ios::binary);
    if (file) {
        file.seekg(0, std::ios::end);
        std::streampos length = file.tellg();
        file.seekg(0, std::ios::beg);
        response.resize(length);
        file.read(&response[0], length);
        file.close();
        Response.Body = response;
        Response.GenerateContentType(path);
        Response.ResponseData = Response.Status + Response.Body;
    }
        ///handle error page
    else {
        generateErrorPage(config, 404);
        return;
    }
}

void ClientSocket::generateErrorPage(const ServerConfig &config, int errorNumber) {
    std::map<short, std::string> errors = config.getErrorPages();
    std::string errorRoot = errors[(short) errorNumber];
    Response.generateDefaultErrorPage(errorNumber);
    if (!errorRoot.empty()) {
        getFoolPath(errorRoot);
        getErrorPageData(errorRoot);
    }
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
        std::getline(file, response, '\0');
        file.close();
    } else {
        Response.ResponseData = Response.Status + Response.Body;
        return;
    }
    Response.Body = response;
    Response.ResponseData = Response.Status + Response.Body;
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

bool ClientSocket::CanMakeResponse() {
    ///check that request method is get and request is complete
    if (Request.getMethod() != "GET" && Request.getMethod() != "POST" &&
        Request.RequestData.find("\r\n\r\n") != std::string::npos) {
        Request.parse_request(Request.RequestData);
        if (Request.getMethod() == "GET") {
            _isReadyToMakeResponse = true;
        }
    }
    ///update data of body size in request
    if (Request.getMethod() == "POST")
        Request.parse_request(Request.RequestData);
    ///check that request method is post and request is complete
    if (Request.getMethod() == "POST" &&
        std::atoi((Request.getHeaders().find("Content-Length")->second).c_str()) == (int) Request.getBody().size()) {
        _isReadyToMakeResponse = true;
    }
    ///check that request method is delete and request is complete
    if (Request.getMethod() == "DELETE" && Request.RequestData.find("\r\n\r\n") != std::string::npos) {
        Request.parse_request(Request.RequestData);
        _isReadyToMakeResponse = true;
    }
    return _isReadyToMakeResponse;
}

void ClientSocket::generateRedirectResponse(const std::string &locationToRedir) {
    Response.Status = "HTTP/1.1 301 Moved Permanently\r\n";
    Response.Status += "Location: " + locationToRedir + "\r\n";
    Response.ResponseData = Response.Status + Response.Body;

}

void ClientSocket::chooseConfig(const std::string &host, ServerConfig &config) {
    for (size_t i = 0; i < _config.size(); i++) {
        if (_config[i].getServerName() == host)
            config = _config[i];
    }
}
