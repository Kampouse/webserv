#include "Server.hpp"

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

void server::get_content_length(std::string buf)
{
	unsigned int len = 0;
	size_t pos = buf.find("Content-Length: ");
	if (pos != std::string::npos)
	{
		pos += 16;
		while (std::isdigit(buf[pos])) {
			len *= 10;
			len += ((int)(buf[pos]) - 48);
			pos++;
		}
	}
	content_length = len;
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
	// std::string data;
	int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
	get_content_length(buf);
	buffer.clear();
	buffer.reserve(content_length + 2500);
	buffer.insert(buffer.begin(), std::begin(buf), std::begin(buf) + strlen(buf));
	if (ret == BUF_SIZE && content_length > BUF_SIZE)
	{
		std::cout << "ENTERED IF\n";
		while (ret == BUF_SIZE) {
			std::cout << "ENTERED RET\n";
			bzero(buf, BUF_SIZE);
			ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
			buffer.insert(buffer.end(), std::begin(buf), std::begin(buf) + strlen(buf));
		}
	}
	if(ret < 0){ return; }
	else if(ret == 0){ clear_fd(i); }
	else
	{
		// data = buf;
		std::string data(buffer.begin(), buffer.end());
		// std::cout << buf << "\n" << strlen(buf) << "\n" << BUF_SIZE << "\n";
		// std::cout << "DATA = \n" << data ;
		std::string path = data.substr(data.find("/"), data.find("HTTP") - data.find("/") - 1);
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
		if (page.first == "POST" && page.second == "/upload")
		{
			// std::cout << "HANDLE UPLOAD\n";
			// std::cout << "\nCONTENT-LENGTH = " << content_length << "\n";
			// std::cout << "BUFFER = \n";
			// for (std::vector<char>::iterator it = buffer.begin(); it != buffer.end(); it++)
			// 	std::cout << *it;
			// std::cout << "END BUFFER\n";
			upload(*this, page, data, content_length);
		}
		else if (page.second.find("cgi-bin") != std::string::npos)
		{

			CGI cgi(serveInfo, page);
			std::cout << "buffer->>>>>>>>>>>>>>"  <<  cgi.get_buffer();
			if (strlen(cgi.get_buffer().c_str()) != 0)
				resp  = response(cgi.get_buffer());
			else
			{
				/// correct error page here!!
				resp = response(serveInfo.locations[page.second],this->serveInfo.error_pages, data);
			}
			
		}
		else
		{
			resp = response(serveInfo.locations[page.second],this->serveInfo.error_pages, data);
		//poll_set[i].revents = 0 | POLLOUT | POLLHUP | POLLERR;
	}
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
