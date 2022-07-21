#include "parser.hpp"
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

int main(int argc,char **argv, char **envp)
{
	try {
		parser parsing("./default.conf");
	} catch (std::exception &e) {
		std::cout << e.what();
	}

	// parsing.printfile();

//  std::vector<parser>  vec_list;
// 	const char *defaul_config_path = "config/default.conf";
// if (argc < 2)
// 	 vec_list.push_back( parser((char *)defaul_config_path)) ;
// else
// 	{
// 			for (int i = 1; i < argc; i++)
// 			{
// 				vec_list.push_back(parser((char *)argv[i]));
// 			}
// 	}
// 	for (std::vector<parser>::iterator it = vec_list.begin(); it != vec_list.end(); ++it)
// 	{
// 		std::cout << "Server name: " << it->get_server_path() << std::endl;
// 	} 

// 	vec_list[0].check_for_error();
// 	vec_list[0].get_server_fields();


//vector of parser objects
(void)envp;
(void)argv;
(void)argc;
//create a socket
	//AF_INET - address family ipv4
	//SOCK_STREAM - TCP
	//0 - IPPROTO_IP (IP protocol)
	/*
	int socket_number = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_number == -1)
	{
		std::cout << "Error creating socket" << std::endl;
		return -1;
	}
	// Listening on port 9999
	//
	struct sockaddr_in  socket_address;
	socket_address.sin_family = AF_INET;
	socket_address.sin_port = htons(9999);
	socket_address.sin_addr.s_addr = INADDR_ANY;
	if (bind(socket_number, (struct sockaddr*)&socket_address, sizeof(socket_address)) == -1)
	{
		std::cout << "Error binding socket" << std::endl;
		return -1;
	}
	if (listen(socket_number, 5) == -1)
	{
		std::cout << "Error listening socket" << std::endl;
		return -1;
	}
	// Accepting connection
	//
  unsigned  int client_address_length = sizeof(struct sockaddr_in); 
  	struct sockaddr_in client_address;
	int client_socket_number = accept(socket_number, (struct sockaddr*)&client_address, &client_address_length);
	if (client_socket_number == -1)
	{
		std::cout << "Error accepting connection" << std::endl;
		return -1;
	}
const 	char *message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\n"; 
	send(client_socket_number ,message , strlen(message), 0); 
*/

}
