#pragma once

#include <iostream>
#include <exception>

class Exceptions {
	public:
		Exceptions();
		~Exceptions();

		class FileOpeningError : public std::exception {
			private:
				std::string arg;
			public:
				FileOpeningError(std::string arg);
				virtual ~FileOpeningError() throw();
				virtual const char * what() const throw();
		};
};