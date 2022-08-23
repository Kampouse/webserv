
#pragma once

#include <map>
#include <string>
#include <vector>
#include <stdbool.h>
#include <netinet/in.h>
#include <iostream> 
#include <fstream> 
#include <ctime>
#include <algorithm> 

#define WHITESPACES "\t\n\v\f\r "

struct location_info {
	std::string								redirect_to;
	int 									redirect_code;
	std::string							root;
	std::map<std::string, std::string>	cgi;
	std::vector<std::string>			allowed_requests;
	std::string							index;
	std::string							upload_dir;
	bool								autoindex;

	location_info() {
		redirect_to = "";
		redirect_code = 0;
		autoindex = false;
	}

	location_info(std::string rooted) {
		autoindex = false;
		root = rooted;
		redirect_to = "";
		redirect_code = 0;
	}

  








	std::string find_error_page(std::string path) const
	{
		std::string data;
		std::ifstream file(path.c_str());
		std::string line;

		while (std::getline(file, line)){
			data += line;
		}

		file.close();
		return data;
	}

	std::string find_content() const
	{
		//open file and read it into a string
		std::string data;
		std::string file_path = root + "/" +  index;
		std::ifstream file(file_path.c_str());
		std::string line;

		while (std::getline(file, line)){
			data += line;
		}

		file.close();
		return data;
	}

	std::string find_type() const
	{
		std::string type = "text/" + this->index.substr(index.find(".") + 1);

		return type;
	}

	friend	std::ostream &operator<<(std::ostream &os , const location_info &test)  {
		time_t rawtime;
	    struct tm * timeinfo;
	    char buffer[80];
	    time (&rawtime);
	    timeinfo = localtime(&rawtime);
	    strftime(buffer,80,"%a, %b %d %H:%M:%S %Y",timeinfo);

		std::string time(buffer);
		std::string time_string = asctime(timeinfo);
		time_string.erase(std::remove(time_string.begin(), time_string.end(), '\n'), time_string.end());
		time_string.insert(time_string.find(' '), " - ");
		os << "root: " << test.root << std::endl;
		os << "index: " << test.index << std::endl;
		os << "upload_dir: " << test.upload_dir << std::endl;
		os << "autoindex: " << test.autoindex << std::endl;
		std::map<std::string, std::string>::const_iterator it;
		for (it = test.cgi.begin(); it != test.cgi.end(); it++) {
			os << "cgi: " << it->first << " " << it->second << std::endl;
		}
		std::vector<std::string>::const_iterator it2;
		for (it2 = test.allowed_requests.begin(); it2 != test.allowed_requests.end(); it2++) {
			os << "allowed_requests: " << *it2 << std::endl;
		}
		std::cout << "content_type: " <<   test.find_type()   << std::endl;
		//maybe wirte this part in a function before sending it so you can reuse the content value
		std::cout << "content_length: " << test.find_content().length() << std::endl;
		std::cout << "time: " << time << std::endl;
		return os;
	}
};

struct server_info {
	std::string								host;
	int										port;
	std::string								server_names;
	std::map<int, std::string>				error_pages;
	unsigned int							client_max_body_size;
	int										server_fd;
	struct sockaddr_in						address;
	std::map<std::string, location_info>	locations;

	server_info() {
		client_max_body_size = 0;
		port = 0;
		server_fd = 0;
		server_names = "localhost";
	}

};

typedef enum parsing_state {
	CONFIG_FIELD,
	BRACE
} parsing_state;
