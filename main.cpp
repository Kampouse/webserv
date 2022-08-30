#include "Server.hpp"
#include "parser.hpp"

std::vector<server> lst_server;

void servers_starter(std::vector<server_info> lst_config, std::vector<server>& lst_servers)
{
	for(std::vector<server_info>::iterator it = lst_config.begin(); it != lst_config.end(); it++)
		lst_servers.push_back(server(*it));
}

void close_serv(int sig)
{
	(void)sig;

	for (size_t i = 0; i < lst_server.size(); i++) {
		while (lst_server[i].poll_set.size())
			lst_server[i].clear_fd(0);
	}

	std::cout << "\n\n";
	throw std::runtime_error("Server terminated!");
}

int main(void)
{
	signal(SIGQUIT, close_serv);
	signal(SIGINT, close_serv);

	try {
		std::cout << "server as started! " << std::endl;
		parser parser("default.conf");
		std::vector<server_info> servers_info = parser.getServers();
		
		for (std::vector<server_info>::iterator it = servers_info.begin(); it != servers_info.end(); ++it)
		{
			std::cout << "server_info: " << it->port << std::endl;
		}
		servers_starter(servers_info, lst_server);  
		while (1)
		{
			for(std::vector<server>::iterator it = lst_server.begin(); it != lst_server.end(); it++)
			{
				it->run();
			}
		}
	} catch (std::exception & e) {
		std::cerr << e.what() << "\n";
		return EXIT_FAILURE;
	}

	return (0);
}
