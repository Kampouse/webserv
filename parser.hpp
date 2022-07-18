#ifndef PARSER_HPP
# define PARSER_HPP
# include <fstream> 
# include <iostream>
# include <map>
# include <set>
# include <vector>
# include "./config_structs.hpp"
class parser
{
	private:
		std::ifstream config_file_fd;
		char *path;
		std::vector<std::map <std::string, std::string> > config_maps  ;
	public:
		parser(char *str);
		std::string get_server_path(void);
		void get_server_fields(void);

		parser (parser const &src);
		parser();
		~parser(void);
		parser	&operator = (const parser &copy);
		void  check_for_error(void);

};

#endif
