#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <iostream>
#include <string>
#include <cstring>
#include "config_structs.hpp"
#if OS == LINUX
#include "wait.h"
#endif


class response {
	public:
		response();
		response(std::string response_string);
		response(location_info serv, std::map<int,std::string> error_page,std::string &path);
		response (std::string &path,std::string &type);
		std::string build_response(void);
	void	set_response(int value); 

	private: 
		location_info  local_info;
		std::map<int,std::string> error_page;
		std::string path;
		std::string type;
		std::string content;
		std::string status;
		int status_code;
};

#endif
