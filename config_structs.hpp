#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdbool.h>

struct location_info {
	// cgi
	std::string					root;
	std::vector<std::string>	allowed_requests;
	std::string					index;
	bool						autoindex;
	bool						upload;
	bool						redirection;
};

struct server_info {
	std::string					host;
	int							port;
	std::string					server_names;
	std::map<int, std::string>	error_pages;
	unsigned int				client_max_body_size;
	std::vector<location_info>	locations;
};