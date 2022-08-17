#pragma once

#include <unistd.h>
#include <vector>
#include <cstring>
#include <iostream>

class CGI {
	private:
		char ** args;
		char ** envp;

	public:
		CGI();
		~CGI();

		void setExecArgs();
		void setEnvVars();
};