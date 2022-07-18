#include "parser.hpp"
#include <cstdio>
#include <iterator>
#include <sstream>


static std::string trim(const std::string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

bool is_semicolon(std::string str) {
  if (str.back() == ';')
    return true;
  return false;
}
// there maybe some error with  the semicolon at the end 
void parser ::check_for_error(void) {
  std::string line;
  int bracket_state = 0;
  int state = 5;
  while (getline(config_file_fd, line)) {

    // this line assume that the config file is well formed
    if (line.find("server") != std::string::npos &&
        line.find("{") != std::string::npos && bracket_state == 0) {
      state = 0;
      bracket_state = 1;
    } else if (line.find("{") != std::string::npos) {
      state = 2;
      bracket_state++;
    } else if (line.find("}") != std::string::npos) {
      bracket_state--;
      state = 3;
    }

    if (is_semicolon(trim(line)) && bracket_state > 0 && state == 5)
		continue ;
    else if (state == 5 && trim(line).size() == 0)
      continue ;
    else if (state == 5)
			throw  std::runtime_error( "missing ;  at" );
    state = 5;
  }
  if (bracket_state != 0) {
	std::runtime_error( "Error in config file");
  }
}
void parser::get_server_fields(void) {


  server_info     server;
  location_info   location;
  config_file_fd.clear();
  config_file_fd.seekg(0, std::ios::beg);
  std::string line;
  getline(config_file_fd, line);
  std::string field;
  std::string value;
  std::istringstream ss((std::string(line)));
  std::map <std::string, std::string> config_map;
  std::set<std::string> field_list = {"listen", "server_name", "index", "root"};
  while (getline(config_file_fd, line)) {
    ss = std::istringstream((std::string(line)));
    while (getline(ss, field, ' ')) {
      field = trim(field);
      if (field_list.find(field) != field_list.end()) 
		{
			getline(ss, value, ';');
			config_map[field] = trim(value);
		}
	  else if (field == "location")
		{
					// 
			std::cout << trim(line.substr( line.find("location") + 8 )) << std::endl;
			getline(ss, value, '{');
			std::cout << trim(value) << std::endl;
			location.root = trim(value); 
			while(getline(config_file_fd,line)) 
			{
				if(line.find("}") != std::string::npos)
					break;
					ss = std::istringstream((std::string(line)));
				while (getline(ss, field, ' ')) 
				{
					getline (ss, field, ';');
				std::cout <<  trim(field) << std::endl;
				}
			}
		}
    }
  }
  if ((field_list.size() == config_map.size()) || (field_list.size() != config_map.size() && config_map["server_name"] == ""))
		config_maps.push_back(config_map);
  else 
	throw std::runtime_error("Error in config file");
}

parser::parser(char *str) {
  std::cout << "Constructor called" << std::endl;
  this->path = str;
  std::cout << "Path: " << this->path << std::endl;
  this->config_file_fd.open(str);
  if (this->config_file_fd.fail()) {
    std::cout << "Error opening file open(): " << str << std::endl;
	throw std::runtime_error("Error opening file");
  }
}

parser::parser() {}

parser::~parser(void) { this->config_file_fd.close(); }

parser::parser(const parser &src) {
  std::cout << "Copy constructor called" << std::endl;
  // maybe herror here
  this->path = src.path;
  this->config_file_fd.open(src.path);
  if (this->config_file_fd.fail()) {
    std::cout << "Error opening file open(): " << src.path << std::endl;
	throw std::runtime_error("Error opening file");
  }
}
std::string parser::get_server_path(void) { return (this->path); }
//
