#include "upload.hpp"

upload::upload() {}
upload::~upload() {}

upload::upload(server_info infos, std::pair<std::string, std::string> page) : serveInfos(infos), rqst(page) {}

void upload::get_filename()
{

}
