#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <iostream>

#define DLEVEL 0
/*
	0 - No debug
	1 - errors
	2 - warning
	3 - info
*/

#if DLEVEL == 1
	#define ERROR(x) 						  \
	    std::cout << "***ERROR: " << x << ": "\
		<< "FAILED IN " << __FILE__ << " AT " \
		<< __LINE__ << "\n"; exit(1)

	#define INFO(x)
	#define WARN(x)

#elif DLEVEL == 2 
	#define ERROR(x) 						  \
	    std::cout << "***ERROR: " << x << ": "\
		<< "FAILED IN " << __FILE__ << " AT " \
		<< __LINE__ << "\n"; exit(1)

	#define WARN(x)							  \
		std::cout << "**WARNING:" << x << "\n" 
	#define INFO(x)

#elif DLEVEL == 3					
	#define ERROR(x) 						  \
	    std::cout << "***ERROR: " << x << ": "\
		<< "FAILED IN " << __FILE__ << " AT " \
		<< __LINE__ << "\n"; exit(1)

	#define WARN(x)							  \
		std::cout << "**WARNING:" << x << "\n" 
	#define INFO(x)							  \
		std::cout << "LOG:" << x << "\n"	

#else
		#define ERROR(x)
		#define WARN(x)
		#define INFO(x)

#endif

#if DLEVEL > 1
	#define ASSERT(x)						  \
		if(!(x))							  \
			std::cout << "ASSERT FAILED IN "  \
			<< __FILE__ << " AT " << __LINE__ \
			<< "\n"  
#else
	#define ASSERT(x)
#endif 



void print_separator();
void print_double_separator();


#endif // __DEBUG_H__

