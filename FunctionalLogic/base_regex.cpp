#include <string>
#include <iostream>
#include <regex>
 
#define REG "hello(, |)"
 
int main()
{
	std::string s1 = "#hello, hello#";
	std::string s2 = std::regex_replace(s1, std::regex(REG), "");
	std::cout << s2 << std::endl;
 
	return 1;
}

