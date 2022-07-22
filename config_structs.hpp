#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdbool.h>
#include <netinet/in.h>

#define WHITESPACES "\t\n\v\f\r "

struct location_info {
	std::string							root;
	std::map<std::string, std::string>	cgi;
	std::vector<std::string>			allowed_requests;
	std::string							index;
	std::string							upload_dir;
	bool								autoindex;

	location_info() {
		autoindex = false;
	}
};

struct server_info {
	std::string								host;
	int										port;
	std::string								server_names;
	std::map<int, std::string>				error_pages;
	unsigned int							client_max_body_size;
	int										server_fd;
	struct sockaddr_in						address;
	std::map<std::string, location_info>	locations;

	server_info() {
		client_max_body_size = 0;
		port = 0;
		server_fd = 0;
		server_names = "localhost";
	}
};

typedef enum parsing_state {
	CONFIG_FIELD,
	BRACE
} parsing_state;