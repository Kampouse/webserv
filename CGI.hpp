#pragma once

#include <map>
#include <unistd.h>
#include "sstream" 
#include "Server.hpp"

class server;

class CGI {
private:
  char **args;
  char **envp;
  server_info serverInfo;
  std::string request;
  std::vector<std::string> allowed_requests;
  std::string path;
  std::string query;
  std::string scriptName;
  std::string data;
  std::string body;
  char buffer[100000];
  size_t env_size;
  std::string getExecPath();

  // Convert Numerical type to String
  template <typename T> std::string IntToString(const T &num) {
    std::stringstream ss;
    std::string str;

    ss << num;
    ss >> str;

    return str;
  }

public:
  CGI();
  CGI(server_info info, std::pair<std::string, std::string> page,
      std::string _data);
  ~CGI();

  void setExecArgs();
  void setEnvVars();

  void execCGI();

  std::string get_buffer() { return buffer; }
};
