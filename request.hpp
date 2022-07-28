#ifndef REQUEST_HPP
# define REQUEST_HPP

# include <iostream>
# include "map"

class request
{
	private:
		const std::string _request_string;
		std::string _method;
		std::string _location;
		std::string _path;
		std::string _version;
		std::map<std::string, std::string>  _req_map;
		public:
		request(void);
		request(std::string &request_string);
		~request(void);
		request(const request &copy);
		request	&operator = (const request &copy);
		void	find_host(std::string string) ;
		void	dispaly_map(void);
};

#endif
