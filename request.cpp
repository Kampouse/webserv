#include "request.hpp"
#include "utils.hpp"
#include <sstream>
#include <string>
#include "map"

request::request(std::string&request_string): _request_string(request_string)
{
	using std::string;

	std::map<string,string> req_map;

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
	_version = request_string.substr(request_string.find ("HTTP/1.1"));

	int temp  =  request_string.find ("\r\n") + 2;
	it = request_string.begin() + temp;
	it_second = it;
/*
	while(it != request_string.end())
	{
		if (*it == ':')
		{
			it++;
			break;
		}
		it++;
	}
	key =	request_string.substr(it_second - request_string.begin(), it - it_second - 1);
	it_second = it + 1;
	while(it != request_string.end())
	{
		if (*it == '\r')
		{
			it++;
			break;
		}
		it++;
	}
*/

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
// dispaly_vetctor 
	for (std::vector<std::string>::iterator it = result.begin(); it != result.end(); ++it)
	{

		std::string str = *it;

		if (str.find(":") != std::string::npos)
		{
			 key = str.substr(0 , str.find(":"));
			value = str.substr(str.find(":") + 1);
			_req_map[ key ] = value;
		}
	}
}

void request::dispaly_map(void)
{
	std::map<std::string, std::string>::iterator it;
	for (it = _req_map.begin(); it != _req_map.end(); it++)
	{
		std::cout << ":->" << it->first << "<-:->" << it->second << "<-"  <<std::endl;
	}
}


request::request(void): _request_string ("")
{

}


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

 void request::find_host(std::string string) 
{
	(void)string;
	std::cout << "hello" <<  _req_map["Host"]  << std::endl;

}









