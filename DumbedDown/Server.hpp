#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <string>
#include <list>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include "poll.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <utility>
#include <sstream>
#include <filesystem>

#include "Exceptions.hpp"
#include "config_structs.hpp"
#include "parser.hpp"
#include "response.hpp"
#include "utils.hpp"
#include "CGI.hpp"
#include "upload.hpp"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF


class response;

class   server {

	public:
		std::vector<pollfd> poll_set;
		struct sockaddr_in server_addr;
		server_info serveInfo;
		int server_fd;
		response resp;
		std::vector<char> buffer;
		size_t content_length;
	
		server(){};
		server( struct server_info serv);
		server(std::string path);
		~server(){};
	
		void add_client(void);
		void clear_fd(int i);
		void get_data_from_client(int i);
		void get_data_from_server(int i);
		void run();

		std::string response_to_client(location_info &local_info);

	private :
		std::vector<std::string> contents;
		void get_content_length(std::string buf);

};

	 std::string trim(const std::string& str);
	 std::pair<std::string, std::string> find_page(server &serv, std::string &path);
	 std::string content_typer(std::vector<std::string> &content_type,int index);
#endif
