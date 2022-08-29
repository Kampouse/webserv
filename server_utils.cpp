#include "Server.hpp" 
#include <sstream>
#include <filesystem>

 std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(WHITESPACES);
    if (first == std::string::npos)
        return str;
    size_t last = str.find_last_not_of(WHITESPACES);
    return str.substr(first, (last - first + 1));
}

std::pair<std::string, std::string> find_page(server &serv, std::string &path)
{
	(void) serv;
	size_t pos = 0;
	std::vector <std::string> allowed_requests;
	allowed_requests.push_back("GET");
	allowed_requests.push_back("POST");
	allowed_requests.push_back("HEAD");
	allowed_requests.push_back("PUT");
	allowed_requests.push_back("DELETE");
	 //refactor to  avoid this
	for (size_t i = 0; i < allowed_requests.size(); i++)
	{
		if (path.find(allowed_requests[i]) != std::string::npos)
		{
			pos = i;
			break;
		}
	}
	std::string page;
	std::string::size_type  end =  path.find("HTTP") - 1;
	std::string::size_type  start =  path.find("/");
	page = path.substr(start, end - start);
	return (std::make_pair<std::string, std::string>(allowed_requests[pos], page));
}

std::string content_typer(std::vector<std::string> &content_type, int index)
{
	std::string content_type_str;
	std::string temp;
	if(index > 2)
	{
		 content_type_str = "img/";
		temp = content_type[index].substr(1, content_type[index].size());
		content_type_str += temp;
	}
	else
	{
		content_type_str = "text/";
		temp = content_type[index].substr(1, content_type[index].size());
		content_type_str += temp;
	}
	return content_type_str;
}
