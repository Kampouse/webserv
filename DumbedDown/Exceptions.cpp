#include "Exceptions.hpp"

Exceptions::FileOpeningError::FileOpeningError(std::string _arg) : arg("Error opening file: " + _arg + "\n") {}
Exceptions::FileOpeningError::~FileOpeningError() throw() {}
const char * Exceptions::FileOpeningError::what() const throw() { return (arg.c_str()); }


const char * Exceptions::ConfigError::what() const throw() { return ("Error in Configuration file\n"); }


const char * Exceptions::NestedServerError::what() const throw() { return ("Nested servers are not allowed in this project\n"); }


const char * Exceptions::SemicolonError::what() const throw() { return ("Missing ; in Configuration file\n"); }


const char * Exceptions::UnknownFieldError::what() const throw() { return ("Unknown field in Configuration file\n"); }


Exceptions::InvalidFieldError::InvalidFieldError(std::string _arg) : arg(_arg + " field has an invalid argument\n") {}
Exceptions::InvalidFieldError::~InvalidFieldError() throw() {}
const char * Exceptions::InvalidFieldError::what() const throw() { return (arg.c_str()); }
