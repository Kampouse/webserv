#include "response.hpp"
#include <pthread.h>
#include <sstream>
#include "dirent.h"

std::vector<std::string> listFilesRecursively( const char *basePath,std::vector<std::string>&lst,
std::map<std::string,location_info >&locations)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);
	std::vector<std::string> files;
    // Unable to open directory stream
    if (!dir)
	{
        return files;
	}
	locations[basePath].autoindex = true;
	locations[basePath].root  = basePath ;
    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);
				if (dp->d_type == DT_DIR)
				{
					std::string s = path; 
					 s =  s.substr( s.find("/")); 
					lst.push_back(s);
					if( )
					locations[s].autoindex = true;
					locations[s].root = path;
				}
				else if (dp->d_type == DT_REG)
				{

					std::string s = path; 
					 s =  s.substr( s.find("/")); 
					 files.push_back (s);
					locations[s].autoindex = false;
					locations[s].root = s;
					locations[s].index = s;
				}
       }
	
	}
	closedir(dir);

	return files;
    }


std::string readfile(std::string path)
{
	std::ifstream file(path.c_str());
	std::stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	return buffer.str();
}


std::string list_directory(std::string&content_type,std::string&content,int&content_length,location_info local_info, std::map<std::string,location_info> &lst_info  )
{
			content_type = "text/html";
			content = "<!DOCTYPE html><html><head><title>Index of " + local_info.root + 
			"</title></head><body><h1>Index of " + local_info.root + 
			"</h1><table><tr><th>Name</th><th>Last modified</th><th>Size</th></tr>";
				std::vector<std::string> lst;
			 std::vector<std::string> files =	listFilesRecursively(local_info.root.c_str(),lst,lst_info);
			for (std::vector<std::string>::iterator it = lst.begin(); it != lst.end(); ++it)
			{
				std::string s = *it; 
				content += "<tr><td><a href=\"" +  s + "\">" + s  + "</a></td><td>" ;
			}
			if(files.size() != 0)
			{
						for (size_t val = 0; val != files.size(); ++val )
						{
							content += "<tr><td><a href=\"" +  files[val]+ "\">" + files[val] + "</a></td><td>" ;
						}
			}
		
			content += "</table></body></html>";
return content;
}

std::string forbiden_access(std::string&content,std::string&content_type,std::string root) 
{
	content = "<!DOCTYPE html><html><head><title>Forbide to access</title></head><body><h1>Forbiden accces to " 
	+ root + "</h1>";
	content += "</table></body></html>";
	content_type = "text/html";
	return content;
}




std::string  response::build_response(std::map<std::string,location_info> &lst_info )
{
	std::string content;
	time_t rawtime;
    struct tm * timeinfo;
    char time_string[80];
	int content_length;
	std::stringstream ss;
	std::string content_type ;
	 if (status_code == 200 && this->content  != "")
	{
		content = this->content;
		content_length = strlen(this->content.c_str());
		content_type = "text/html";
	}
	else if(status_code != 200)
	{
		content  = 	local_info.find_error_page( error_page[status_code]);
		content_length = content.length();
		content_type = "text/html";
	}
	else if(status_code == 200 && type == "")
	{
		content = local_info.find_content();
		content_type = local_info.find_type();
		if(content_type == "text/plain" && local_info.autoindex == true)
			content = list_directory(content_type,content,content_length,local_info,lst_info);
		else if(content_type == "text/plain" && local_info.autoindex == false)
			forbiden_access(content,content_type,local_info.root);
			content_length = content.length();
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

	 std::string str_path = "." + local_info.root;
	std::string str;
	 if(local_info.index != "" && (str = readfile(str_path)).length() != 0)
				 content_type = local_info.find_type();
	ss << content_length;
	std::string content_length_str = ss.str();
    time (&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(time_string,80,"%a, %b %d %H:%M:%S %Y",timeinfo);
	std::string response = "HTTP/1.1 " + status + "\r\n";
	response += "Date: " + std::string(time_string) + "\r\n";
	if (local_info.redirect_to != "")
		response += "Location: " + local_info.redirect_to + "\r\n";
	if (str.length() !=0)
	 {
		 std::stringstream ssd;
		 ssd << str.length();
		 content_length_str = ssd.str();
		 response += "Content-Length: " + content_length_str + "\r\n";
	 }
	else
		response += "Content-Length: " + content_length_str + "\r\n";
	response += "\r\n";
	if(str != "")
	{
		response += str;
	}
	else
	{
		response += content;
	}
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
