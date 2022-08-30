#include "Server.hpp"
#include "parser.hpp"

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

void close_serv(int sig)
{
	(void)sig;
	std::cout << "\n\n";
	throw std::runtime_error("Server terminated!");
	exit (1);
}

int main(void)
{
	signal(SIGQUIT, close_serv);
	signal(SIGINT, close_serv);

	std::cout << "server as started! " << std::endl;
	parser parser("default.conf");
	std::vector<server_info> servers_info = parser.getServers();
	
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
