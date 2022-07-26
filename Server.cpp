#include "Server.hpp"


Server::Server() {}
Server::~Server() {}

Server::Server(std::string path)
{
	parser Parsing(path);
	servers = Parsing.getServers();
}
std::vector<server_info>&Server::getServers()
{
	return servers;
}

