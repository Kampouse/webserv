#include "Server.hpp"

Server::Server() { client_fd = 0; }
Server::~Server() {}

Server::Server(std::string path)
{
	parser Parsing(path);
	servers = Parsing.getServers();
}

void Server::connect_servers(void)
{
	for (size_t i = 0; i < servers.size(); i++)
	{
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
	}
}

bool Server::canBind(int port)
{
	if (!binded_ports.empty()) {
		std::vector<int>::iterator it = binded_ports.begin();
		for (; it != binded_ports.end(); it++) {
			if (*it == port)
				return (false);
		}
	}
	return (true);
}

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
}

void Server::handle_client(std::vector<pollfd>::iterator& it, int i)
{
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
	}
}

std::vector<server_info>&Server::getServers(void)
{
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
		{
			if (ret == 0)
				throw std::runtime_error("Request Timeout [408]");
			if (ret == -1)
				throw std::runtime_error("Internal Server Error [500]");
		}

		std::vector<pollfd>::iterator it;
		for (it = fds.begin(); it != fds.end(); it++)
		{
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

	}
}

void Server::printServers(void)
{
	std::vector<server_info>::iterator it = servers.begin();
	for (; it != servers.end(); it++) {
		std::cout << "\n-------------------------------\n\n";
		std::cout << "host : " << it->host << "\n";
		std::cout << "port : " << it->port << "\n";

		std::cout << "server_names : " << it->server_names << "\n";

		std::cout << "error_pages : ";
		if (it->error_pages.size() == 0)
			std::cout << "undefined\n";
		else {
			std::cout << "\n";
			std::map<int, std::string>::iterator mit = it->error_pages.begin();
			for (; mit != it->error_pages.end(); mit++) {
				std::cout << "    " << mit->first << " " << mit->second << "\n";
			}
		}

		std::cout << "client_max_body_size : " << it->client_max_body_size << "\n";
		std::cout << "server_fd : " << it->server_fd << "\n";

		std::cout << "locations : ";
		if (it->locations.size() == 0)
			std::cout << "undefined\n";
		else {
			std::cout << "\n";
			std::map<std::string, location_info>::iterator mit2 = it->locations.begin();
			for (; mit2 != it->locations.end(); mit2++) {
				std::cout << mit2->first << "\n";
				std::cout << "    root : " << mit2->second.root << "\n";
				std::cout << "    index : " << mit2->second.index << "\n";
				std::cout << "    upload_dir : " << mit2->second.upload_dir << "\n";
				std::cout << "    autoindex : " << ((mit2->second.autoindex) ? "true" : "false") << "\n";

				std::cout << "    cgi : ";
				if (mit2->second.cgi.size() == 0)
					std::cout << "undefined\n";
				else {
					std::cout << "\n";
					std::map<std::string, std::string>::iterator mit3 = mit2->second.cgi.begin();
					for (; mit3 != mit2->second.cgi.end(); mit3++) {
						std::cout << "        " << mit3->first << " " << mit3->second << "\n";
					}
				}

				std::cout << "    allowed_requests : ";
				if (mit2->second.allowed_requests.size() == 0)
					std::cout << "undefined\n";
				else {
					std::cout << "\n";
					std::vector<std::string>::iterator it2 = mit2->second.allowed_requests.begin();
					for (; it2 != mit2->second.allowed_requests.end(); it2++) {
						std::cout << "        " << *it2 << "\n";
					}
				}
			}
		}
	}

	std::cout << "\n-------------------------------\n";
}
