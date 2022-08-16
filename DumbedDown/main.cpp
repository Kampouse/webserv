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
#include <vector>

#include "Exceptions.hpp"
#include "config_structs.hpp"
#include "parser.hpp"
#include "utils.hpp"
#include "server.hpp"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8888
#define EPOLL_SIZE 5000
#define BUF_SIZE 0xFFFF



int nonblock(int sockfd)
{
	fcntl(sockfd, F_SETFL, O_NONBLOCK);
	return 0;
}


std::vector<server> servers_starter(std::vector<server_info> lst_config)
{
	std::vector<server> lst_servers;
	for(std::vector<server_info>::iterator it = lst_config.begin(); it != lst_config.end(); it++)
	{
		server s(*it);
		lst_servers.push_back(s);
	}
	return(lst_servers);
}

int main(void)
{

	std::cout << "server as started! " << std::endl;

	// handle server socket so it avoid  sigpipe
	// signal(SIGPIPE, SIG_IGN);
	parser parser("default.conf");
	std::vector<server_info> servers_info = parser.get_servers();
	
	for (std::vector<server_info>::iterator it = servers_info.begin(); it != servers_info.end(); ++it)
	{
		std::cout << "server_info: " << it->port << std::endl;
	}

	std::vector<server> lst_server = servers_starter(servers_info);  
	while (1)
	{
		for(std::vector<server>::iterator it = lst_server.begin(); it != lst_server.end(); it++)
		{
			it->run();
		}
	}
	return (0);

}

