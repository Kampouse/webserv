#pragma once

# include <fstream> 
# include <iostream>
# include <map>
# include <set>
# include <vector>
# include <string>

#include "config_structs.hpp"
#include "Exceptions.hpp"

class Exceptions;

class myparser
{
	private:
		std::ifstream config_file_fd;
		// std::vector<std::map<std::string, std::string> > config_maps;
		std::vector<server_info> servers;
		std::vector<std::string> ext_file;

		void parsefile(std::string path);
		void extractfile(void);
		void check_errors(void);
		void split_servers(void);

		// std::string get_server_path(void);
		// void get_server_fields(void);


	public:
		myparser();
		myparser(std::string _path);
		~myparser();

		void printfile(void);
};