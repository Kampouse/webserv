#include "CGI.hpp"

CGI::CGI() {}
CGI::~CGI() {}

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
	vars.push_back("SERVER_NAME="); // Get server_name from server
	vars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	vars.push_back("SERVER_PROTOCOL=HTTP/1.1");
	vars.push_back("SERVER_PORT="); // Get port from server
	vars.push_back("REQUEST_METHOD="); // GET/POST/DELETE from request
	vars.push_back("PATH_INFO="); // Get script path
	vars.push_back("PATH_TRANSLATED="); // Get script path
	vars.push_back("SCRIPT_NAME="); // Get script name
	vars.push_back("QUERY_STRING="); // Get query (Info which follows the ? in the URL which referenced the script)
	vars.push_back("REMOTE_ADDR="); // Get host from server
	vars.push_back("CONTENT_TYPE=");
	vars.push_back("CONTENT_LENGTH=");

	envp = new char*[14];
	envp[13] = NULL;

	// Convert string into char

	vars.clear();
}
