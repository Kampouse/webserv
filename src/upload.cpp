#include "upload.hpp"

upload::upload() {}
upload::~upload() {}

upload::upload(server &serv, std::pair<std::string, std::string> page,
               std::string buf, unsigned int content_length)
    : serverInfo(serv.serveInfo), rqst(page), buffer(buf), filename("") {
  struct stat s;
  if (content_length > serverInfo.client_max_body_size) {
    serv.resp.set_status_code(413);
    return;
  }
  get_filename();
  if (stat(path.c_str(), &s) == 0) {
    if (!delete_file()) {
      serv.resp.set_status_code(400);
      return;
    }
  }
  write_file(serv);
  serv.resp = response("/upload");
}

bool upload::delete_file() {
  std::ifstream infile(path.c_str());
  if (infile.good()) {
    if (remove(path.c_str()) == -1)
      return false;
  }
  return true;
}

void upload::get_filename() {
  struct stat s;
  size_t pos = buffer.find("filename=\"");
  if (pos != std::string::npos) {
    pos += 10;
    size_t len = buffer.find('"', pos);
    filename = buffer.substr(pos, len - pos);

    std::map<std::string, location_info>::iterator it =
        serverInfo.locations.begin();
    for (; it != serverInfo.locations.end(); it++) {
      if (it->second.upload_dir != "") {
        path.assign(it->second.upload_dir);
        break;
      }
    }
    if (path == "") {
      path.assign("upload");
      if (stat(path.c_str(), &s) != 0)
        mkdir(path.c_str(), S_IRWXG | S_IRWXO | S_IRWXU);
      path.append("/");
      path.append(filename);
    }
  }
}

void upload::write_file(server &serv) {
  std::ofstream ofs(path.c_str(), std::ofstream::out | std::ofstream::binary |
                                      std::ofstream::app);

  size_t start = buffer.find("filename=\"");
  start = buffer.find("\r\n\r\n", start);
  start += 4;

  size_t pos = buffer.find("boundary") + 9;
  std::string boundary = buffer.substr(pos, buffer.find("\r\n", pos) - pos);
  boundary.insert(0, "--");

  const char *str = buffer.c_str();
  char *ptr;
  pos = start;
  while (pos < serv.getTotalRet()) {
    size_t len = strlen(str + pos);
    ptr = (char *)memchr(str + pos, '-', len);
    if (ptr == NULL)
      pos += len + 1;
    else {
      pos = ptr - str + 1;
      if (memcmp(ptr, boundary.c_str(), strlen(boundary.c_str())) == 0) {
        break;
      }
    }
  }
  size_t length = pos - start - 3;
  ofs.write(&buffer[start], length);
  ofs.close();
}
