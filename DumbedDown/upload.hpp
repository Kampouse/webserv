#pragma once

#include <iostream>

#include "config_structs.hpp"

class upload {
	private:
		server_info serveInfos;
		std::pair<std::string, std::string> rqst;

	public:
		upload();
		upload(server_info, std::pair<std::string, std::string>);
		~upload();

		void get_filename();
};