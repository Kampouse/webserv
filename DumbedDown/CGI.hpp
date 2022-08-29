#pragma once

#include <unistd.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <map>

#include "Server.hpp"

class server;

class CGI {
	private:
		char ** args;
		char ** envp;
		server_info serverInfo;
		std::string request;
		std::string path;
		std::string query;
		std::string scriptName;
		std::string data;
		std::string body;
		char buffer[100000];
		std::string getExecPath();

	public:
		CGI();
		CGI(server_info info, std::pair<std::string, std::string> page, std::string _data);
		~CGI();

		void setExecArgs();
		void setEnvVars();

		void execCGI();
		std::string get_buffer() { return buffer; }
};
