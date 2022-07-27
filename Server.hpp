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


		bool canBind(int port);

		void handle_listen(int i);
		void handle_client(std::vector<pollfd>::iterator& it, int i);

	public:
		Server();
		Server(std::string path);
		~Server();

		void connect_servers(void);
		void run(void);
};