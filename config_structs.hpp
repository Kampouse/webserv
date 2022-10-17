#pragma once

#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <vector>

#define WHITESPACES "\t\n\v\f\r "

struct location_info {
  std::string redirect_to;
  int redirect_code;
  std::string root;
  std::map<std::string, std::string> cgi;
  std::vector<std::string> allowed_requests;
  std::string index;
  std::string upload_dir;
  unsigned int client_max_body_size;
  bool autoindex;
  int len;
  bool is_dir;

  location_info() {
    autoindex = false;
    upload_dir = "";
    redirect_to = "";
    redirect_code = 0;
    client_max_body_size = 0;
    root = "";
  }
  ~location_info() {}

  location_info(std::string rooted) {
    root = rooted;
    autoindex = false;
    upload_dir = "";
    redirect_to = "";
    redirect_code = 0;
    client_max_body_size = 0;
    is_dir = false;
  }

  location_info &operator=(const location_info &rhs) {
    redirect_code = rhs.redirect_code;
    redirect_to = rhs.redirect_to;
    root = rhs.root;
    cgi = rhs.cgi;
    allowed_requests = rhs.allowed_requests;
    index = rhs.index;
    upload_dir = rhs.upload_dir;
    autoindex = rhs.autoindex;
    is_dir = rhs.is_dir;
    return *this;
  }

  int find_allow_request(std::string request) {
    for (size_t i = 0; i < allowed_requests.size(); i++) {
      if (allowed_requests[i] == request)
        return (true);
    }
    return (false);
  }

  std::string find_error_page(std::string path) const {
    std::string data;
    std::ifstream file(path.c_str());
    std::string line;

    while (std::getline(file, line)) {
      data += line;
    }

    file.close();
    return data;
  }

  std::string find_content() const {
    std::string data;
    std::string file_path = root + "/" + index;
    std::ifstream file(file_path.c_str());
    std::string line;

    while (std::getline(file, line)) {
      data += line;
    }

    file.close();
    return data;
  }

  std::string find_type() const {
    std::vector<std::string> contents;
    std::string val = this->index.substr(index.find(".") + 1);
    contents.push_back("png");
    contents.push_back("jpg");
    contents.push_back("jpeg");
    contents.push_back("gif");
    contents.push_back("ico");
    contents.push_back("svg");

    if (std::find(contents.begin(), contents.end(), val) != contents.end())
      return "image/" + val;
    else {
      contents.clear();
      contents.push_back("css");
      contents.push_back("html");
      contents.push_back("js");
      contents.push_back("txt");
      contents.push_back("json");
      if (std::find(contents.begin(), contents.end(), val) != contents.end())
        return "text/" + val;
      else
        return "text/plain";
    }
    std::string type = "text/" + this->index.substr(index.find(".") + 1);
    return type;
  }

  friend std::ostream &operator<<(std::ostream &os, const location_info &test) {
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%a, %b %d %H:%M:%S %Y", timeinfo);

    std::string time(buffer);
    std::string time_string = asctime(timeinfo);
    time_string.erase(std::remove(time_string.begin(), time_string.end(), '\n'),
                      time_string.end());
    time_string.insert(time_string.find(' '), " - ");
    // os << "root: " << test.root << std::endl;
    // os << "index: " << test.index << std::endl;
    // os << "upload_dir: " << test.upload_dir << std::endl;
    // os << "autoindex: " << test.autoindex << std::endl;
    std::map<std::string, std::string>::const_iterator it;
    for (it = test.cgi.begin(); it != test.cgi.end(); it++) {
      os << "cgi: " << it->first << " " << it->second << std::endl;
    }
    std::vector<std::string>::const_iterator it2;
    for (it2 = test.allowed_requests.begin();
         it2 != test.allowed_requests.end(); it2++) {
      os << "allowed_requests: " << *it2 << std::endl;
    }
    return os;
  }
};

struct server_info {
  std::string host;
  int port;
  std::string server_names;
  std::map<int, std::string> error_pages;
  unsigned int client_max_body_size;
  struct sockaddr_in address;
  std::map<std::string, location_info> locations;

  server_info() {
    client_max_body_size = 0;
    port = 0;
    server_names = "localhost";
  }

  ~server_info() {}

  server_info &operator=(const server_info &rhs) {
    host = rhs.host;
    port = rhs.port;
    server_names = rhs.server_names;
    error_pages = rhs.error_pages;
    client_max_body_size = rhs.client_max_body_size;
    address = rhs.address;
    locations = rhs.locations;
    return *this;
  }
};

typedef enum parsing_state { CONFIG_FIELD, BRACE } parsing_state;
