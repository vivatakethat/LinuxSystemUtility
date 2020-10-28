// regex_match example
#include <iostream>
#include <string>
#include <regex>
using namespace std;

int main()
{

  
     //   regex::smatch path_match;
      std::smatch path_match;
    //  std::regex b("^/SpecialTrojan/?$");
     regex  b("(Geek)(.*)"); // Geeks followed by any character 
    string a="/SpecialTrojan";
 
   if(std::regex_match(a.begin(), a.end(), b))
    {
   //cout<<path_match[0]<<endl;
   
   }
  return 0;
}


