#include <iostream> 
#include "Server.hpp" 
#include "request.hpp" 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <exception>
#define PORT 9991
int request_fn(bool callback) 
{
 	parser  parser;
    int server_fd, new_socket; 
    struct sockaddr_in address;
    int addrlen = sizeof(address);
	const   char *hello = "Hello muscle man";
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    memset(address.sin_zero, '\0', sizeof address.sin_zero);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0 && callback == true)
    {
        perror("In bind");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
        char buffer[30000] = {0};
		// this is the buffer that will be used to store the data received from the client
		recv (new_socket, buffer, 30000, 0);
		std::string request_string(buffer);
		request request(request_string);
		request.find_host(std::string("HOst"));
		std :: cout << "---------------"  << std::endl;
		request.dispaly_map();
		std :: cout << "---------------"  << std::endl;
		std::vector<server_info> servers =  parser.get_servers();
		// here we will parse the data received from the client and store it in a vector of strings
		// and send back the right response to the client
		send (new_socket , hello , strlen(hello) , 0 );
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }
		recv (new_socket, buffer, 30000, 0);
		return (0);
}
int main()
{
	Server server("./default.conf");
	server_info temp =  server.get_servers()[0]; 

	
	//print  map of servers
	//std::map<std::string, server_info>::iterator it;
	request_fn(true);
		
	return 0;
}

