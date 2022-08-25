#include "upload.hpp"

upload::upload() {}
upload::~upload() {}

upload::upload(server &serv, std::pair<std::string, std::string> page, std::string buf, unsigned int content_length)
	: serverInfo(serv.serveInfo), rqst(page), buffer(buf), filename("")
{
	content_length = 100000000;
	if (content_length > serverInfo.client_max_body_size)
	{
		serv.resp.set_status_code(413);
		// resp = response(serveInfo.locations[page.second],this->serveInfo.error_pages, data);
		return ;
	}
	get_filename();
}

void upload::get_filename()
{
	struct stat s;

	size_t pos = buffer.find("filename=\"");
	if (pos != std::string::npos)
	{
		pos += 10;
		size_t len = buffer.find('"', pos);
		filename = buffer.substr(pos, len - pos);

		std::map<std::string, location_info>::iterator it = serverInfo.locations.begin();
		for (; it != serverInfo.locations.end(); it++) {
			if (it->second.upload_dir != "") {
				path.assign(it->second.upload_dir);
				break ;
			}
		}

		std::cout << "PATH = " << path << "\n";
		if (!(stat(path.c_str(), &s) == 0 && s.st_mode & S_IFDIR))
			mkdir(path.c_str(), S_IRWXG | S_IRWXO | S_IRWXU);

		path.append(filename);
		std::cout << "PATH = " << path << "\n";
	}
}
