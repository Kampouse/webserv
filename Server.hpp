#pragma once

#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <vector>
#include <poll.h>
#include <dirent.h>

#include "Exceptions.hpp"
#include "config_structs.hpp"
#include "parser.hpp"
#include "response.hpp"
#include "CGI.hpp"
#include "upload.hpp"

#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 0xFFFF

class response;

class server {
	public:
		server_info serveInfo;
		response resp;
	
		server() {};
		server(struct server_info serv);
		server(std::string path);
		~server() {};
	
		void add_client(void);
		void clear_fd(int i);
		void run();

		std::string response_to_client(location_info &local_info);
		unsigned int getTotalRet() { return total_ret; }

	private :
		std::vector<pollfd> poll_set;
		struct sockaddr_in server_addr;
		int server_fd;
		std::vector<char> buffer;
		unsigned int total_ret;
		unsigned int content_length;
		std::vector<std::string> contents;

		void get_content_length(std::string buf);
		void delete_upload(std::string path);
		void get_data_from_client(int i);
		void get_data_from_server(int i);

};

	std::string trim(const std::string& str);
	std::pair<std::string, std::string> find_page(std::string &path);
	std::string content_typer(std::vector<std::string> &content_type,int index);
