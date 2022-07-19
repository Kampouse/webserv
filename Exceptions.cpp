#include "Exceptions.hpp"

Exceptions::FileOpeningError::FileOpeningError(std::string _arg) : arg("Error opening file: " + _arg) {}
Exceptions::FileOpeningError::~FileOpeningError() throw() {}

const char * Exceptions::FileOpeningError::what() const throw() { return (arg.c_str()); }