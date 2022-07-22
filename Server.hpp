#pragma once

#include <iostream> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

		bool canBind(int port);

	public:
		Server();
		Server(std::string path);
		~Server();

		void connect_servers(void);
};