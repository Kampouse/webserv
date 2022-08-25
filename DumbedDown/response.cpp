#include "response.hpp"
#include <sstream>

void response::set_status_code(int code) { status_code = code; }

std::string readfile(std::string path)
{
	std::ifstream file(path.c_str());
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}

std::string  response::build_response(void)
{
	std::string content;
	time_t rawtime;
    struct tm * timeinfo;
    char time_string[80];
	int content_length;
	std::stringstream ss;
	std::string content_type ;
	std::cout << status_code << "\n";
	 if (status_code == 200 && this->content  != "")
	{
		std::cout << "hello " << this->content << std::endl;
		content = this->content;
		content_length = strlen(this->content.c_str());
		content_type = "text/html";
	}
	else if(status_code != 200)
	{
		content  = 	local_info.find_error_page( error_page[status_code]);
		std::cout << "error " << content << std::endl;
		content_length = content.length();
		content_type = "text/html";
	}
	else if(status_code == 200 && type == "")
	{
		content = local_info.find_content();
		content_length = content.length();
		content_type = local_info.find_type();
	}
	else if (status_code == 200 && type != "")
	{
		content = readfile(path);
		content_length = content.length();
		content_type = type;
	}
	
	else
	{
		content = local_info.find_error_page( error_page[status_code]);
		content_length = content.length();
		content_type = "text/html";
	}
	ss << content_length;
	std::string content_length_str = ss.str();
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_string,80,"%a, %b %d %H:%M:%S %Y",timeinfo);
	std::string response = "HTTP/1.1 " + status + "\r\n";
	response += "Date: " + std::string(time_string) + "\r\n";
	if (local_info.redirect_to != "")
		response += "Location: " + local_info.redirect_to + "\r\n";
	response += "Content-Type: " + content_type + "\r\n";
	response += "Content-Length: " + content_length_str  + "\r\n";
	response += "\r\n";
	std::cout << response << std::endl;
	response += content;
	return response;
}

response::response():path(""){}
response::response(std::string reponse_string):content(reponse_string),status_code(200){}

response::response(std::string &path,std::string &type):path(path),type(type)
{
	if (path == "")
	{
		status_code = 404;
		status = "404 Not Found";
	}
	else
	{
		status_code = 200;
		status = "200 OK";
	}
}
void response ::set_response(int status_code)
{
	std::stringstream  ss;
	std::string status;
	ss << status;
	status = ss.str();
	if(status_code > 300  && status_code < 400)
	{
		this->status_code = status_code;
		this->status = "200 OK";
	}
	else if(status_code > 400 && status_code < 500)
	{
		this->status_code = status_code;
		status = "400 Bad Request";
	}
	else if(status_code > 500 && status_code < 600)
	{
		this->status_code = status_code;
		status = "500 Internal Server Error";
	}
	else
	{
		this->status_code = status_code;
		status = "200 OK";
	}
}

response::response(location_info local_info, std::map<int, std::string> error_page, std::string &path) : error_page(error_page), path(path)
{
	this->type = "";
	this->local_info = local_info;
	this->path = path;

	if(local_info.redirect_to != "")
	{
		this->status = "301 Moved Permanently"; 
		this->status_code = 301;
	}
	else if (local_info.root == "")
	{
		this->status = "404 Not Found";
		this->status_code = 404;

	}
	else
	{
		this->status = "200 OK";
		this->status_code = 200;

	}
}
