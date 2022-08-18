#include "Server.hpp"	
#include <sstream>
#include <filesystem>

server::server(server_info servInfo)
{

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
		return;
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

void   server::clear_fd (int i)
{
	close(poll_set[i].fd);
	poll_set[i].fd = -1;
	poll_set.erase(poll_set.begin() + i);
}

void server::add_client (void)
{
	struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	int client_fd;
	pollfd client;
	if ((client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len)) < 0)
	{
		perror("accept");
		return;
		//exit(1);
	}
	fcntl(client_fd, F_SETFL, O_NONBLOCK);
	client.fd = client_fd;
	client.events = POLLIN | POLLHUP | POLLERR;
	client.revents = 0;
	poll_set.push_back(client);
}

void server::get_data_from_client(int i)
{
	char buf[BUF_SIZE];
	std::string data;
	int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
	if(ret < 0){ return; }
	else if(ret == 0){ clear_fd(i); }
	else
	{
		data = buf;
		std::cout << data ;
		std::string path = data.substr(data.find("/"), data.find("HTTP") - 4);
		for (unsigned int i = 0; i < contents.size(); i++)
		{
			if (path.find(contents[i]) != std::string::npos)
			{
				std::cout << content_typer(contents,i) << std::endl;
				std::string pathed = trim(this->serveInfo.locations["/"].root +  path);
				std::ifstream file;

				file.open(pathed.c_str());
				if (!file.is_open())
				{

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
		std::pair<std::string, std::string> page = find_page(*this, data);
		if (page.second.find("cgi-bin") != std::string::npos)
		{
			CGI cgi(serveInfo, page);
		}
		else
			resp = response(serveInfo.locations[page.second],this->serveInfo.error_pages, data);
		// std::cout << resp. << "\n";
		//poll_set[i].revents = 0 | POLLOUT | POLLHUP | POLLERR;
	}
}

void server::get_data_from_server(int i)
{
	std::string http_response =  resp.build_response();
	int ret = send(poll_set[i].fd, http_response.c_str(), http_response.length(), 0);

	if (ret < 0) { return; }
	else { clear_fd(i); }

	std::cout << "closed" << std::endl;
}

void server::run()
{
	poll(poll_set.data(),poll_set.size(), 50);
	for(unsigned long i = 0; i <  poll_set.size(); i++)
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
		if (poll_set[i].revents & POLLOUT)
		{
			get_data_from_server(i);
		}
	}
}
