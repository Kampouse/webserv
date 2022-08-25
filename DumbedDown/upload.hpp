#pragma once

#include <iostream>
#include <sys/stat.h>

#include "config_structs.hpp"
#include "Server.hpp"

class server;
class response;

class upload {
	private:
		server_info serverInfo;
		std::pair<std::string, std::string> rqst;
		std::string buffer;
		std::string filename;
		std::string path;

	public:
		upload();
		upload(server&, std::pair<std::string, std::string>, std::string, unsigned int);
		~upload();

		void get_filename();
		bool delete_file();
		void write_file();
};