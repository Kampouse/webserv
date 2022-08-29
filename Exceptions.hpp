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
				FileOpeningError(std::string _arg);
				virtual ~FileOpeningError() throw();
				virtual const char * what() const throw();
		};

		class ConfigError : public std::exception {
			public:
				virtual const char * what() const throw();
		};

		class NestedServerError : public std::exception {
			public:
				virtual const char * what() const throw();
		};

		class SemicolonError : public std::exception {
			public:
				virtual const char * what() const throw();
		};

		class UnknownFieldError : public std::exception {
			public:
				virtual const char * what() const throw();
		};

		class InvalidFieldError : public std::exception {
			private:
				std::string arg;
			public:
				InvalidFieldError(std::string _arg);
				virtual ~InvalidFieldError() throw();
				virtual const char * what() const throw();
		};
};