#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <string>
#include <list>
#include <sys/fcntl.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<fcntl.h>
#include<errno.h>
#include<unistd.h>
#include "poll.h" 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <vector>
<<<<<<< HEAD:Server.hpp
#include"Exceptions.hpp"
#include"config_structs.hpp"
#include"parser.hpp"
#include"response.hpp"
#include"utils.hpp"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF
=======
#include <exception>
#include <stdbool.h>
#include <fcntl.h>

#include "parser.hpp"
#include "config_structs.hpp"
#include "Exceptions.hpp"
#include "utils.hpp"

class Server
{
	private:
		std::vector<server_info>	servers;
		std::vector<int>			binded_ports;
		std::vector<pollfd>			fds;
		std::string					request;
		int							client_fd;
		sockaddr_storage client_addr;

>>>>>>> origin/gasselin:Garbage/Server.hpp


<<<<<<< HEAD:Server.hpp
class response;
class   server {
=======
		void handle_listen(std::vector<pollfd>::iterator& it);
		void handle_client(std::vector<pollfd>::iterator& it, int i);
>>>>>>> origin/gasselin:Garbage/Server.hpp

	public:
			std::vector<pollfd> poll_set;
		   struct sockaddr_in server_addr;
		   server_info serveInfo;
		   int server_fd;
		   response resp;
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

};

	 std::string trim(const std::string& str);
	 location_info find_page(server &serv, std::string &path);
	 std::string content_typer(std::vector<std::string> &content_type,int index);
#endif
