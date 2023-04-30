#pragma once

#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>

#include "Exceptions.hpp"
#include "config_structs.hpp"

#if OS_LINUX
#include <bits/stdc++.h>
#endif

class Exceptions;

class parser {
private:
  std::ifstream config_file_fd;
  std::vector<server_info> servers;
  std::vector<std::string> ext_file;

  void parsefile(std::string path);
  void extractfile(void);
  void check_errors(void);
  void get_server_fields(void);
  void manage_locations(std::vector<std::string>::iterator it);
  void set_max_body_size(void);

public:
  parser();
  parser(std::string _path);
  ~parser();

  std::vector<server_info> &getServers();
};
