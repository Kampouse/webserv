#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdbool.h>

#define WHITESPACES "\t\n\v\f\r "

struct location_info {
	// cgi
	std::string					location;
	std::string					root;
	std::vector<std::string>	allowed_requests;
	std::string					index;
	bool						autoindex;

	location_info() {
		autoindex = false;
	}
};

struct server_info {
	std::string					host;
	int							port;
	std::string					server_names;
	std::map<int, std::string>	error_pages;
	unsigned int				client_max_body_size;
	std::vector<location_info>	locations;

	server_info() {
		client_max_body_size = 0;
		port = 0;
	}
};

typedef enum parsing_state {
	CONFIG_FIELD,
	BRACE
} parsing_state;