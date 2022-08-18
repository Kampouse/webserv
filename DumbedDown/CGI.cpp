#include "CGI.hpp"

CGI::CGI() {}
CGI::~CGI() {}

CGI::CGI(server_info info, std::pair<std::string, std::string> page)
{
	serverInfo = info;
	request = page.first;
	size_t pos = page.second.find("?");
	path = page.second.substr(0, pos);
	query = page.second.substr(pos + 1, std::string::npos);
	scriptName = path.substr(path.find_last_of('/') + 1, std::string::npos);
	setExecArgs();
}

void CGI::setExecArgs()
{
	args = new char*[3];

	args[0] = new char[25]; // /usr/bin/ path
	args[1] = new char[25]; // file path
	args[2] = NULL;
}

void CGI::setEnvVars()
{
	std::vector<std::string> vars;

	vars.push_back("SERVER_SOFTWARE=webserv");
	vars.push_back("SERVER_NAME=" + serverInfo.server_names);
	vars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	vars.push_back("SERVER_PROTOCOL=HTTP/1.1");
	//vars.push_back("SERVER_PORT=" + serverInfo.port);
	vars.push_back("REQUEST_METHOD=" + request);
	vars.push_back("PATH_INFO=./" + path);
	vars.push_back("PATH_TRANSLATED=./" + path);
	vars.push_back("SCRIPT_NAME=" + scriptName);
	vars.push_back("QUERY_STRING=" + query);
	vars.push_back("REMOTE_ADDR=" + serverInfo.host);
	vars.push_back("CONTENT_TYPE=");
	vars.push_back("CONTENT_LENGTH=");

	envp = new char*[14];
	envp[13] = NULL;

	// Convert string into char

	vars.clear();
}
