#pragma once

#include <iostream>
#include <string>

struct RootNullException : public std::exception
{
	std::string message;
	RootNullException(std::string _message) : message(_message) {}
	~RootNullException() throw () {} // Updated
	const char* what() const throw() { return message.c_str(); }
};