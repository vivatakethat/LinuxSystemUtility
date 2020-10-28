


#include <iostream>
#include <fstream>
 
#include <regex>
using namespace std;

int main (int argc,char *argv[]) {
  ifstream myfile (argv[1]);
//  cout<<"myfile is:"<<argv[1]<<endl;
   string line;
   string str_txt;
  if (myfile.is_open())
  {
      while(getline(myfile,line))
      {
           const std::regex txt_regex("(?:]|:) usb (.*?): ");
        bool isGetUSB= std::regex_match(line, txt_regex);//take:str_buffer USB记录信息
       if( isGetUSB )
        {      str_txt.append(line);
 
         str_txt+='\n'; //take:加上换行符!
           cout<<str_txt<<endl;
         }
      }
  //  myfile.close();
  }
  else cout << "Unable to open file";
//cout<<str_txt;


ofstream targetfile("target.txt") ;
if(targetfile.fail())
{
  cout<<"write file is failed!"<<endl;
}
else
{
  targetfile<<str_txt;
}

  return 0;
}

