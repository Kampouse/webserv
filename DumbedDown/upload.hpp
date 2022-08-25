#pragma once

#include <iostream>
#include <sys/stat.h>

#include "config_structs.hpp"
#include "Server.hpp"

class upload {
	private:
		server_info serverInfo;
		std::pair<std::string, std::string> rqst;
		std::string buffer;
		std::string filename;
		std::string path;

	public:
		upload();
		upload(server_info, std::pair<std::string, std::string>, std::string);
		~upload();

		void get_filename();
};