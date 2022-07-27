#include "Server.hpp"

Server::Server() {}
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

void Server::handle_listen(int i)
{
	sockaddr_storage client_addr;
	socklen_t addrlen = sizeof(client_addr);

	int client_fd = accept(fds[i].fd, (struct sockaddr*)&client_addr, &addrlen);

	if (client_fd == -1)
		throw std::runtime_error("Accept Error");

	fds.push_back(pollfd());
	fds.back().fd = fds[i].fd;
	fds.back().events = POLLIN;
	fds.back().revents = 0;
}

void Server::handle_client(std::vector<pollfd>::iterator& it, int i)
{
	char buf[4096];
	int sender_fd = fds[i].fd;
	int nbytes = recv(sender_fd, buf, 4096, 0);

	if (nbytes <= 0)
	{
		if (nbytes < 0)
			;// print error, not throw???
		close(fds[i].fd);
		fds.erase(it);
		it = fds.begin();
	}
	else
	{
		for (size_t j = 0; j < nbytes; j++)
			request.push_back(buf[j]);

		// Request parsing

		std::string header = ""; // Get Request Header
		std::string body = ""; // Get Request Body

		size_t size = header.length() + body.length() + 1;

		char * buffer = new char[size];
		bzero(buffer, size);

		memcpy(buffer, header.data(), header.length());
		memcpy(buffer + header.length(), body.data(), body.length());

		send(sender_fd, buffer, size, 0);
		delete[] buffer;
		bzero(buf, 4096);
		request.clear();
	}
}

void Server::run(void)
{
	int ret;

	while (true)
	{
		if ((ret = poll(&(fds.front()), fds.size(), 10000)) <= 0)
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
				for (size_t i = 0; i < fds.size(); i++) {
					if (it->fd == fds[i].fd)
						handle_listen(i);
					else
						handle_client(it, i);
				}
			}
		}
	}
}