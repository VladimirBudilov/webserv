#include "DataStorage.hpp"

std::map<int, std::string> DataStorage::defaultErrorPages;
std::map<std::string, std::string> DataStorage::MimeTypes = initMimeTypes();

std::string DataStorage::root = DataStorage::getCurrentWorkingDirectory();

std::string DataStorage::getCurrentWorkingDirectory() {
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string current_working_dir(buff);
    initDefaultErrorPages();
    return current_working_dir;
}

std::map<std::string, std::string> DataStorage::initMimeTypes()
{
	std::map<std::string, std::string> mimeTypes;
	mimeTypes["html"] = "text/html";
	mimeTypes["htm"] = "text/html";
	mimeTypes["shtml"] = "text/html";
	mimeTypes["css"] = "text/css";
	mimeTypes["xml"] = "text/xml";
	mimeTypes["rss"] = "text/xml";
	mimeTypes["gif"] = "image/gif";
	mimeTypes["jpeg"] = "image/jpeg";
	mimeTypes["jpg"] = "image/jpeg";
	mimeTypes["js"] = "application/x-javascript";
	mimeTypes["txt"] = "text/plain";
	mimeTypes["htc"] = "text/x-component";
	mimeTypes["mml"] = "text/mathml";
	mimeTypes["png"] = "image/png";
	mimeTypes["ico"] = "image/x-icon";
	mimeTypes["jng"] = "image/x-jng";
	mimeTypes["wbmp"] = "image/vnd.wap.wbmp";
	mimeTypes["jar"] = "application/java-archive";
	mimeTypes["war"] = "application/java-archive";
	mimeTypes["ear"] = "application/java-archive";
	mimeTypes["hqx"] = "application/mac-binhex40";
	mimeTypes["pdf"] = "application/pdf";
	mimeTypes["cco"] = "application/x-cocoa";
	mimeTypes["jardiff"] = "application/x-java-archive-diff";
	mimeTypes["jnlp"] = "application/x-java-jnlp-file";
	mimeTypes["run"] = "application/x-makeself";
	mimeTypes["pl"] = "application/x-perl";
	mimeTypes["pm"] = "application/x-perl";
	mimeTypes["prc"] = "application/x-pilot";
	mimeTypes["pdb"] = "application/x-pilot";
	mimeTypes["rar"] = "application/x-rar-compressed";
	mimeTypes["rpm"] = "application/x-redhat-package-manager";
	mimeTypes["sea"] = "application/x-sea";
	mimeTypes["swf"] = "application/x-shockwave-flash";
	mimeTypes["sit"] = "application/x-stuffit";
	mimeTypes["tcl"] = "application/x-tcl";
	mimeTypes["tk"] = "application/x-tcl";
	mimeTypes["der"] = "application/x-x509-ca-cert";
	mimeTypes["pem"] = "application/x-x509-ca-cert";
	mimeTypes["crt"] = "application/x-x509-ca-cert";
	mimeTypes["xpi"] = "application/x-xpinstall";
	mimeTypes["zip"] = "application/zip";
	mimeTypes["deb"] = "application/octet-stream";
	mimeTypes["bin"] = "application/octet-stream";
	mimeTypes["exe"] = "application/octet-stream";
	mimeTypes["dll"] = "application/octet-stream";
	mimeTypes["dmg"] = "application/octet-stream";
	mimeTypes["eot"] = "application/octet-stream";
	mimeTypes["iso"] = "application/octet-stream";
	mimeTypes["img"] = "application/octet-stream";
	mimeTypes["msi"] = "application/octet-stream";
	mimeTypes["msp"] = "application/octet-stream";
	mimeTypes["msm"] = "application/octet-stream";
	mimeTypes["mp3"] = "audio/mpeg";
	mimeTypes["ra"] = "audio/x-realaudio";
	mimeTypes["mpeg"] = "video/mpeg";
	mimeTypes["mpg"] = "video/mpeg";
	mimeTypes["mov"] = "video/quicktime";
	mimeTypes["flv"] = "video/x-flv";
	mimeTypes["avi"] = "video/x-msvideo";
	mimeTypes["wmv"] = "video/x-ms-wmv";
	mimeTypes["asx"] = "video/x-ms-asf";
	mimeTypes["asf"] = "video/x-ms-asf";
	mimeTypes["mng"] = "video/x-mng";
	mimeTypes["svg"] = "image/svg+xml";
	mimeTypes["svgz"] = "image/svg+xml";
	mimeTypes["tif"] = "image/tiff";
	mimeTypes["tiff"] = "image/tiff";
	mimeTypes["au"] = "audio/basic";
	mimeTypes["snd"] = "audio/basic";
	mimeTypes["wav"] = "audio/x-wav";
	mimeTypes["mov"] = "video/quicktime";
	mimeTypes["qt"] = "video/quicktime";
	return mimeTypes;
}

void DataStorage::initDefaultErrorPages() {
    defaultErrorPages[400] = root + "/www/errors/standardErrors/HTTP400.html";
    defaultErrorPages[401] = root + "/www/errors/standardErrors/HTTP401.html";
    defaultErrorPages[403] = root + "/www/errors/standardErrors/HTTP403.html";
    defaultErrorPages[404] = root + "/www/errors/standardErrors/HTTP404.html";
    defaultErrorPages[405] = root + "/www/errors/standardErrors/HTTP405.html";
    defaultErrorPages[413] = root + "/www/errors/standardErrors/HTTP413.html";
    defaultErrorPages[500] = root + "/www/errors/standardErrors/HTTP500.html";
    defaultErrorPages[501] = root + "/www/errors/standardErrors/HTTP501.html";
    defaultErrorPages[505] = root + "/www/errors/standardErrors/HTTP505.html";
}



