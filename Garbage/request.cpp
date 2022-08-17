#include "request.hpp"
#include "Server.hpp" 
#include "utils.hpp"
#include <sstream>
#include <string>
#include "map"

static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of("\n \r\t");
    if (first == std::string::npos)
        return str;
    size_t last = str.find_last_not_of("\n \r\t");
    return str.substr(first, (last - first + 1));
}
request::request(std::string&request_string): _request_string(request_string)
{
	std::map<std::string,std::string> req_map;
	std::string::iterator it; 
	std::string::iterator it_second;
	std::string key;
	std::string value;
	it = request_string.begin();
	while (it != request_string.end())
	{
		if (*it == ' ')
		{
			it++;
			break;
		}
		it++;
	}
	it_second = it;
	 _method = request_string.substr(0, it - request_string.begin());
	while (it != request_string.end())
	{
		if (*it == ' ')
		{
			it++;
			break;
		}
		it++;
	}
	_location = request_string.substr(it_second - request_string.begin(), it - it_second);
	_version =   trim(request_string.substr(request_string.find ("HTTP") , request_string.find ("Host") - request_string.find ("HTTP")));
	int temp  =  request_string.find ("\r\n") + 2;
	it = request_string.begin() + temp;
	it_second = it;
	unsigned long i  = temp;
	unsigned long j  = temp;
	std::vector <std::string> result;
	while (i < _request_string.length() && j < _request_string.length())
	{
		i = _request_string.find("\r\n", j);
		if (i == std::string::npos)
			i = _request_string.length();
		std::string str = _request_string.substr(j, i - j);
		if (!str.empty())
			result.push_back(str);
		j = _request_string.find_first_not_of("\r\n", i);
	}
	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
	{
		std::string str = *it;
		if (str.find(":") != std::string::npos)
		{
			 key = str.substr(0 , str.find(":"));
			value = str.substr(str.find(":") + 1);
			_req_map[key] = trim(value);
		}
	}
}

void request::dispaly_map(void)
{
	std::map<std::string, std::string>::iterator it;
	std::cout << _version << std::endl;
	for (it = _req_map.begin(); it != _req_map.end(); it++)
		std::cout << "key:-> " << it->first << "value:-> " << it->second  << std::endl;
}

request::request(void): _request_string (""){}

request::~request(void)
{
	std::cout << "Destructor called" << std::endl;
}

request::request(const request &copy)
{
	std::cout << "Copy constructor called" << std::endl;
	*this = copy;
}

request	&request::operator = (const request &copy)
{
	(void )copy;
	std::cout << "Assignation operator called" << std::endl;
	return (*this);
}

 int request::find_host_index(std::vector<server_info>&server) 
{
	for (std::vector<server_info>::iterator it = server.begin(); it != server.end(); ++it)
	{
		std::string temp  = _req_map["Host"];
		temp = temp.substr(temp.find(":") + 1);
		if  (temp != "")
		{
			std::cout <<  atoi(temp.c_str()) << "<-> " << it->port << std::endl;
			 if( atoi(temp.c_str())  == it->port)
				return (it - server.begin());
		}
		if (it->host == _req_map["Host"] )
			return (it - server.begin());

	}
	return (-1);
}
int request::request_handler(std::vector<server_info>&server)
{
	int index = find_host_index(server);
	if (index == -1)
		return (-1);
	 for (std::map<std::string, location_info>::iterator it = server[index].locations.begin(); it != server[index].locations.end(); ++it)
	 {
		 if(trim(it->first) == trim(_location))
			std::cout << "hello "  << it->second.index << std::endl;
	

	 }
	return(index);
}













