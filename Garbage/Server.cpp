#include "Server.hpp"	
#include <sstream>
#include <filesystem>

<<<<<<< HEAD:Server.cpp
server::server(server_info servInfo)
{
=======
Server::Server() { client_fd = 0; }
Server::~Server() {}

Server::Server(std::string path)
{
	parser Parsing(path);
	servers = Parsing.getServers();
}
>>>>>>> origin/gasselin:Garbage/Server.cpp

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(servInfo.port);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
	serveInfo = servInfo;
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	fcntl(server_fd, F_SETFL, O_NONBLOCK);
	setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr, sizeof(server_addr));
	bzero(&(server_addr.sin_zero), 8);
	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
<<<<<<< HEAD:Server.cpp
		return;
=======
		if ((servers[i].server_fd = socket(AF_INET, SOCK_STREAM, 0)) <= 2)
			throw std::runtime_error("Socket failed");

		// Non-blocking socket, only on MacOS
		fcntl(servers[i].server_fd, F_SETFL, O_NONBLOCK);

		int optval = 1;
		if (setsockopt(servers[i].server_fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int)) < 0)
			throw std::runtime_error("Failed to set options");

		bzero(&servers[i].address, sizeof(servers[i].address));
		servers[i].address.sin_family = AF_INET;
		servers[i].address.sin_port = htons(servers[i].port);
		servers[i].address.sin_addr.s_addr = inet_addr(servers[i].host.c_str());

		if (canBind(servers[i].port)) {
			if (bind(servers[i].server_fd, (struct sockaddr *)&servers[i].address, sizeof(servers[i].address)) < 0)
				throw std::runtime_error("Bind failed");
			binded_ports.push_back(servers[i].port);
		}

		if (listen(servers[i].server_fd, 10) < 0)
			throw std::runtime_error("Listen failed");

		pollfd newfd;
		newfd.fd = servers[i].server_fd;
		newfd.events = POLLIN;
		newfd.revents = 0;
		fds.push_back(newfd);
>>>>>>> origin/gasselin:Garbage/Server.cpp
	}

	listen(server_fd, 100);
	pollfd serv;
	serv.fd = server_fd;
	serv.events = POLLIN | POLLHUP | POLLERR;
	serv.revents = 0;
	poll_set.push_back(serv);
	contents.push_back(".css");
	contents.push_back(".html");
	contents.push_back(".js");
	contents.push_back(".png");
	contents.push_back(".jpg");
	contents.push_back(".jpeg");
	contents.push_back(".gif");
	contents.push_back(".ico");

}

<<<<<<< HEAD:Server.cpp
void   server::clear_fd (int i)
{
	close(poll_set[i].fd);
	poll_set[i].fd = -1;
	poll_set.erase(poll_set.begin() + i);
=======
void Server::handle_listen(std::vector<pollfd>::iterator& it)
{
	socklen_t addrlen = sizeof(client_addr);

	// ssize_t valread;
	// std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	client_fd = accept(it->fd, (struct sockaddr*)&client_addr, &addrlen);

	if (client_fd == -1)
		throw std::runtime_error("Accept Error");

	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	char buf[4096];
	size_t nbytes = recv(it->fd, buf, 4096, 0);

	if (nbytes <= 0)
	{
		if (nbytes < 0)
			;// print error, not throw???
		close(it->fd);
		fds.erase(it);
		it = fds.begin();
	}
	else
	{
		send(it->fd, hello.c_str(), hello.length(), 0);
		bzero(buf, 4096);

		fds.push_back(pollfd());
		fds.back().fd = it->fd;
		fds.back().events = POLLIN;
		fds.back().revents = 0;
	}

	// char buffer[30000] = {0};

	// this is the buffer that will be used to store the data received from the client
	// valread = recv( client_fd , buffer, 30000, 0);
	// std::cout << buffer << "\n";

	// here we will parse the data received from the client and store it in a vector of strings
	// and send back the right response to the client
	// send(client_fd, hello.c_str(), hello.length(), 0);
	// std::cout << "------------------Hello message sent-------------------\n";

	// close(client_fd);


	// it = fds.begin();
	// while (it->fd != fds[i].fd)
	// 	it++;
>>>>>>> origin/gasselin:Garbage/Server.cpp
}

void server::add_client (void)
{
<<<<<<< HEAD:Server.cpp
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd;
	pollfd client;
	if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0)
	{
		perror("accept");
		return;
		//exit(1);
=======
	char buf[4096];
	size_t nbytes = recv(fds[i].fd, buf, 4096, 0);
	std::string hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

	if (nbytes <= 0)
	{
		if (nbytes < 0)
			;// print error, not throw???
		close(it->fd);
		fds.erase(it);
		it = fds.begin();
	}
	else
	{
		// for (size_t j = 0; j < nbytes; j++)
		// 	request.push_back(buf[j]);

		// Request parsing

		// std::string header = ""; // Get Request Header
		// std::string body = ""; // Get Request Body

		// size_t size = header.length() + body.length() + 1;

		// char buffer[size];
		// bzero(buffer, size);

		// memcpy(buffer, header.data(), header.length());
		// memcpy(buffer + header.length(), body.data(), body.length());

		send(fds[i].fd, hello.c_str(), hello.length(), 0);
		bzero(buf, 4096);
		// request.clear();
>>>>>>> origin/gasselin:Garbage/Server.cpp
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	client.fd = client_fd;
	client.events = POLLIN | POLLHUP | POLLERR;
	client.revents = 0;
	poll_set.push_back(client);
}

void server::get_data_from_client(int i)
{
<<<<<<< HEAD:Server.cpp
		char buf[BUF_SIZE];
		std::string data;
		int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
		if(ret < 0){ return;}
		else if(ret == 0){clear_fd(i);}
		else
=======
	return (servers);
}

void Server::run(void)
{
	int ret, new_socket;
	// struct sockaddr_in address;
	// int addrlen = sizeof(address);
	(void)new_socket;

	while (true)
	{
		std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";

		if ((ret = poll(&(fds.front()), fds.size(), -1)) <= 0)
>>>>>>> origin/gasselin:Garbage/Server.cpp
		{
			data = buf;
			std::string path = data.substr(data.find("/"), data.find("HTTP") - 4);
			for (unsigned int i = 0; i < contents.size(); i++)
		{
<<<<<<< HEAD:Server.cpp
           if (path.find(contents[i]) != std::string::npos)
		   {
			   std::string root_ext;
			   if(contents[i] == ".css")
				   root_ext = "/styles";
			   else if(contents[i] == ".html")
				    continue;
			   else if(contents[i] == ".js")
				   root_ext = "/scripts";
			   else
				   root_ext = "/images";
			   std::cout << this->serveInfo.locations["/"].root + root_ext  << path << std::endl;

			   std::cout << content_typer(contents,i) << std::endl;
			   std::string pathed = trim(this->serveInfo.locations["/"].root + root_ext +  path);
			   std::ifstream file;
			   file.open(pathed.c_str());
			   if (!file.is_open())
			   {
				   //  what to send to request when the content is not found
				   std::cout << "file not found" << std::endl;
			   }
			   else
			   {
				   file.close();
				   std::string content_type = content_typer(contents, i);
				    resp = response(pathed, content_type);
					return;
			   }
			}
		}
			resp =  response(find_page(*this, data),this->serveInfo.error_pages,data);
			//poll_set[i].revents = 0 | POLLOUT | POLLHUP | POLLERR;
		}
}
void server::get_data_from_server(int i)
{
	std::string http_response =  resp.build_response();
	int ret = send(poll_set[i].fd, http_response.c_str(), http_response.length(), 0);
	if(ret < 0){ return;}
	else if(ret == 0){clear_fd(i);}
	else
	{
		clear_fd(i);
=======
			if (it->revents & POLLIN) {
				// for (size_t i = 0; i < fds.size(); i++) {
				// 	if (it->fd == fds[i].fd) //&& it->fd != client_fd)
				// 	{
						handle_listen(it);
						// break ;
					// }
					// else// if (it->fd == fds[i].fd)
					// {
					// 	handle_client(it, i);
					// 	// break ;
					// }
				// }
			}
		}

		// if ((new_socket = accept(servers[0].server_fd, (struct sockaddr *)&servers[0].address, (socklen_t*)&addrlen)) < 0)
		// {
		// 	perror("In accept");
		// 	exit(EXIT_FAILURE);
		// }

		// char buffer[30000] = {0};

		// this is the buffer that will be used to store the data received from the client
		// valread = recv( new_socket , buffer, 30000, 0);
		// std::cout << buffer << "\n";

		// here we will parse the data received from the client and store it in a vector of strings
		// and send back the right response to the client
		// send(new_socket, hello.c_str(), hello.length(), 0);
		// std::cout << "------------------Hello message sent-------------------\n";

		// close(new_socket);

>>>>>>> origin/gasselin:Garbage/Server.cpp
	}
	std::cout << "closed" << std::endl;
	(void)ret;
}
void server::run()
{
	poll (poll_set.data(),poll_set.size() , 50);
	for(unsigned long i = 0; i <  poll_set.size();i++)
	{
		if(poll_set[i].revents & POLLIN)
		{
			if(poll_set[i].fd == server_fd)
				add_client();
			else
			{
				get_data_from_client(i);
				if(poll_set[i].revents & POLLIN)
				{
					get_data_from_server(i);
				}
			}
		}
		if (poll_set[i].revents & POLLOUT )
		{
			get_data_from_server(i);
		}
	}
}
