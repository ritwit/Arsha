#include "debug.h"

void print_separator()
{
	for (int i = 0; i < 30; i++)
		std::cout << "--";
	std::cout << std::endl;
}

void print_double_separator()
{
	for (int i = 0; i < 30; i++)
		std::cout << "==";
	std::cout << std::endl;
}
