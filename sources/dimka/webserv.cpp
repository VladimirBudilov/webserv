#include "webserv.hpp"


void configError() {
    std::cout << "Error, bad config file." << std::endl;
    exit(1);
}

std::string generate_autoindex(const std::string& path) {
	DIR* dir;
	struct dirent* ent;
	struct stat filestat;
	std::stringstream html;

	html << "<html><body><ul>";

	if ((dir = opendir(path.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			std::string filepath = path + "/" + ent->d_name;
			stat(filepath.c_str(), &filestat);

			std::string mod_time = ctime(&filestat.st_mtime);
			mod_time = mod_time.substr(0, mod_time.size()-1);  // remove trailing newline

			html << "<li><a href=\"" << ent->d_name << "\">" << ent->d_name << "</a> "
				 << " (size: " << filestat.st_size << ", "
				 << "modified: " << mod_time << ")</li>";
		}
		closedir(dir);
	} else {
		html << "<li>Error opening directory</li>";
	}

	html << "</ul></body></html>";

	return html.str();
}

void parseServer(std::vector<std::string> &str, std::vector<ServerConfig>& servers, int& i) {
    i++;
    ServerConfig res;
    while (str[i] != "}" && i < static_cast<int>(str.size())) {
        std::stringstream ss(str[i]);
        std::string word;
        ss >> word;
        if (word == "listen")
            res.listen(ss);
        else if (word == "port")
            res.port(ss);
        else if (word == "server_name")
            res.server_name(ss);
        else if (word == "error_page")
            res.error_page(ss);
        else if (word == "location")
            res.parseLocation(str, i);
        else if (word == "client_max_body_size")
            res.max_body_size(ss);
        else
            configError();
        i++;
    }
    if (str[i] != "}")
        configError();
    servers.push_back(res);
}

void removeComments(std::vector<std::string> &str) {
    for (size_t i = 0; i < str.size(); i++) {
        size_t pos = str[i].find('#');
        if (pos != std::string::npos)
            str[i].erase(pos);
    }

    for (int i = 0; i < static_cast<int>(str.size()); ++i) {
        while(isspace(str[i][0]))
            str[i].erase(0, 1);
    }

    for (int i = 0; i < static_cast<int>(str.size()); ++i) {
        while(str[i].size() != 0  && isspace( str[i][str[i].size() - 1]))
        {
            str[i].erase(str[i].size() - 1, 1);
        }
    }

    for (int i = 0; i < static_cast<int>(str.size()); i++) {
        if (str[i].empty()) {
            str.erase(str.begin() + i);
            i--;
        }
    }
}

bool isValidIP(const std::string& ip) {
    std::stringstream ss(ip);
    std::string segment;
    int i = 0, num;
    while (std::getline(ss, segment, '.')) {
        if (segment.empty() || segment.size() > 3 || (segment.size() > 1 && segment[0] == '0'))
            return false;
        for (int j = 0; j < static_cast<int>(segment.size()); ++j)
        {
            char c = segment[j];
            if (!isdigit(c))
                return false;
        }
        std::istringstream(segment) >> num;
        if (num < 0 || num > 255) return false;
        ++i;
    }
    return i == 4 && ss.eof();
}