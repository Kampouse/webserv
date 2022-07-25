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

		if (setsockopt(servers[i].server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
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