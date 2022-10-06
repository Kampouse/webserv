#pragma once

#include <iostream>
#include <cstring>
#include <sstream>
#include <dirent.h>
#include <time.h>

#include "config_structs.hpp"

#if OS_LINUX
#include "wait.h"
#endif

class response {
	public:
		response();
		~response();
		response(std::string response_string);
		response(location_info serv, std::map<int,std::string> error_page,std::string &path);
		response(std::string &path,std::string &type);
		response & operator=(const response & rhs);
		std::string build_response(std::map<std::string,location_info> &lst_info );
		void set_response(int value); 
		void set_status(std::string status) { this->status = status;}
		void set_status_code(int code);


	private: 
		location_info  local_info;
		std::map<int,std::string> error_page;
		std::string path;
		std::string type;
		std::string content;
		std::string status;
		int status_code;
};
