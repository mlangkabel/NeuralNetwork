#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>

/**
* @brief Makros to simplify usage of the log manager
*/
#define LOG_INFO(__str__) \
	do \
	{ \
		std::cout << "INFO: " << __str__ << std::endl; \
		/*std::cout << "INFO: " << __str__ << " (" << __FILE__ << "; function: " << __FUNCTION__ << "; line: " << __LINE__ << ")" << std::endl;*/ \
	} \
	while(0)

#define LOG_WARNING(__str__) \
	do \
	{ \
		std::cout << "WARNING: " << __str__ << std::endl; \
		/*std::cout << "WARNING: " << __str__ << " (" << __FILE__ << "; function: " << __FUNCTION__ << "; line: " << __LINE__ << ")" << std::endl;*/ \
	} \
	while(0)

#define LOG_ERROR(__str__) \
	do \
	{ \
		std::cout << "ERROR: " << __str__ << std::endl; \
		/*std::cout << "ERROR: " << __str__ << " (" << __FILE__ << "; function: " << __FUNCTION__ << "; line: " << __LINE__ << ")" << std::endl;*/ \
	} \
	while(0)

#endif // LOGGING_H
