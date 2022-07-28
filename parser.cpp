#include "parser.hpp"
#include "config_structs.hpp"
#include <iterator>
#include <sstream>
#include <string>
#include <cstdlib>
#include <exception>

parser::parser() {}

parser::~parser() {}

parser::parser(std::string _path) {
	parsefile(_path);
}

static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(WHITESPACES);
    if (first == std::string::npos)
        return str;
    size_t last = str.find_last_not_of(WHITESPACES);
    return str.substr(first, (last - first + 1));
}

void parser::extractfile()
{
	std::string line;
	size_t pos;
	while (getline(config_file_fd, line))
	{
		line = trim(line);
		while ((pos = line.find_first_of("{}")) != std::string::npos || line.length())
		{
			if (pos == std::string::npos)
			{
				ext_file.push_back(line);
				break;
			}
			else if (pos == 0)
			{
				ext_file.push_back(line.substr(0, 1));
				line = line.substr(1, std::string::npos);
			}
			else
			{
				ext_file.push_back(line.substr(0, pos));
				line = line.substr(pos, std::string::npos);
			}
			line = trim(line);
		}
		line.clear();
	}
}

void parser::check_errors(void) {
	std::string line;
	int bracket_state = 0;
	parsing_state state = CONFIG_FIELD;
	std::vector<std::string>::iterator it = ext_file.begin();

	while (it != ext_file.end()) {
		// this line assume that the config file is well formed
		if (*it == "server" && bracket_state != 0)
			throw Exceptions::NestedServerError();

		if (bracket_state == 0 && *it != "server")
			throw Exceptions::ConfigError();

		if (*it == "server" || ((*it).find("location") == 0 && (*it).find('/') != std::string::npos)) {
			if (*(++it) != "{")
				throw Exceptions::ConfigError();
			state = BRACE;
			bracket_state++;
		} else if (*it == "{") {
			state = BRACE;
			bracket_state++;
		} else if (*it == "}") {
			state = BRACE;
			bracket_state--;
		}
		else
			state = CONFIG_FIELD;

		if (bracket_state == -1)
			throw Exceptions::ConfigError();

		if (state == CONFIG_FIELD && (*it).back() != ';')
			throw Exceptions::SemicolonError();
		it++;
	}

	if (bracket_state != 0)
		throw Exceptions::ConfigError();
}

void parser::manage_locations(std::vector<std::string>::iterator it)
{
	size_t pos;
	std::string data, field, location;

	location = (*it).substr((*it).find('/'), std::string::npos);
	servers.back().locations.insert(std::pair<std::string, location_info>(location, location_info()));
	it += 2;
	while (*it != "}")
	{
		pos = (*it).find_first_of(WHITESPACES);
		field = (*it).substr(0, pos);
		data = (*it).substr(pos, (*it).find_first_of(';') - pos);
		data = trim(data);

		if (field == "root") {
			servers.back().locations[location].root = data;
		}
		else if (field == "cgi_ext") {
			std::string cgi, root;
			cgi = data.substr(0, data.find_first_of(WHITESPACES));
			root = data.substr(cgi.length(), std::string::npos);
			root = trim(root);
			servers.back().locations[location].cgi.insert(std::pair<std::string, std::string>(cgi, root));
		}
		else if (field == "index") {
			servers.back().locations[location].index = data;
		}
		else if (field == "autoindex") {
			if (data != "on" && data != "off")
				throw Exceptions::InvalidFieldError("autoindex");
			servers.back().locations[location].autoindex = ((data == "on") ? true : false);
		}
		else if (field == "upload_dir") {
			servers.back().locations[location].upload_dir = data;
		}
		else if (field == "allow_request") {
			while (data.length()) {
				servers.back().locations[location].allowed_requests.push_back(data.substr(0, data.find_first_of(WHITESPACES)));
				if (data.find_first_of(WHITESPACES) == std::string::npos)
					data = "";
				else
				{
					data = data.substr(data.find_first_of(WHITESPACES), std::string::npos);
					data = trim(data);
				}
			}
		}
		else if (field == "return") {
			;
		}
		else
			throw Exceptions::UnknownFieldError();
		it++;
	}
}

void parser::get_server_fields(void)
{
	std::vector<std::string>::iterator it = ext_file.begin();
	size_t pos;
	std::string data, field;

	servers.push_back(server_info());
	while (++it != ext_file.end())
	{
		if (*it == "server")
			servers.push_back(server_info());
		else if (*it == "{" || *it == "}")
			continue;
		else if ((*it).find("location") == 0 && (*it).find('/') != std::string::npos) {
			manage_locations(it);
			while (*it != "}")
				it++;
		}
		else {
			pos = (*it).find_first_of(WHITESPACES);
			field = (*it).substr(0, pos);
			data = (*it).substr(pos, (*it).find_first_of(';') - pos);
			data = trim(data);

			if (field == "listen") {
				if (std::count(data.begin(), data.end(), ':') != 1)
					throw Exceptions::InvalidFieldError("listen");
				pos = data.find(':');

				servers.back().host = data.substr(0, pos);
				data = data.substr(pos + 1, std::string::npos);
				servers.back().port = atoi(data.c_str());
			}
			else if (field == "server_name") {
				servers.back().server_names = data;
			}
			else if (field == "client_max_body_size") {
				if (!std::isdigit(data[0]))
					throw Exceptions::InvalidFieldError("client_max_body_size");
				servers.back().client_max_body_size = atoi(data.c_str());
				pos = 0;
				while (std::isdigit(data[pos]))
					pos++;
				if (data.length() != pos + 1 || (data[pos] != 'm' && data[pos] != 'M'))
					throw Exceptions::InvalidFieldError("client_max_body_size");
			}
			else if (field == "error_page") {
				if (!std::isdigit(data[0]))
					throw Exceptions::InvalidFieldError("error_page");
				servers.back().error_pages.insert(std::pair<int, std::string>(atoi(data.c_str()), \
										data.substr(data.find_last_of(WHITESPACES) + 1, std::string::npos)));
			}
			else
				throw Exceptions::UnknownFieldError();
		}
	}
}

void parser::parsefile(std::string path)
{
	this->config_file_fd.open(path.c_str());
	if (this->config_file_fd.fail())
		throw Exceptions::FileOpeningError(path);
	extractfile();
	this->config_file_fd.close();
	// printfile();
	check_errors();
	get_server_fields();
}

std::vector<server_info>&parser::getServers() { return (servers); }

void parser::printfile(void)
{
	std::vector<std::string>::iterator it = ext_file.begin();
	while (it != ext_file.end())
		std::cout << *it++ << "\n";
}
std::vector<server_info>&parser::get_servers(void)
{
	return servers;
}
