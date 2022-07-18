#ifndef PARSER_HPP
# define PARSER_HPP
# include <fstream> 
# include <iostream>
# include <map>
# include <set>
# include <vector>
# include <string>

#include "config_structs.hpp"

class parser
{
	private:
		std::ifstream config_file_fd;
		char *path;
		std::vector<std::map<std::string, std::string> > config_maps;
		std::vector<server_info> servers;
		std::vector<std::string> server_confs;

	public:
		parser();
		~parser();
		parser (parser const &src);
		parser(char *str);

		std::string get_server_path(void);
		void get_server_fields(void);

		parser	&operator = (const parser &copy);
		void  check_for_error(void);

};

#endif
