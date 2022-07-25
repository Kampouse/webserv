#include "request.hpp"
#include "map"

request::request(std::string&request_string): _request_string(request_string)
{
	using std::string;

	int pos  = 0;
	int lpos  = 0;
	string key = "" ;
	string value = "";
	std::map<string,string> req_map;
	// head 
	pos = request_string.find(" ");
	req_map["method"] = request_string.substr(0,pos); 
	lpos = request_string.find(" ",pos+1);
	req_map["path"] = request_string.substr(pos+1,lpos-pos-1);
	pos = request_string.find("HTTP/1.1");
	req_map["version"] = request_string.substr(lpos+1,pos);
	// head
	key = "";
	value = "";
	pos = request_string.find(":");
	while (pos != -1)
	{
		key = request_string.substr(lpos+1,pos-lpos-1);
		lpos = request_string.find("\r\n",pos+1);
		value = request_string.substr(pos+1,lpos-pos-1);
		req_map[key] = value;
		pos = request_string.find(":",lpos);
	}
	_method   = req_map["method"]; 
	_location = req_map["path"]; 
	_version  = req_map["version"];
	_headers  = req_map;
	 for (std::map<string,string>::iterator it=req_map.begin(); it!=req_map.end(); ++it)
	 {
		std::cout << it->first << " => " << it->second << '\n';
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
