#include "myparser.hpp"
#include "config_structs.hpp"
#include <iterator>
#include <sstream>
#include <string>
#include <cstdlib>
#include <exception>

myparser::myparser() {}

myparser::~myparser() { this->config_file_fd.close(); }

myparser::myparser(std::string _path) {
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

void myparser::extractfile()
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

void myparser::check_errors(void) {
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

		if (*it == "server" || (*it).find("location /") == 0) {
			if (*(++it) != "{")
				throw Exceptions::ConfigError();
			state = BRACE;
			bracket_state = 1;
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

		if (state != CONFIG_FIELD)
			continue ;
		else if (state == CONFIG_FIELD && (*it).back() == ';')
			continue ;
		else
			throw Exceptions::SemicolonError();
		it++;
	}

	if (bracket_state != 0)
		throw Exceptions::ConfigError();
}

void myparser::get_server_fields(void)
{
	std::vector<std::string>::iterator it = ext_file.begin();
	size_t pos;
	std::string data, field;

	servers.push_back();
	while (++it != ext_file.end())
	{
		if (*it == "server")
			servers.push_back();
		else if ((*it).find("location") == 0 && (*it).find('/') != std::string::npos) {
			// locations
		}
		else if (*it == "{" || *it == "}")
			continue;
		else {
			pos = (*it).find_first_of(WHITESPACES);
			field = (*it).substr(0, pos);
			data = (*it).substr(pos, std::string::npos);
			data = trim(data);

			if (field.find("listen") == 0) {
				if (std::count(data.begin(), data.end(), ':') != 1)
					throw Exceptions::InvalidFieldError("listen");
				pos = data.find(':');
				servers.back().host = data.substr(0, pos);
				servers.back().port = data.substr(pos + 1, std::string::npos);
			}
			else if ((*it).find("server_name") == 0) {
				servers.back().server_names = data;
			}
			else if ((*it).find("client_max_body_size") == 0) {
				if (!std::isdigit(*data))
					throw Exceptions::InvalidFieldError("client_max_body_size");
				servers.back().client_max_body_size = atoi(data.c_str());
				pos = std::to_string(servers.back().client_max_body_size).length();
				if (data.length() != pos + 1 || data[pos] != 'm' || data[pos] != 'M')
					throw Exceptions::InvalidFieldError("client_max_body_size");
			}
			else if ((*it).find("error_page") == 0) {
				if (!std::isdigit(*data))
					throw Exceptions::InvalidFieldError("error_page");
				servers.back().error_pages.insert(std::pair<int, std::string>(atoi(data.c_str()), \
										data.substr(data.find_last_of(WHITESPACES), std::string::npos)));
			}
			else
				throw Exceptions::UnknownFieldError();
		}
	}
}

void myparser::parsefile(std::string path)
{
	this->config_file_fd.open(path.c_str());
	if (this->config_file_fd.fail())
		throw Exceptions::FileOpeningError(path);
	extractfile();
	check_errors();
	get_server_fields();
}

void myparser::printfile(void)
{
	std::vector<std::string>::iterator it = ext_file.begin();
	while (it != ext_file.end())
		std::cout << *it++ << "\n";
}