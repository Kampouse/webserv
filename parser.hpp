#pragma once

# include <fstream> 
# include <iostream>
# include <map>
# include <set>
# include <vector>
# include <string>
# include <stdlib.h>

#include "config_structs.hpp"
#include "Exceptions.hpp"
#include "utils.hpp"

class Exceptions;

class parser
{
	private:
		std::ifstream config_file_fd;
		std::vector<server_info> servers;
		std::vector<std::string> ext_file;

		void parsefile(std::string path);
		void extractfile(void);
		void check_errors(void);
		void get_server_fields(void);
		void manage_locations(std::vector<std::string>::iterator it);

	public:
		std::vector<server_info>&get_servers(void);
		parser();
		parser(std::string _path);
		~parser();
		void printfile(void);
};
