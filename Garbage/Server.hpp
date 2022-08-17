#pragma once

#include <iostream> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/poll.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
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


		bool canBind(int port);

		void handle_listen(std::vector<pollfd>::iterator& it);
		void handle_client(std::vector<pollfd>::iterator& it, int i);

	public:
		Server();
		Server(std::string path);
		~Server();

		std::vector<server_info>& getServers();

		void connect_servers(void);
		void run(void);

		void printServers(void);
};