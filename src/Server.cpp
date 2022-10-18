#include "Server.hpp"
#include "filesystem"
#include "response.hpp"

server::server() {}
server::~server() {}

server::server(server_info servInfo) {
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(servInfo.port);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
  serveInfo = servInfo;
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  fcntl(server_fd, F_SETFL, O_NONBLOCK);
  setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &server_addr,sizeof(server_addr));
  bzero(&(server_addr.sin_zero), 8);
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    return;
  listen(server_fd, 255);
  pollfd serv;
  serv.fd = server_fd;
  serv.events = POLLIN | POLLHUP | POLLERR;
  serv.revents = 0;
  poll_set.push_back(serv);
  contents.push_back(".css");
  contents.push_back(".html");
  contents.push_back(".js");
  contents.push_back(".png");
  contents.push_back(".jpg");
  contents.push_back(".jpeg");
  contents.push_back(".gif");
  contents.push_back(".ico");
}

void server::delete_upload(std::string path) {
  DIR *d;
  dirent *dir;
  std::string file;
  struct stat s;
  int ret = 0;

  d = opendir(path.c_str());
  if (d == nullptr) {
    resp.set_status_code(404);
    return;
  }
  while ((dir = readdir(d))) {
    file.clear();
    file = path;
    if (file.rfind('/') != file.size() - 1)
      file.append("/");
    file.append(dir->d_name);
    if (file.rfind('.') == file.size() - 1)
      continue;
    if (dir->d_type == DT_DIR)
      file.append("/");
    if ((stat(file.c_str(), &s) == 0) && (s.st_mode & S_IFDIR)) {
      delete_upload(file);
      ret = rmdir(file.c_str());
    } else
      ret = unlink(file.c_str());
    if (ret < 0)
      std::cout << "Deleting error:" << errno << "\n";
  }
  rmdir(path.c_str());
  closedir(d);
}

void server::get_content_length(std::string buf) {
  unsigned int len = 0;
  size_t pos = buf.find("Content-Length: ");
  if (pos != std::string::npos) {
    pos += 16;
    while (std::isdigit(buf[pos])) {
      len *= 10;
      len += ((int)(buf[pos]) - 48);
      pos++;
    }
  }
  content_length = len;
}

void server::clear_fd(int i) {
  close(poll_set[i].fd);
  poll_set[i].fd = -1;
  poll_set.erase(poll_set.begin() + i);
}

void server::add_client(void) {
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  int client_fd;
  pollfd client;
  if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr,
                          &client_addr_len)) < 0) {
    perror("accept");
    return;
  }
  fcntl(client_fd, F_SETFL, O_NONBLOCK);
  client.fd = client_fd;
  client.events = POLLIN | POLLHUP | POLLERR | POLLOUT;
  client.revents = 0;
  poll_set.push_back(client);
}

void server::get_data_from_client(int i) {
  total_ret = 0;
  char buf[BUF_SIZE];
  std::string temp;

  int ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
  std::string pathed;
  total_ret += ret;
  get_content_length(buf);

  buffer.clear();
  buffer.insert(buffer.begin(), buf, buf + ret);

  if (ret == BUF_SIZE && content_length > BUF_SIZE) {
    while (ret == BUF_SIZE) {
      bzero(buf, BUF_SIZE);
      ret = recv(poll_set[i].fd, buf, BUF_SIZE, 0);
      total_ret += ret;
      buffer.insert(buffer.end(), buf, buf + ret);
    }
  }

  if (ret < 0) {
    std::cout << "recv error" << std::endl;
    clear_fd(i);
    return;
  } else if (ret == 0) {
    clear_fd(i);
  } else {
    std::string data(buffer.begin(), buffer.end());
    std::string path =
        data.substr(data.find("/"), data.find("HTTP") - data.find("/") - 1);
    for (unsigned int i = 0; i < contents.size(); i++) {
      if (path.find(contents[i]) != std::string::npos) {
        pathed = trim(this->serveInfo.locations["/"].root + path);
        std::ifstream file;

        file.open(pathed.c_str());
        if (!file.is_open())
          continue;
        else {
          file.close();
          std::string content_type = content_typer(contents, i);
          resp = response(pathed, content_type);
          return;
        }
      }
    }

    std::pair<std::string, std::string> page = find_page(data);
    if (page.first == "DELETE" &&
        (this->serveInfo.locations["/"].find_allow_request("DELETE") ||
         this->serveInfo.locations["/"].allowed_requests.size() == 0)) {

      std::string upload_path;
      std::map<std::string, location_info>::iterator it =
          serveInfo.locations.begin();
      for (; it != serveInfo.locations.end(); it++) {
        if (it->second.upload_dir != "") {
          upload_path = it->second.upload_dir;
          break;
        }
      }
      delete_upload(upload_path);
    } else if (page.first == "POST" && page.second == "/upload") {
      upload(*this, page, data, content_length);
    } else if (page.second.find("cgi-bin") != std::string::npos &&
               (this->serveInfo.locations["/"].find_allow_request(page.first) ||
                this->serveInfo.locations["/"].allowed_requests.size() == 0)) {
      CGI cgi(serveInfo, page, data);
      if (strlen(cgi.get_buffer().c_str()) != 0)
      {
        resp = response(cgi.get_buffer());
      }
      else
      {
        resp = response(serveInfo.locations[page.second],this->serveInfo.error_pages, data); 
              resp.set_status_code(500);
      }
    } else {
      if (page.second.find("cgi-bin") != std::string::npos &&
          (this->serveInfo.locations["/"].find_allow_request(page.first) ==
           false)) {
        resp = response(serveInfo.locations[page.second],
                        this->serveInfo.error_pages, data);
        resp.set_status_code(405);
        resp.set_status("405 Method Not Allowed");
        return;
      }
      serveInfo.locations[page.second].len = content_length;
      resp = response(serveInfo.locations[page.second],
                      this->serveInfo.error_pages, data);
    }
  }
}

void server::get_data_from_server(int i) {
  std::string http_response = resp.build_response(serveInfo.locations);
  int ret = send(poll_set[i].fd, http_response.c_str(), http_response.length(), 0);
  if (ret < 0) {
    std::cout << "Error sending response\n" << std::endl;
    clear_fd(i);
  } else {
    clear_fd(i);
  }
}

void server::run() {
  poll(poll_set.data(), poll_set.size(), 50);
  for (unsigned long i = 0; i < poll_set.size(); i++) {
    if (poll_set[i].revents & POLLIN) {
      if (poll_set[i].fd == server_fd)
        add_client();
      else if (poll_set[i].fd != server_fd) {
        get_data_from_client(i);
      }
    }
    if (poll_set[i].revents & POLLOUT) {
      get_data_from_server(i);
    }
  }
}
