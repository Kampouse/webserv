#include "CGI.hpp"

CGI::CGI() {}
CGI::~CGI() {}

CGI::CGI(server_info info, std::pair<std::string, std::string> page, std::string _data)
{
	serverInfo = info;
	request = page.first;
	data = _data;
	size_t pos = page.second.find("?");
	path = info.locations["/"].root + page.second.substr(0, pos);
	query = page.second.substr(pos + 1, std::string::npos);
	scriptName = path.substr(path.find_last_of('/') + 1, std::string::npos);

	pos = data.find("Accept-Language: ");
	pos = data.find("\r\n\r\n", pos);
	pos += 4;
	body = data.substr(pos);
	setEnvVars();
	setExecArgs();
	execCGI();
}

std::string CGI::getExecPath()
{
	std::string scriptExt = scriptName.substr(scriptName.find('.'), std::string::npos);
	std::map<std::string, std::string>::iterator cgi_it;
	if ((cgi_it = serverInfo.locations["/"].cgi.find(scriptExt)) != serverInfo.locations["/"].cgi.end())
		return (cgi_it->second);
	std::cout << "Invalid File Extension\n";
	return "";
}

void CGI::setExecArgs()
{
	size_t len;
	args = new char*[3];

	std::string execPath; 
	execPath = getExecPath();
	if (execPath == "")
		return ;
	
	len = execPath.length() + 1;
	args[0] = new char[len];
	strcpy(args[0], execPath.c_str());

	len = path.length() + 1;
	args[1] = new char[len];
	strcpy(args[1], path.c_str());

	args[2] = NULL;
}

void CGI::setEnvVars()
{
	std::vector<std::string> vars;
	size_t pos = 0;

	vars.push_back("SERVER_SOFTWARE=webserv");
	vars.push_back("SERVER_NAME=" + serverInfo.server_names);
	vars.push_back("GATEWAY_INTERFACE=CGI/1.1");
	vars.push_back("SERVER_PROTOCOL=HTTP/1.1");
	vars.push_back("SERVER_PORT=" + IntToString( serverInfo.port));
	vars.push_back("REQUEST_METHOD=" + request);
	vars.push_back("PATH_INFO=" + path);
	vars.push_back("PATH_TRANSLATED=" + path);
	vars.push_back("SCRIPT_NAME=" + scriptName);
	vars.push_back("QUERY_STRING=" + query);
	vars.push_back("REMOTE_ADDR=" + serverInfo.host);
	vars.push_back("CONTENT_TYPE=text/html");
	vars.push_back("CONTENT_LENGTH=" + IntToString(body.size()));

	if (request == "POST")
	{
		pos = body.find("name=") + 5;
		vars.push_back("NAME=" + body.substr(pos, body.find('&') - pos));

		pos = body.find("email=") + 6;
		vars.push_back("EMAIL=" + body.substr(pos, body.find('&', pos) - pos));

		pos = body.find("message=") + 8;
		vars.push_back("MESSAGE=" + body.substr(pos));
	}

	envp = new char*[17];
	envp[13] = NULL;
	envp[14] = NULL;
	envp[15] = NULL;
	envp[16] = NULL;

	env_size = vars.size();

	size_t i = 0;
	size_t len;
	for ( std::vector<std::string>::iterator it = vars.begin(); it != vars.end(); it++, i++) {
		len = it->length() + 1;
		envp[i] = new char[len];
		strcpy(envp[i], it->c_str());
	}

	vars.clear();
}

void CGI::execCGI()
{
	int fd[2];
	int in;
	pid_t pid;
	int status;

	int fd_f= 0;

   if( (fd_f = open(args[0], O_RDONLY)) < 0)
	   return ;
   else
	   close(fd_f);
	pipe(fd);

	in = dup(STDIN_FILENO);

	pid = fork();
	if (pid == 0)
	{
		dup2(in, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(in);
		close(fd[0]);
		close(fd[1]);
		execve(args[0], args, envp);
		exit(1);
		std::cout << "execve failed\n";
	}
	else
	{
		waitpid(pid, &status, 0);
		dup2(fd[0], in);

		bzero(buffer, 100000);
		read(fd[0], buffer,100000);
		close(fd[0]);
		close(fd[1]);

		delete[] args[0];
		delete[] args[1];
		delete[] args;

		for (size_t i = 0; i < env_size; i++)
			delete[] envp[i];
		delete[] envp;
	}
	close(in);
}
