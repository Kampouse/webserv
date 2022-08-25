#include "upload.hpp"

upload::upload() {}
upload::~upload() {}

upload::upload(server &serv, std::pair<std::string, std::string> page, std::string buf, unsigned int content_length)
	: serverInfo(serv.serveInfo), rqst(page), buffer(buf), filename("")
{
	// content_length = 100000000;
	struct stat s;
	std::cout << buffer << "\nHELLO\n";
	if (content_length > serverInfo.client_max_body_size)
	{
		serv.resp.set_status_code(413);
		return ;
	}

	get_filename();

	if (stat(path.c_str(), &s) == 0)
	{
		if (!delete_file()){
			serv.resp.set_status_code(400);
			return ;
		}
	}

	write_file();
	serv.resp = response("/upload");
}

bool upload::delete_file()
{
	std::ifstream infile(path.c_str());
	if (infile.good()) {
		if (remove(path.c_str()) == -1)
			return false;
	}
	return true;
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

void upload::write_file()
{
	std::ofstream ofs(path.c_str(), std::ofstream::out | std::ofstream::binary | std::ofstream::app);
	
	size_t start = buffer.find("filename=\"");
	start = buffer.find("\r\n\r\n", start);
	start += 4;
	std::cout << "START = " << start << "\n";

	size_t pos = buffer.find("boundary") + 9;
	std::string boundary = buffer.substr(pos, buffer.find("\r\n\r\n", pos) - pos);

	std::cout << "BOUNDARY = " << boundary << "END OF BOUNDARY\n";

	size_t length = buffer.find(boundary, start);
	if (length != std::string::npos)
		length -= start;

	std::cout << "LENGTH = " << length << "\n";

	ofs.write(&buffer[start], length);
	ofs.close();
}
